# Advent Of Code 2023
Santa's going to need lots of extra space for all those memory leaks this year. 

## How to Build

#### 1. Navigate into the source directory, create build folder and run `CMake`:

```sh
mkdir build
cd build
cmake [-G generator] ..
```

  * The `generator` option is the build system you'd like to use. Run `cmake` without arguments to see a full list of available generators.
    * On Windows, you might use "Visual Studio 12 2013" (VS 2013 32-bits), or "Visual Studio 14 2015 Win64" (VS 2015 64-bits).
    * On OS X, you might use "Xcode".
    * On a UNIX-like system, omit the option and a Makefile will be generated.

#### 2. Build it!
  * The command you'll need to run depends on the generator you chose earlier.
