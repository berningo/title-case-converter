# Title Case Converter

This is a wxWidgets desktop application that converts text to APA-7 title case.

Install the wxWidgets 3.2 development package for your platform. On Debian or Ubuntu:

```sh
sudo apt install libwxgtk3.2-dev
```

Build with CMake:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd build
make -j$(nproc)
```
