param(
    [string]$gitcommit = "Change: Changed several files"
)

# Get the directory where the current script is located
$CurrentDirectory = Split-Path -Parent $MyInvocation.MyCommand.Definition

# Get all .exe files in the same directory
$ExeFiles = Get-ChildItem -Path $CurrentDirectory -Filter *.exe

if ($ExeFiles.Count -eq 0) 
{
    Write-Host "No .exe files found."
} 

else {
    Write-Host "The following .exe files will be deleted:"
    $ExeFiles | ForEach-Object {
        Write-Host $_.FullName
    }

    # Delete all found .exe files
    $ExeFiles | Remove-Item -Force

    Write-Host "Cleanup completed!"
}

# Stage all changes, commit, and push to Git
git add .
git commit -m $gitcommit
git push