@echo off

set "OCCT_7_5_0=E:\OpenCASCADE-7.5.0-vc14-64"

if ["%OCCT_7_5_0%"] == [""] (
set /p OCCT_7_5_0 =请输入OpenCasCade7.5.0安装路径：
)
call "%OCCT_7_5_0%\opencascade-7.5.0\env.bat" %1 %2 %3

set "BIN_DIR=win%ARCH%\%VCVER%\bind"
set "LIB_DIR=win%ARCH%\%VCVER%\libd"

if ["%CASDEB%"] == [""] (
  set "BIN_DIR=win%ARCH%\%VCVER%\bin"
  set "LIB_DIR=win%ARCH%\%VCVER%\lib"
)

set "PATH=%~dp0%LIB_DIR%;%~dp0%BIN_DIR%;%PATH%"