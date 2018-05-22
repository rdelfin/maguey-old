# Maguey

![Build Status](https://travis-ci.org/rdelfin/maguey.svg?branch=master)

A game engine written in C++

## Installation

You can install the package using the ppa:

```bash
sudo add-apt-repository ppa:rdelfin/ppa
sudo apt-get update
sudo apt-get install maguey
```

## Build

If you'd rather build this from scratch, here are the instructions.

First up, install all the necessary dependencies:

```bash
sudo apt-get install libglfw3-dev libsoil-dev libglew-dev libglm-dev libgtest-dev pkgconfig
```

Then clone the repo into a folder called `maguey` and run:

```bash
cd maguey
mkdir build
cd build
cmake ..
make
```

Alternatively, if you want to also build the tests, run:

```bash
cd maguey
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
make
```

Do not build the tests if you intend to install this package, as it will attempt to install the Google Test framework.

Afterwards, you can create a `.deb` package file by running:

```bash
cpack -G DEB
``` 

This will generate a `maguey-${VERSION}-Linux.deb` file (where `${VERSION}` is the current version number). You can then install the package using:

```bash
sudo dpkg -i maguey-${VERSION}-Linux.deb
```

Alternatively you can use `sudo make install` to install the built files with the same effect.

## Adding to CMake

Once you've installed this package, you can add this to your `CMakeLists.txt` by adding:

```cmake
find_package(maguey REQUIRED)

...

include_directories(${MAGUEY_INCLUDE_DIRS}
                    # Other include directories
                    )

target_link_libraries(${YOUR_TARGET}
                      ${MAGUEY_LIBRARIES}
                      ${stdgl_libraries}
                      SOIL)
```

Where `${YOUR_TARGET}` is the name of the CMake target you're linking maguey to.

## Deployment

If you have the abillity to deploy this package to the ppa, remember to first increment the version (as described below). If this is part of a new minor or major version, create a branch with the appropriate name (e.g. `1.1` or `2.0`). Otherwise, move the appropriate version branch forward to master. Afterwards, run the following commands from this repo:

```bash
debuild -S -sd
cd ..
dput ppa:rdelfin/ppa maguey_${VERSION}_source.changes
```

All credit for this process goes to Simon Schneegans [for this article](http://simmesimme.github.io/lessons/2011/11/02/ppa-launchpad-cmake).

### On Version Increment

When incrementing the version, there are three files that need to change:

- `CMakeLists.txt`: Increment version.
- `debian/changelog`: Increment version and add changes
