@echo OFF

rem build arch (x86 | x64) generator (2015|2017)

set msvs=%1
set arch=%2

set __CWD__=%CD%
pushd %~dp0\..
set __ROOT__=%CD%
popd


if not defined arch ( set arch=x64 )
if "[%arch%]" neq "[x86]" (
   if "[%arch%]" neq "[x64]" (
       set arch=x64
	   echo force arch to %arch%
   )
)

if not defined msvs set msvs=2015


REM -----------------------------
REM     build
REM -----------------------------

if not exist build mkdir build
cd build
set options=-G "Visual Studio

if %msvs% == 2015 set options=%options% 14 2015
if %msvs% == 2017 set options=%options% 15 2017
if %arch% == x64  set options=%options% Win64
set options=%options%"
cmake %options% ..
if %errorlevel% neq 0 goto _ERROR

set options=--config Release
if defined _DEBUG set options=--config Debug
cmake --build . --target ALL_BUILD %options%
if %errorlevel% neq 0 goto _ERROR

popd
cls
:success
echo.
echo.

set config=Release
if defined _DEBUG ( set config=Debug )
color 0A
echo         libwebstreamer build success
echo         Arch   : %arch%
echo         Build  : %config%
echo.
echo.
color

GOTO _END

:_ERROR
color 04
echo libwebstreamer build failed !
color
popd

:_END
cd %__CWD__%