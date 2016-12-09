# Setup Instructions for Pump Station Software

The software is supposed to be running on a raspberry pi with Raspbian (Jessie) as operating system.

## Requirements

You should have a running Raspbian Linux installation based on the
[build instructions](../build/RaspbianLinux.md) to start with.
In addition, please install the following packages via

   `sudo apt-get install apache2`<br/>
   `sudo apt-get install libapache2-mod-webauth`<br/>
   `sudo apt-get install libapache2-mod-php5`<br/>

## Installation

Once building the software has successfully finished, install all required libraries and applications via

   `sudo make install`

This will also install scripts to automatically start the pump station daemon on system startup.
After a reboot of the system, the daemon will be running as the default raspberry pi user.