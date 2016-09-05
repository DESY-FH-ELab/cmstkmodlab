# Build Instructions for Ubuntu Linux (16.04)

## Requirements

The software relies on package version not available in a standard Ubuntu installation.
In order to get the latest versions of the installed packages edit the file
`sudo pico /etc/apt/sources.list` and add the line

   `deb http://archive.ubuntu.com/ubuntu/ xenial-proposed restricted main multiverse universe`

to its end. After that execute

   `sudo apt-get update`
   `sudo apt-get upgrade`

on the command line. You can now install the required packages via
   
   `sudo apt-get install git`<br/>
   `sudo apt-get install cmake`<br/>
   `sudo apt-get install root-system`<br/>
   `sudo apt-get install gcc`<br/>
   `sudo apt-get install locate`<br/>
   `sudo apt-get install libqt4-dev`<br/>
   `sudo apt-get install libqwt-dev`<br/>
   `sudo apt-get install libqwtplot3d-qt4-dev`<br/>
   `sudo apt-get install libgphoto2-dev`<br/>
   `sudo apt-get install libexiv2-dev`<br/>
   `sudo apt-get install libopencv-dev`<br/>
   `sudo apt-get install pkg-config`<br/>
   `sudo apt-get install libboost-dev`<br/>
   `sudo apt-get install arduino`<br/>
   `sudo apt-get install arduino-mk`

## Building

Go to the directory to which you checked out the source code and run

   `./configure` [options]

with options as described in the [documentation](./configure).

Then run

   `source tkmodlabenv.sh`
   
in case you use `bash` or add the script to your `.bashrc`. You should
now be good to go and build the software by running

   `make`
