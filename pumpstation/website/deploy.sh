#!/bin/bash

SOURCEPATH="`dirname \"$0\"`"
DEPLOYMENTPATH=$PWD

echo $SOURCEPATH
echo $DEPLOYMENTPATH

cp -f $SOURCEPATH/index.php $DEPLOYMENTPATH
cp -f $SOURCEPATH/ConradSwitch.php $DEPLOYMENTPATH
cp -f $SOURCEPATH/script.js $DEPLOYMENTPATH
cp -rf $SOURCEPATH/data $DEPLOYMENTPATH
cp -f $SOURCEPATH/../controller/PumpStationControl $DEPLOYMENTPATH
cp -f $SOURCEPATH/../pumpstation.cfg $DEPLOYMENTPATH
