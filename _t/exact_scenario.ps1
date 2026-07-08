# Simulate the finding's EXACT scenario:
# user opens powershell, cd's to a project folder, then runs `powershell -File criar.ps1`
# We measure what [Environment]::CurrentDirectory is INSIDE the child -File process.

# Step 1: parent does cd (like user doing 'cd C:\MeusProjetos')
Set-Location 'C:\Windows'
Write-Host "[parent] Get-Location      = $((Get-Location).Path)"
Write-Host "[parent] Env.CurrentDir     = $([Environment]::CurrentDirectory)"
Write-Host "[parent] stale?             = $((Get-Location).Path -ne [Environment]::CurrentDirectory)"
Write-Host ""

# Step 2: parent spawns child via -File (exactly like criar.bat line 15 / finding scenario)
$child = @'
Write-Host "[child]  Get-Location      = $((Get-Location).Path)"
Write-Host "[child]  Env.CurrentDir     = $([Environment]::CurrentDirectory)"
Write-Host "[child]  GetFullPath(test.c) = $([System.IO.Path]::GetFullPath('test.c'))"
Write-Host "[child]  lands in project?  = $([System.IO.Path]::GetFullPath('test.c') -like 'C:\Windows\*')"
'@
$child | Set-Content -Encoding ascii child_probe.ps1
Write-Host "[parent] spawning: powershell -NoProfile -File child_probe.ps1"
Write-Host "------------------------------------------------------------"
& powershell.exe -NoProfile -File child_probe.ps1
Write-Host "------------------------------------------------------------"
