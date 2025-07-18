# FerryFlow

A project designed to streamline the management of ferry operations.


# Configuration and Setup

The following instructions will help you set up and configure the project based on your development environment.

### CMake

You can download CMake installers from the [official CMake download page](https://cmake.org/download/). Please ensure that you meet the following minimum version requirement:

```diff
CMake version 3.22.1
```

### Environment and Setup

#### Terminal

To manually configure and build a CMake project using the terminal, first create a build directory in the root of the project. Then, open your platform's terminal, navigate to the build directory, and run the following commands each time you want to build your project:

```diff
cmake ..
cmake --build . --parallel
```

If the build completes successfully, all binary outputs (including libraries, executables, and test cases) will be generated in the build folder.

#### Using an IDE 

We’ve included instructions for some of the most commonly used IDEs to make the setup process a bit easier.

##### Visual Studio (Windows)

If you're using Visual Studio as your IDE, you can refer to the [official Microsoft documentation](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170) for guidance on opening and managing a CMake project.

**Here is a brief summary of the steps:**

1. Download and run the Visual Studio Installer.
2. Install your preferred version of Visual Studio, making sure to include the **"Desktop development with C++"** module.
3. Once installation is complete, open Visual Studio and select the folder containing the root `CMakeLists.txt` file.
4. Build, configure, and run the project.

##### Visual Studio Code (Cross-platform)

If you're using Visual Studio Code as your IDE, you can refer to the [official Visual Studio Code documentation](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/README.md) for guidance on opening and managing a CMake project.

**Here is a brief summary of the steps:**

1. Download and run the Visual Studio Code Installer.
2. Install Visual Studio Code, making sure to add the **"CMake Tools"** extension after installation is completed.
3. Open Visual Studio Code and select the folder containing the root `CMakeLists.txt` file.
4. Build, configure, and run the project.

# Tutorials and documentations

If you want to learn more about writing unit tests, then visit the official [Catch2 library documentation page](https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md#top).

# License

This project is licensed under the [MIT License](LICENSE).
