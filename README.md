# Hello OpenGL

Basic C++ program demonstrating OpenGL usage. Repo is setup for Mac development on Visual Studio Code.

## Dependencies

Deps are installed with Homebrew as a quick and dirty way to download the proper header files and library files (dlls) for Mac. These files are then referenced in the Makefile from `/usr/local` (where Homebrew placed them).

Other approaches could include downloading built versions of the dependencies from their websites and copying manually into the project, or compiling from source.

### GLFW

[GLFW](https://www.glfw.org/) is a cross-platform library that abstracts over the creation of an application window and OpenGL context, amoungst other things.

```sh
brew install glfw
```

### GLEW

[GLEW](https://glew.sourceforge.net/) is a library that operates at runtime to expose/link the OpenGL functions/extensions supported by the current system's GPU drivers.

```sh
brew install glew
```

## Getting started

1. Install the above deps with `brew`
1. Consider installing the recommended VS Code extensions.
1. Create some empty folders for the compiler output,
   ```sh
   mkdir bin
   mkdir obj
   ```
1. To run the app in debug mode,
   - Run the `Make::Debug` task to compile the project
   - Hit `F5` to start debugging
