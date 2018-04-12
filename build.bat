if not exist build mkdir build

pushd build

cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build . --target ALL_BUILD --config Debug

popd