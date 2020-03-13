# Setup Instructions for Pump Station Software

The software is supposed to be running on a raspberry pi with Raspbian (Jessie) as operating system.

## Requirements

You should have a running Raspbian Linux installation based on the
[build instructions](../build/RaspbianLinux.md) to start with.
In addition, please install the following packages via

   `sudo apt-get install apache2`<br/>
   `sudo apt-get install libapache2-mod-webauth`<br/>
   `sudo apt-get install libapache2-mod-php5`<br/>
   `sudo apt-get install udev`</br>
   `sudo apt-get install davfs2`</br>
   
## Installation

Once building the software has successfully finished, install all required libraries and applications via

   `sudo make install`

This will also install scripts to automatically start the pump station daemon on system startup.
After a reboot of the system, the daemon will be running as the default raspberry pi user.

The deamon communicating with the devices in the background requires the devices (`/dev/ttyUSB*`) to
be correctly found. The Leybold GraphixThree controller is supposed to accessable via `/dev/ttyLeybold`,
and the Conrad relay card via `/dev/ttyConrad`. A udev rules file `99-usb-serial.rules` is
available in the `pumpstation` directory of the repository. Please copy it to the directory
`/etc/udev/rules.d/` and restart the system.

In order to automatically synchronise the data to the DESY cloud, perform the following steps:

- create a directory to be used as a mount point for the DESY cloud via</br>
   `sudo mkdir /media/desyCloud`</br>

- add the following line to `/etc/fstab`</br>
   `https://desycloud.desy.de/remote.php/webdav /media/desyCloud davfs noauto,user 0 0`</br>

- add the user `pi` to the group `davfs2` via</br>
   `sudo usermod -a -G davfs2 pi`</br>

- add the DESY cloud credentials to `davfs2` by adding the following line to the file `~/.davfs2/secrets`</br>
   `https://desycloud.desy.de/remote.php/webdav USER PASSWORD`</br>
   
- add the following entry to the crontab for the user `pi`</br>
   `0 7,19 * * * /home/pi/cmstkmodlab/pumpstation/cloudSync.sh > /dev/null 2>&1`
