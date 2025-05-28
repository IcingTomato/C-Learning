#!/bin/bash

# 如果没有传入参数，脚本将使用默认的commit消息
gitcommit="$1"

# 执行git pull更新仓库
echo "Pulling latest changes from the repository..."
git pull
echo " "

# 获取当前脚本所在的目录
CURRENT_DIRECTORY=$(dirname "$(realpath "$0")")
echo "Working directory: $CURRENT_DIRECTORY"

# 查找所有可执行二进制文件的更可靠方法
echo "Searching for binary executable files..."

# 使用更详细的搜索方式找到可执行文件并过滤出二进制文件
# 先找出所有可执行文件，然后用file命令检查，只保留ELF格式的文件
find "$CURRENT_DIRECTORY" -type f -executable -not -path "*/\.*" > /tmp/exec_files.txt

# 添加调试输出
echo "Found executable files:"
cat /tmp/exec_files.txt
echo "-------------------------"

# 初始化二进制文件列表
> /tmp/binary_files.txt

# 遍历所有可执行文件，过滤出二进制文件
while IFS= read -r file; do
    if file "$file" | grep -q "ELF"; then
        echo "$file" >> /tmp/binary_files.txt
    fi
done < /tmp/exec_files.txt

# 读取二进制文件列表到数组
mapfile -t BINARY_FILES < /tmp/binary_files.txt

# 检查是否找到二进制文件
if [ ${#BINARY_FILES[@]} -eq 0 ]; then
    echo "No binary files found."
else
    echo "The following binary files will be deleted:"
    for binary in "${BINARY_FILES[@]}"; do
        if [ -n "$binary" ]; then
            echo "$binary"
        fi
    done
    
    # 删除所有找到的二进制文件
    for binary in "${BINARY_FILES[@]}"; do
        if [ -n "$binary" ] && [ -f "$binary" ]; then
            echo "Deleting: $binary"
            rm "$binary"
        fi
    done
    
    echo " "
    echo "Cleanup completed!"
fi

# 清理临时文件
rm -f /tmp/exec_files.txt /tmp/binary_files.txt

# 获取修改过的文件列表
modified_files_staged=$(git diff --name-only --cached)
modified_files_unstaged=$(git diff --name-only)
untracked_files=$(git ls-files --others --exclude-standard)

# 合并所有文件列表并删除重复项
all_modified_files="$modified_files_staged $modified_files_unstaged $untracked_files"
modified_files=($(echo "$all_modified_files" | tr ' ' '\n' | sort | uniq | grep -v "^$"))

if [ ${#modified_files[@]} -eq 0 ]; then
    echo " "
    echo "No uncommitted changes."
    echo " "
    files_output=""
else
    # 输出调试信息
    echo "Debug - Number of changed files: ${#modified_files[@]}"
    for file in "${modified_files[@]}"; do
        echo "File: $file"
    done
    
    # 根据修改文件数量格式化输出
    if [ ${#modified_files[@]} -gt 2 ]; then
        files_output="${modified_files[0]}, ${modified_files[1]}, etc."
    elif [ ${#modified_files[@]} -eq 2 ]; then
        files_output="${modified_files[0]}, ${modified_files[1]}"
    elif [ ${#modified_files[@]} -eq 1 ]; then
        files_output="${modified_files[0]}"
    else
        files_output=""
    fi
    
    # 输出调试信息
    echo "Debug - Formatted output: [$files_output]"
fi

# 如果没有提供git commit参数，使用默认值
if [ -z "$gitcommit" ]; then
    gitcommit="Change: Changed $files_output"
fi

# 提交所有更改并推送到Git
echo " "
git add .
echo " "
echo "Committing with message: $gitcommit"
git commit -m "$gitcommit"
echo " "
git push