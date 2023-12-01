# CMake VCPKG C++ Template

Basic example CMake + VCPKG C++ template to easily build and debug a project easily with simple 3rd party library package management.

# Instructions

#### Requirements

1.  Cmake: https://cmake.org/download/
1.  Ninja: https://github.com/ninja-build/ninja/releases
    1.  A C++ compiler
    1.  MSVC for windows
    1.  Either gcc/g++ or clang/clang++ on linux

#### VS Code (Linux and Windows)

1. Install VSCode and the C++ & CMake extensions.
1. With the CMake extension installed, you should see a label/button at the bottom left of the window (in the VSCode status bar). It should contain the name of one of the labels defined in CMakePresets.json ("GCC Debug". "MSVC Release", etc..) ".

   - If you don't see that label, hit `Ctrl + Shift + P`, then search for "CMake: Configure" or "CMake: Delete Cache and Reconfigure". The first time you're prompted to run this you will be asked to choose one of the configuration presets (from CMakePresets.json). On windows you should see 3 build options for MSVC (Debug, Release, and DebWithRelInfo). On linux you should have those same 3 choices for GCC and Clang.

#### Visual Studio

- Install the C++ development packages along with the CMake extension from the VS installer. If you already have VS installed, you can add packages to it using a program called "Visual Studio Installer". It should already be installed on your machine if Visual studio is installed.
- Open this repo directory **_as a folder_** from Visual Studio. If the cmake extension is installed you should see the options to configure the project under the "Project" file menu. Select configure first, then you can build normally (Ctrl + B or F7 depending on your shortcuts).

#### Launching / Debugging

Config files are in place for both VS (.vs/) and VSCode (.vscode/) that set up debugging for both applications. In VS you should see a debug target named "myprogram". Just select it if it isn't already selected, then you can launch the debugger.

By default the program will be built to:

    ./out/build/{compiler}-{build type}/myprogram[.exe]

You can modify the config files `.vscode/launch.json` or `.vs/launch.vs.json` to alter the command that's run when the debugger is invoked or to add arguments to the command (as well as a handful of additional options)
