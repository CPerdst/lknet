@ECHO OFF
chcp 65001
cls
@SETLOCAL
echo "==============lknet=============="
echo "默认全选一号选项"
echo.

@REM ---------------------
@REM 检测cmake是否可用
@REM 不可用则告知用户并退出
@REM ---------------------

where cmake >nul 2>nul

if %errorlevel% neq 0 (
    echo "Cmake未安装或者没有添加到环境变量当中，请尝试重新安装Cmake"
    pause
    exit /b
)

@REM ---------------------
@REM 检测lklog模块是否初始化
@REM ---------------------

if not exist "./lklog/CMakeLists.txt" (
    echo "更新lklog子模块"
    git submodule update --init --recursive lklog
    echo.
)

echo "BoostConfig.cmake绝对根路径:"
set /p user_preset_boost_dir=

if not exist %user_preset_boost_dir% (
    echo "输入路径存在问题，请重新输入"
    pause
    exit /b
)

@REM ---------------------
@REM 选择编译模式
@REM ---------------------

set option_compile=1

echo "编译模式:"
echo "1) Debug"
echo "2) Release"

set /p option_compile=

set Dcompile="Debug"

if %option_compile% == 1 (
    set Dcompile="Debug"
) else if %option_compile% == 2 (
    set Dcompile="Release"
) else (
    echo "请选择1或2"
    pause
    exit /b
)

@REM ---------------------
@REM 选择所支持的编译套件
@REM ---------------------

set dev_kit_opt=1

echo "选择所支持的编译套件"
echo "1) v143-x64"
echo "2) v143-32"
echo "3) x142-x64"
echo "4) x142-32"

set /p dev_kit_opt=

set BUILD_TOOLSET_="v143"
set BUILD_ARCH_="x64"

if %dev_kit_opt% == 1 (
    set BUILD_TOOLSET_="v143"
    set BUILD_ARCH_="x64"
) else if %dev_kit_opt% == 2 (
    set BUILD_TOOLSET_="v143"
    set BUILD_ARCH_="WIN32"
) else if %dev_kit_opt% == 3 (
    set BUILD_TOOLSET_="v142"
    set BUILD_ARCH_="x64"
) else if %dev_kit_opt% == 4 (
    set BUILD_TOOLSET_="v142"
    set BUILD_ARCH_="WIN32"
) else (
    echo "请选择1或2或3或4"
    pause
    exit /b
)

@REM ---------------------
@REM 是否编译使用样例与测试样例
@REM ---------------------

set example_test_compile=1

echo "是否编译使用样例与测试样例: "
echo "1) 不编译"
echo "2) 编译"

set /p example_test_compile=

set Dexample_test=False

if %example_test_compile% == 1 (
    set Dexample_test=False
) else if %example_test_compile% == 2 (
    set Dexample_test=True
) else (
    echo "请选择1或2"
    pause
    exit /b
)

mkdir build-%Dcompile%-%BUILD_TOOLSET_%-%BUILD_ARCH_%
pushd build-%Dcompile%-%BUILD_TOOLSET_%-%BUILD_ARCH_%

cmake   -T %BUILD_TOOLSET_% -A %BUILD_ARCH_% ^
        -DEXAMPLE_BUILD=%Dexample_test% -DTEST_BUILD=%Dexample_test% ^
        -DUSER_PRESET_BOOST_DIR=%user_preset_boost_dir% ^
        -S .. -B .
cmake --build . --config %Dcompile% -j 10
cmake --build . --config %Dcompile% --target all