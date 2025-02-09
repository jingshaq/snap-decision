@echo off
@REM 编译asm.vcxproj
set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\amd64\MSBuild.exe"

if not exist %MSBUILD% (
    echo MSBuild not found at expected location.
    echo Please update the path to match your Visual Studio installation.
    exit /b 1
)


%MSBUILD% asm.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v142  



if errorlevel 1 (
    echo Build failed
    exit /b 1
) else (
    echo Build completed successfully
    exit /b 0
)

@REM objdump -d example.obj