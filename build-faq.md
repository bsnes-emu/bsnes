# Attempting to build the Cocoa frontend fails with NSInternalInconsistencyException

When building on macOS, the build system will make a native Cocoa app by default. In this case, the build system uses the Xcode `ibtool` command to build user interface files. If this command fails, you can fix this issue by starting Xcode and letting it install components. After this is done, you should be able to close Xcode and build successfully.

# Attempting to build the SDL frontend on macOS fails on linking

SameBoy on macOS expects you to have SDL2 installed via Brew, and not as a framework. Older versions expected it to be installed as a framework, but this is no longer the case.

# Windows build process

For the various tools and libraries, follow the below guide to ensure easy, proper configuration for the build environment:

#### clang

This may be installed via a Visual Studio installer packages instead of built from source.

#### SDL Port

[libsdl2](https://libsdl.org/download-2.0.php) has two separate files that must be downloaded
  1. The `-x86` Runtime Binary (e.g., `SDL2-2.0.12-win32-x86.zip` (as of writing))
  2. The Visual C++ Development Library (e.g., `SDL2-devel-2.0.12-VC.zip` (as of writing))

For the Runtime Binary, place the extracted `SDL2.dll` into a known folder for later.

- `C:\SDL2\bin\SDL2.dll` will be used as an example

For the Visual C++ Development Library, place the extracted files within a known folder for later.

The following examples will be referenced later:

- `C:\SDL2\lib\x86\*`
- `C:\SDL2\include\*`

#### Gnuwin

Ensure that this is in %PATH%.

If errors arise (i.e., particularly with the `CREATE_PROCESS('usr/bin/mkdir')` calls, also verify that Git for Windows has not been installed with full Linux support. If it has, remove `C:\Program Files\Git\usr\bin` from the SYSTEM %PATH% until after compilation.

### Building

Within a command prompt in the project directory:

```
vcvars32
set path=%path%;C:\SDL2\bin
set lib=%lib%;C:\SDL2\lib\x86
set include=%include%;C:\SDL2\include
make
```
Please note that these directories (`C:\SDL2\*`) are the examples given within the "SDL Port" section above. Ensure that your `path`, `lib`, and `include` paths are updated appropriately with the SDL2 downloads.

#### Error -1073741819

If encountering an error that appears as follows:

> make: *** [build/bin/BootROMs/dmg_boot.bin] Error -1073741819

Simply run `make` again, and the process will continue. This appears to happen occasionally with `build/bin/BootROMs/dmg_boot.bin` and `build/bin/BootROMs/sgb2_boot.bin`. It does not affect the compiled output.


