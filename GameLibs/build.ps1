# PowerShell script to rebuild all .sln files

# Get all .sln files recursively
$projs = Get-ChildItem -Path . -Recurse -Filter *.sln | Select-Object -ExpandProperty FullName

# Build Modules first
foreach ($proj in $projs) {
	if ($proj -match "Modules") {
		$batContent = @"
call "%VS80COMNTOOLS%\vsvars32.bat"
devenv /Rebuild Debug "$proj"
devenv /Rebuild Release "$proj"
"@
		Set-Content -Path buildTmp.bat -Value $batContent -Encoding ASCII
		& .\buildTmp.bat
	}
}

# Build other solutions
foreach ($proj in $projs) {
	if ($proj -notmatch "Modules") {
		$batContent = @"
call "%VS80COMNTOOLS%\vsvars32.bat"
devenv /Rebuild Debug "$proj"
if ERRORLEVEL 1 exit /b 1
devenv /Rebuild Release "$proj"
if ERRORLEVEL 1 exit /b 1
"@
		Set-Content -Path buildTmp.bat -Value $batContent -Encoding ASCII
		& .\buildTmp.bat
	}
}

# Clean up temporary batch file
Remove-Item -Path buildTmp.bat -Force
