REM Remove build/Debug/Release/lib/bin/obj directories recursively
for /d /r . %%d in (build,Debug,Release,lib,bin,obj) do (
    if exist "%%d" rd /s /q "%%d"
)

REM Remove .vs/.vscode directories recursively
::for /d /r . %%d in (.vs,.vscode) do (
::    if exist "%%d" rd /s /q "%%d"
::)

REM Remove files with specific extensions recursively
for /r . %%f in (*.user) do (
    if exist "%%f" del /f /q "%%f"
)
