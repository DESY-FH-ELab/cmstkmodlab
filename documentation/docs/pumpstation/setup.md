# Setup Instructions for Pump Station Software on Raspbian Linux (Jessie)

## Requirements

You should have a running Raspbian Linux installation based on the
[build instructions](../build/RaspbianLinux.md) to start with.
In addition, please install the following packages via

   `sudo apt-get install apache2`<br/>
   `sudo apt-get install libapache2-mod-webauth`<br/>
   `sudo apt-get install libapache2-mod-php5`<br/>

## Installation

Go to the directory to which you checked out the source code and run

   `./configure` [options]

with options as described in the [documentation](./configure).

Then run

   `source tkmodlabenv.sh`
   
in case you use `bash` or add the script to your `.bashrc`. You should
now be good to go and build the software by running

   `make`
