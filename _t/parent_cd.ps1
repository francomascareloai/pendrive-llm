Set-Location 'C:\Windows'
Write-Host "parent Get-Location: $((Get-Location).Path)"
Write-Host "parent Env.CD: $([Environment]::CurrentDirectory)"
# spawn child like criar.bat does
& powershell.exe -NoProfile -Command "Write-Host ('child Env.CD: ' + [Environment]::CurrentDirectory)"
