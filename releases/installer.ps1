# Installer for Windows
$zip = "build-windows.zip"

Write-Host "Downloading $zip..."
Invoke-WebRequest -Uri "https://github.com/Eliaslopes248/Boilr-CLI-TOOL/releases/download/v1.0.1/$zip" -OutFile $zip

$temp = "temp_dir"
Write-Host "Extracting br.exe..."
Expand-Archive -Path $zip -DestinationPath $temp

Copy-Item -Path "$temp\br.exe" -Destination "." -Force

Remove-Item -Recurse -Force $temp,$zip
Write-Host "✅ br.exe is ready in $(Get-Location)"
