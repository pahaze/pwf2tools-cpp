# ![pwf2tools logo](https://github.com/pahaze/pwf2tools-cpp/blob/master/img/pwf2tools-x64.png?raw=true) pwf2tools - A suite of tools for modding PTR2!

[![CodeFactor](https://www.codefactor.io/repository/github/pahaze/pwf2tools-cpp/badge)](https://www.codefactor.io/repository/github/pahaze/pwf2tools-cpp)

 pwf2tools is a suite of modding tools for the game "Parappa the Rapper 2". It is released under the GPLv3 license. It's free to use and open source (and always will be!). It can be used for INT, SPM, TM2, and TEX0 modification. This version is not complete, so it isn't perfect. A public beta will be released soon. This version is built in C++ (Qt), and is targetted for every OS, including Linux and macOS. If you want to test, feel free to download and build. You have to have zlib and libpng development files. 

## Currently finished -

  * pwf2int
  * pwf2spm
  * pwf2tex

# Installing and using
You can build from source or you can use a pre-built executable file.

## From source
To build pwf2tools-cpp with the GUI and executables, you need the following dependencies on your system: **Qt5** (or newer), **CMake 3.5** (or newer), **libpng**, **zlib**, and a C/C++ compiler. At the root of the directory, you will need to run these commands:

    mkdir build
    cd build
    cmake ..
    make
    make install (not necessary but can be used if wanted)

Output will be in the root directory in a folder called "out". (pwf2tools-cpp/out)

For only the executables, dependencies are: **libpng**, **zlib**, **CMake 3.5** (or newer), and a C/C++ compiler.
For only the GUI, dependencies are: **Qt5** (or newer), **CMake 3.5** (or newer), and a C/C++ compiler.

## Pre-built
Pre-built executables can be found on the [Releases](https://github.com/pahaze/pwf2tools-cpp/releases) page. Pre-built Macintosh executables will most likely not happen as I do not own a Mac.


## CMake Arguments

Don't want the GUI? Just want the executables? Sure! Supply `-DBUILD_QTGUI=NO` to your CMake arguments. \
Not feeling like the executables? Just want the GUI? Sure! Supply `-DBUILD_EXECS=NO` to your CMake arguments.

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

pwf2hat deals with the editing of `TM2` textures. These are used by Parappa 2 to store hats. There are 0 color limitations on them, so don't worry about indexing. (Will not be in C++ port for a while, use the tool Rainbow to edit these for now :]).