#!/bin/bash

# 如果没有传入参数，脚本将使用默认的commit消息
gitcommit="$1"

# 执行git pull更新仓库
echo "Pulling latest changes from the repository..."
git pull
echo " "

# 获取当前脚本所在的目录
CURRENT_DIRECTORY=$(dirname "$(realpath "$0")")

# 查找所有二进制可执行文件（ELF格式）
echo "Searching for binary files..."
# 找出所有文件并用file命令过滤出ELF二进制文件
BINARY_FILES=$(find "$CURRENT_DIRECTORY" -type f -not -path "*/\.*" -exec file {} \; | grep "ELF" | cut -d':' -f1)

# 将结果转换为数组以便计数
mapfile -t BINARY_FILES_ARRAY <<< "$BINARY_FILES"

# 检查是否找到二进制文件
if [ ${#BINARY_FILES_ARRAY[@]} -eq 0 ]; then
    echo "No binary files found."
else
    echo "The following binary files will be deleted:"
    for binary in "${BINARY_FILES_ARRAY[@]}"; do
        if [ -n "$binary" ]; then
            echo "$binary"
        fi
    done
    
    # 删除所有找到的二进制文件
    for binary in "${BINARY_FILES_ARRAY[@]}"; do
        if [ -n "$binary" ]; then
            rm "$binary"
        fi
    done
    
    echo " "
    echo "Cleanup completed!"
fi

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