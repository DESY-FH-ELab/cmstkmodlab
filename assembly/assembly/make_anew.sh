#!/bin/bash

if [ -d ${TKMODLABBASE} ]; then

  cd ${TKMODLABBASE}

  make clean
  ./reconfigure
  make all

  cd ${OLDPWD}
fi
