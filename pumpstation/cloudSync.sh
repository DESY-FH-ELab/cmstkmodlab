#!/bin/bash

mount /media/desyCloud

rsync -rtvuc /home/pi/cmstkmodlab/pumpstation/data/ /media/desyCloud/DAF/25c/PumpStation/

umount /media/desyCloud
