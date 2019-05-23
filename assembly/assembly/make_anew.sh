#!/bin/bash

if [ ! -z ${TKMODLABBASE} ] && [ -d ${TKMODLABBASE} ]; then

  cd ${TKMODLABBASE}

  make clean
  ./reconfigure
  make all

  cd ${OLDPWD}
fi
