# ![pwf2tools logo](https://github.com/pahaze/pwf2tools-cpp/blob/master/img/pwf2tools-x64.png?raw=true) pwf2tools - C++ port

 pwf2tools is a suite of modding tools for the game "Parappa the Rapper 2". It's free to use and open source (and always will be!). It can be used for INT, SPM, TM2, and TEX0 modification. This version is not complete, so it should not be used yet, as I'm still learning C++ and Qt! Continue to use pwf2tools-cs, please. This version is built in C++ (Qt), and is targetted for every OS, including Linux and macOS. If you want to test, feel free to download and build. You have to have zlib and libpng development files. 

## Currently finished -

  * pwf2int
  * pwf2spm

## Building

Build as you normally would a CMake application. Qt5 or higher is needed, along with libpng/zlib. The good ol' `mkdir build && cd build && cmake ..` will work, and output will be put in build/../out (or rootdir/out).

## CMake Arguments

Don't want the GUI? Just want the executables? Sure! Supply `-DBUILD_QTGUI=NO` to CMake.

## Supported files
  * Archives
    - `INT archives`
  * Textures
    - `TM2 textures`
    - `TEX0 textures (using an SPM)`
  * Models
    - `SPM models`

## pwf2int

pwf2int deals with the extraction and creation of `INT` archives. These are what stages are held in for Parappa 2. A more detailed explanation will be in a documentation file.

## pwf2spm

pwf2spm deals with the extraction of `tex0` files from `SPM` files. These are used to grab textures from `TM0` files. `TM0` files cannot be directly edited as they're swizzled and still being looked into. They have 4 byte swizzles with a 32 bit buffer.

## pwf2tex

pwf2tex deals with the extraction and injection of textures using `tex0` files to find them in `TM0` files. The textures are indexed RGB (with a CLUT). Creating your own is simple, just index to 16 colors in GIMP (or your application of choice), then change back to RGB and save/overwrite the PNG.

## pwf2hat

pwf2hat deals with the editing of `TM2` textures. These are used by Parappa 2 to store hats. There are 0 color limitations on them, so don't worry about indexing. (Will not be in C++ port for a while).