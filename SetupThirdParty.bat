@echo off

rem Set path
set "ROOT_PATH=%~dp0"
set "SCRIPT_PATH=%ROOT_PATH%\Script"
set "THIRD_PARTY_PATH=%ROOT_PATH%\Engine\Source\ThirdParty"
set "THIRD_PARTY_BUILD_PATH=%ROOT_PATH%\Engine\Source\ThirdParty\build"

echo Root path: %ROOT_PATH%
echo Script path: %SCRIPT_PATH%
echo ThirdParty path: %THIRD_PARTY_PATH%
echo ThirdParty build path: %THIRD_PARTY_BUILD_PATH%

rem Find MSBuild by vswhere
set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set "MSBUILD_PATH=%%i"
)
echo Found MSBuild at: %MSBUILD_PATH%
echo.

rem Manually organise thirdparty solution and projects, then compile them.
cd %SCRIPT_PATH%
"premake-bin\premake5.exe" --file=thirdparty.lua vs2022
"%MSBUILD_PATH%" "%THIRD_PARTY_BUILD_PATH%\SlamThirdparty.sln" -p:Configuration=Debug;Platform=x64
"%MSBUILD_PATH%" "%THIRD_PARTY_BUILD_PATH%\SlamThirdparty.sln" -p:Configuration=Release;Platform=x64

rem glfw
set "GLFW_PATH=%THIRD_PARTY_PATH%\glfw"
echo [ glfw ] path: %GLFW_PATH%
cd %GLFW_PATH%

cmake -B build -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DUSE_MSVC_RUNTIME_LIBRARY_DLL=OFF
cmake --build build --target glfw --config Debug
cmake --build build --target glfw --config Release
echo.

rem yaml-cpp
set "YAMLCPP_PATH=%THIRD_PARTY_PATH%\yaml-cpp"
echo [ yaml-cpp ] path: %YAMLCPP_PATH%
cd %YAMLCPP_PATH%

cmake -B build -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DYAML_MSVC_SHARED_RT=OFF
cmake --build build --target yaml-cpp --config Debug
cmake --build build --target yaml-cpp --config Release
echo.

rem shaderc
set "SHADERC_PATH=%THIRD_PARTY_PATH%\shaderc"
echo [ shaderc ] path: %SHADERC_PATH%
cd %SHADERC_PATH%

cmake -B build -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DSHADERC_SKIP_TESTS=ON
cmake --build build --target shaderc_combined --config Debug
cmake --build build --target shaderc_combined --config Release
echo.

rem spirv-cross
set "SPIRVCROSS_PATH=%THIRD_PARTY_PATH%\spirv-cross"
echo [ spirv-cross ] path: %SPIRVCROSS_PATH%
cd %SPIRVCROSS_PATH%

cmake -B build -DCMAKE_CONFIGURATION_TYPES="Debug;Release"
cmake --build build --config Debug
cmake --build build --config Release
echo.

pause
