# CMake VCPKG C++ Template

Basic example CMake + VCPKG C++ template to easily build and debug a project easily with simple 3rd party library package management.

# Instructions

### Requirements

1.  Cmake: https://cmake.org/download/
1.  Ninja: https://github.com/ninja-build/ninja/releases
    1.  A C++ compiler
    1.  Either MSVC or clang toolchain for windows (mingw also works, but on windows msvc and clang just tend to work better overall).
    1.  Either gcc or clang toolchains on linux

### How to use this repo as a base for your project 

This repo is set up as a github template, so no need to star, fork, or clone. 
Use the "Use this template" button shown on the main github page for this repo, the select "create a new repository".
This will copy the full contents of this repo into a new repo in your github account. With this approach you'll get a complete copy of this repo with all of the history stripped out of it.

This repo includes a handful of branches that provide working examples using a variety of different 3rd party libraries. If you want these branches to be included in the repo that ends up being created in your github account, you need to check the "include all branches" option (in the "create a new repository" dialog). If you select that option you'll have all of the same branches included in the repo created in your account and you can run `git checkout <branch>` to use it.

### Adding / modifying dependencies included with vcpkg

1. Include the list of libraries you want vcpkg to download, build, configure, and install by editing the [vcpkg.json](vcpkg.json) file (in the "dependencies" section).
2. Run the cmake configuration to have vcpkg run through the process to download, build, configure, and install all dependencies for you. When this step is complete and succeeds, scroll up a bit in the cmake output in your IDE/terminal until you see a section where vcpkg outputs the changes that need to be made to [cmakelists.txt](cmakelists.txt). Most packages will require a `find_package(<package_name>)` line added as well as the 3rd party library linkage using something like `target_link_libraries(${PROJECT_NAME} PRIVATE <library_name/library_alias>)`
3. Update cmakelists.txt to include the changes vcpkg outputs described in step 2 above.
4. Re-run the cmake configuration, then when that finishes/succeeds, run the build. 

### VS Code (Linux and Windows)

1. Install VSCode and the C++ & CMake extensions.
1. With the CMake extension installed, you should see a label/button at the bottom left of the window (in the VSCode status bar). It should contain the name of one of the labels defined in CMakePresets.json ("GCC Debug". "MSVC Release", etc..) ".

   - If you don't see that label, hit `Ctrl + Shift + P`, then search for "CMake: Configure" or "CMake: Delete Cache and Reconfigure". The first time you're prompted to run this you will be asked to choose one of the configuration presets (from CMakePresets.json). On windows you should see 3 build options for MSVC (Debug, Release, and DebWithRelInfo). On linux you should have those same 3 choices for GCC and Clang.

### Visual Studio

- Install the C++ development packages along with the CMake extension from the VS installer. If you already have VS installed, you can add packages to it using a program called "Visual Studio Installer". It should already be installed on your machine if Visual studio is installed.
- Open this repo directory **_as a folder_** from Visual Studio. If the cmake extension is installed you should see the options to configure the project under the "Project" file menu. Select configure first, then you can build normally (Ctrl + B or F7 depending on your shortcuts).

### Launching / Debugging

Config files are in place for both VS (.vs/) and VSCode (.vscode/) that set up debugging for both applications. In VS you should see a debug target named "myprogram". Just select it if it isn't already selected, then you can launch the debugger.

By default the program will be built to:

    ./out/build/{compiler}-{build type}/myprogram[.exe]

You can modify the config files `.vscode/launch.json` or `.vs/launch.vs.json` to alter the command that's run when the debugger is invoked or to add arguments to the command (as well as a handful of additional options)
