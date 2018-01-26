if not exist %~dp0\build mkdir %~dp0\build
pushd %~dp0\build
set PKG_CONFIG=C:\cerbero\build-tools\bin\pkg-config

set PKG_CONFIG_LIBDIR=C:\cerbero\x86_64\lib\pkgconfig
%PKG_CONFIG% --version
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX="C:/cerbero/x86_64" ..
cmake --build . --target ALL_BUILD --config Debug -- /p:Platform=x64
cmake --build . --target INSTALL  --config Debug -- /p:Platform=x64
popd 