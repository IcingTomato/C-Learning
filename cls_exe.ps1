# If no parameter is passed, the script will use the default commit message: "Change: Changed $filesOutput"
param
(
    [string]$gitcommit = $null
)

# Perform git pull to update the repository
Write-Host "Pulling latest changes from the repository..."
git pull
Write-Host " "

# Get the directory where the current script is located
$CurrentDirectory = Split-Path -Parent $MyInvocation.MyCommand.Definition

# Get all .exe files in the directory and its subdirectories
$ExeFiles = Get-ChildItem -Path $CurrentDirectory -Filter *.exe -Recurse

if ($ExeFiles.Count -eq 0) {
    Write-Host "No .exe files found."
} else {
    Write-Host "The following .exe files will be deleted:"
    $ExeFiles | ForEach-Object {
        Write-Host $_.FullName
    }

    # Delete all found .exe files
    $ExeFiles | Remove-Item -Force

    Write-Host " "
    Write-Host "Cleanup completed!"
}

# Get modified files using git diff instead of git status
$modifiedFiles = git diff --name-only --cached
$modifiedFiles += git diff --name-only
$modifiedFiles += git ls-files --others --exclude-standard

# Remove duplicates
$modifiedFiles = $modifiedFiles | Select-Object -Unique

if (-not $modifiedFiles) {
    Write-Host " "
    Write-Host "No uncommitted changes."
    Write-Host " "
    $filesOutput = ""
} else {
    # 转换为数组以确保正确处理单个文件的情况
    $FileNames = @($modifiedFiles)
    
    # 确保所有元素都被当作数组处理
    Write-Host "Debug - Number of changed files:" $FileNames.Count
    $FileNames | ForEach-Object { Write-Host "File: $_" }
    
    # If there are more than two files, show only the first two and add "etc."
    if ($FileNames.Count -gt 2) {
        $filesOutput = "$($FileNames[0]), $($FileNames[1]), etc."
    } else {
        # 使用字符串拼接方式构建输出，确保正确应用分隔符
        if ($FileNames.Count -eq 1) {
            $filesOutput = $FileNames[0]
        } else {
            $filesOutput = "$($FileNames[0]), $($FileNames[1])"
        }
    }
    Write-Host "Changed files: $filesOutput"
}

# 如果没有传递 -gitcommit 参数，则使用默认值
if (-not $gitcommit) {
    $gitcommit = "Change: Changed $filesOutput"
}

# 确保 commit 消息正确加引号
$gitcommitQuoted = "`"$gitcommit`""

# Stage all changes, commit, and push to Git
Write-Host " "
git add .
Write-Host " "
Write-Host "Committing with message: $gitcommit"
git commit -m $gitcommitQuoted
Write-Host " "
git push