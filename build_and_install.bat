@echo off
chcp 65001 >nul
echo ========================================
echo    HappyWorld - 一键编译 + 部署脚本
echo ========================================
echo.

set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%build32
set INSTALL_DIR=%ROOT_DIR%install

REM 检测 VS
for /f "usebackq tokens=*" %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set VS_PATH=%%i
if "%VS_PATH%"=="" (
    echo [错误] 未找到 Visual Studio 2022
    pause
    exit /b 1
)

call "%VS_PATH%\Common7\Tools\VsDevCmd.bat" -arch=x86 >nul 2>&1

REM 检查是否已有 CMake 缓存，避免重复配置
echo [1/3] 检查 CMake 配置 ...
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo     首次配置 CMake ...
    cmake -S %ROOT_DIR% -B %BUILD_DIR% -G "Visual Studio 17 2022" -A Win32 >nul 2>&1
    if %errorlevel% neq 0 (
        echo [错误] CMake 配置失败
        pause
        exit /b 1
    )
) else (
    echo     已有缓存，跳过配置
)
echo [OK]

echo [2/3] 增量编译 HappyWorld 项目 ...
msbuild %BUILD_DIR%\HappyWorld.vcxproj /p:Configuration=Release /p:Platform=Win32 /t:Build /v:m /m /nologo
if %errorlevel% neq 0 (
    echo [错误] 编译失败
    pause
    exit /b 1
)
echo [OK]

echo [3/3] 部署到 install 目录 ...
if not exist %INSTALL_DIR% mkdir %INSTALL_DIR%
cmake --install %BUILD_DIR% --prefix %INSTALL_DIR% >nul 2>&1

REM 复制所有依赖 DLL
for /r %ROOT_DIR%cocos2d-x\external %%f in (*.dll) do (
    copy "%%f" %INSTALL_DIR%\ >nul 2>nul
)

REM 同步 Resources 到编译输出目录
echo     同步 Resources 到编译输出 ...
if not exist "%BUILD_DIR%\bin\HappyWorld\Release\Resources" mkdir "%BUILD_DIR%\bin\HappyWorld\Release\Resources"
xcopy "%ROOT_DIR%Resources\*" "%BUILD_DIR%\bin\HappyWorld\Release\Resources\" /E /Y /Q >nul 2>&1

REM 同步 Resources 到 install 目录
if not exist "%INSTALL_DIR%\Release\Resources" mkdir "%INSTALL_DIR%\Release\Resources"
xcopy "%ROOT_DIR%Resources\*" "%INSTALL_DIR%\Release\Resources\" /E /Y /Q >nul 2>&1

echo [OK]
echo.
echo ========================================
echo   部署完成！
echo   运行: %INSTALL_DIR%\HappyWorld.exe
echo ========================================
echo.
pause
