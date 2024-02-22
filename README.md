# Building cmstkmodlab

Note: This recipe was tested for the Automated Assembly software on Ubuntu 20.04 LTS and 22.04 LTS and is not valid for older Ubuntu releases, other Linux distributions or other operating systems. Other parts of the framework might require additional packages or drivers.

## Prerequesites

The partially recommended, partially required prerequisites include the packages installed via the following command:

```
sudo apt-get install git cmake cmake-curses-gui build-essential qt5-qmake qtchooser qtbase5-dev-tools qtscript5-dev libqt5svg5-dev libqt5charts5-dev libqwtplot3d-qt5-dev libgphoto2-dev libexiv2-dev pkg-config libboost-dev arduino arduino-mk libcurl4-openssl-dev python3 libboost-python-dev libopencv-dev gfortran libomp5
```

### ROOT

The framework, except for the assembly part, requires an installation of ROOT. For completeness, the following will provide the basic commands for the installation. Please refer to the [ROOT web page](https://root.cern/install/build_from_source/) for more information. Please note that ROOT requires to be built with the option `CXX_STANDARD=17` for c++17 support, see below.

Note: compatibility was tested for ROOT versions 6.26 and 6.28.

There are additional packages that are required for an installation of ROOT, they can be installed via

```bash
sudo apt-get install libxpm-dev libxft-dev
```

Then, create a directory for the installation and clone the latest version from the repository:

```bash
mkdir ROOT
cd ROOT
mkdir root_src root_build root_install
git clone --branch latest-stable --depth=1 https://github.com/root-project/root.git root_src
```

Configure and build the software with the required and recommended options (`CXX_STANDARD=17` is required, switching off `xrootd` recommended):
```bash
cd root_build
cmake -DCMAKE_INSTALL_PREFIX=../root_install/ -DCMAKE_CXX_STANDARD=17 -Dxrootd=OFF ../root_src/
cmake --build . --target install -- -j8
```

To source ROOT by default at the startup of a new session, add a line to your bash profile via

```bash
echo "source /path/to/root/root_install/bin/thisroot.sh" >> ~/.bashrc
```

### UEye

The UEye driver supported here are versions 4.95 and 4.96. The currently installed version can be identified via the command `ueyedemo --version`. The software can be downloaded from the vendor.


## cmstkmodlab

Clone the repository to get the source code:

```bash
git clone https://github.com/DESY-FH-ELab/cmstkmodlab.git
```

Navigate into the directory and create a build directory.

```bash
cd cmstkmodlab
mkdir build && cd build
```

Call `cmake` to configure the software:
```bash
cmake ..
```

By default, the framework is configured with a minimal setup. Individual options and parts can be configured via `cmake`. Individual options can be switched on and off either interactively via the command `ccmake ..` or via the command line. E.g. to configure the framework for an installation of the PS automated assembly software, use

```bash
cmake -DCMSTKMODLAB_ASSEMBLY=ON ..
```

Install the software via

```bash
make install
```

All binaries will be installed into the directory `cmstkmodlab/bin`.