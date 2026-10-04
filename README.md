# Title Case Converter

## Linux and MacOS

This is a wxWidgets desktop application that converts text to APA-7 title case.

Install the wxWidgets 3.2 development package for your platform. On Debian or Ubuntu:

```bash
sudo apt install libwxgtk3.2-dev
```

Build with CMake:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd build
make -j$(nproc)
```

## Windows

Install the header files, the right precompiled developer libraries and the release DLLs (for the right compiler) from here: https://wxwidgets.org/downloads/

If you installed them under C:\Libraries\wxwidgets-3.3.3 then use this command in the Powershell:

```pwsh
Remove-Item -Recurse -Force build

cmake -S . -B build `
  -DwxWidgets_ROOT_DIR="C:/Libraries/wxWidgets-3.3.3" `
  -DwxWidgets_LIB_DIR="C:/Libraries/wxWidgets-3.3.3/lib/vc14x_x64_dll" `
  -DwxWidgets_CONFIGURATION=mswu `
  -DCMAKE_WIN32_EXECUTABLE=ON `
  -G "Visual Studio 17 2022" `
  -A x64
```

And then:

```pwsh
cmake --build build --config Release
```
Assuming that you installed the entire directory `wxMSW-3.3.3_vc14x_x64_ReleaseDLL` that contains the release DLLs into `C:\Libraries\wxwidgets-3.3.3\`, you might need to add this directory to the PATH:

```pwsh
$env:Path += [IO.Path]::PathSeparator + "C:\Libraries\wxwidgets-3.3.3\wxMSW-3.3.3_vc14x_x64_ReleaseDLL\lib\vc14x_x64_dll"
```