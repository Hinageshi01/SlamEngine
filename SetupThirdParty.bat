@echo off

rem Set path
set "ROOT_PATH=%~dp0"
set "THIRD_PARTY_PATH=%ROOT_PATH%\Engine\Source\ThirdParty"
echo Root path: %ROOT_PATH%
echo ThirdParty path: %THIRD_PARTY_PATH%

rem Find MSBuild by vswhere
set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set "MSBUILD_PATH=%%i"
)
echo Found MSBuild at: %MSBUILD_PATH%

rem fmt
set "FMT_PATH=%THIRD_PARTY_PATH%\fmt"
echo [ fmt ] path: %FMT_PATH%
cd %FMT_PATH%

cmake -B build
:: cmake --build build --target fmt --config Release
:: cmake --build build --target fmt --config Debug

rem spdlog
set "SPDLOG_PATH=%THIRD_PARTY_PATH%\spdlog"
echo [ spdlog ] path: %SPDLOG_PATH%
cd %SPDLOG_PATH%

cmake -B build -DCMAKE_CONFIGURATION_TYPES="Release;Debug" -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_FMT_EXTERNAL_HO=ON -Dfmt_DIR="%FMT_PATH%\build" -DSPDLOG_NO_EXCEPTIONS=ON
cmake --build build --target spdlog --config Release
cmake --build build --target spdlog --config Debug

pause
