# GemMiner game

* Thirdparty libraries (everything is built-in):
  * SDL 2.0.3
  * STB image
  * STB truetype

# Build instructions
## Windows
### MinGW

* tools:
  * MinGW 4.8.0 and above
  * CMake 2.8.12 and above
* cd build_folder
* cmake -G"Unix Makefiles" PATH_TO_GEM_MINER_DIR
* make

### Visual Studio

* tools:
  * tested with Visual Studio 2013 Prof/Express, should work with 2010 and above (Prof/Express)
  * CMake 2.8.12 and above
* cd build_folder
* cmake -G"Visual Studio 12 Win64" PATH_TO_GEM_MINER_DIR
  * for x86: cmake -G"Visual Studio 12" PATH_TO_GEM_MINER_DIR
* open GemMiner.sln at PATH_TO_GEM_MINER_DIR folder

## Mac OSX

* tools:
  * Xcode 6.1
  * CMake 2.8.12 and above
* cd build_folder
* cmake -G"Xcode" PATH_TO_GEM_MINER_DIR
* open GemMiner at PATH_TO_GEM_MINER_DIR folder

## iOS

* dependencies:
  * Xcode 6.1
* open ios/GemMiner/GemMiner.xcodeproj

## Android

* tools:
  * (from windows host)
  * apache-ant-1.9.2
  * android-ndk-r9
  * adt-bundle-windows-x86_64-20130917
* cd android-project
* ndk-build
* adb debug
* adb debug install
