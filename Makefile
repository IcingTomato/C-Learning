# Makefile for C Learning project

# 如果制表符被替换为空格，请使用以下命令确保使用制表符缩进
# sed -i 's/^    /\t/g' Makefile

# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -g

# 编译指定的C文件
# 使用方法: make FILE=path/to/yourfile.c
.PHONY: all
all:
	@if [ -z "$(FILE)" ]; then \
	    $(MAKE) help; \
	else \
	    $(MAKE) compile; \
	fi

# 具体的编译过程
.PHONY: compile
compile:
	@if [ -z "$(FILE)" ]; then \
	    echo "错误: 请指定要编译的C文件"; \
	    echo "使用方法: make FILE=path/to/yourfile.c"; \
	    exit 1; \
	fi
	@if [ ! -f "$(FILE)" ]; then \
	    echo "错误: 文件 $(FILE) 不存在"; \
	    exit 1; \
	fi
	@echo "编译文件: $(FILE)"
	@$(CC) $(CFLAGS) -o $(basename $(FILE)) $(FILE)
	@echo "编译完成: $(basename $(FILE))"

# 清理所有二进制文件
.PHONY: clean
clean:
	@echo "查找并删除所有二进制文件..."
	@find . -type f -executable -not -path "*/\.*" > /tmp/exec_files.txt
	@while IFS= read -r file; do \
	    if file "$$file" | grep -q "ELF"; then \
	        echo "删除: $$file"; \
	        rm "$$file"; \
	    fi \
	done < /tmp/exec_files.txt
	@rm -f /tmp/exec_files.txt
	@echo "清理完成!"

# 显示帮助信息
.PHONY: help
help:
	@echo "C-Learning Makefile 帮助"
	@echo "------------------------"
	@echo "可用命令:"
	@echo "  make FILE=path/to/file.c  - 编译指定的C文件"
	@echo "  make clean                - 删除所有二进制文件"
	@echo "  make help                 - 显示此帮助信息"