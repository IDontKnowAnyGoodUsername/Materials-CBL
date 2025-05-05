param([string]$sourceFile)

if (-not $sourceFile) {
    Write-Host "Usage: .\build.ps1 file.c"
    exit
}

$exe = [System.IO.Path]::ChangeExtension($sourceFile, ".exe")

gcc $sourceFile -o $exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compiled successfully. Running:"
    .\$exe
} else {
    Write-Host "Compilation failed."
}
