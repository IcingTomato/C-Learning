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
    $FileNames = $modifiedFiles
    
    # If there are more than two files, show only the first two and add "etc."
    if ($FileNames.Count -gt 2) {
        $filesOutput = ($FileNames[0..1] -join ", ") + ", etc."
    } else {
        $filesOutput = ($FileNames -join ", ")
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