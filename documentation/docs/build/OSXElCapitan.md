# Build Instructions for OSX El Capitan

## Requirements

Building the software package on OSX El Capitan requires XCode,
the XCode Command Line Tools and homebrew to be installed. XCode
is available from the Apple App Store. The XCode Command Line
Tools can be installed via

   `sudo xcode-select --install`

Installation instruction for homebrew can be found on
[brew.sh](http://brew.sh). It can also be installed directly via

   `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

Once homebrew is installed, install the following packages

   * `brew install cmake`
   * `brew install gcc`
   * `brew install opencv`
   * `brew install gphoto2`
   * `brew install pkg-config`
   * `brew install exiv2`
   * `brew install qt4`
   * `brew install boost`

## Building

Go to the directory to which you checked out the source code and run

   `./configure` [options]

with options as described in the [documentation](./configure).
Please note that in the case of OSX, the option `--fake` is
forced.

Then run

   `source tkmodlabenv.sh`
   
in case you use `bash` or add the script to your `.bashrc`. You should
now be good to go and build the software by running

   `make`

