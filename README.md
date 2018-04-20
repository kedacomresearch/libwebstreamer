# libwebstreamer

Coding Rule see [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
# How to build

tools you have to be installed first.
* CMake
* Python (cpplint rquired)
* [pkg-config](https://github.com/kedacomresearch/libwebstreamer/releases/download/0.0.0/pkg-config.zip)
* Visual Studio 2015/2017
* GStreamer >=1.14.0 complete install

set the PKG_CONFIG_PATH=To where gstreamer lib pkg config (C:\gstreamer\1.0\x86_64\lib\pkgconfig)

add pkg-config.exe path to your enviroment PATH

first check code rule
run tools/cpplint.bat
if no error then build.

run tools/build.bat

You can  use cam to simplify these

install python then do as below
```bash
pip install cam
cd libwebstreamer
cam build
```