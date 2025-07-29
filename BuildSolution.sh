#!/usr/bin/bash

clear

MountOverlay=$(findmnt / | grep -i docker | wc -l)
if [ ${MountOverlay} -gt 0 ]; then
    BasePath='/home/dev/FreeRTOS'

    cd ${BasePath}

    ### Housekeeping
    rm -rf ${BasePath}/build > /dev/null 2>&1
    mkdir ${BasePath}/build
    rm  ${BasePath}/FreeRTOS_RP2040.uf2 > /dev/null 2>&1

    ### Build
    cd ${BasePath}/build
    cmake ../
    make
    cd ${BasePath}
    cp ./build/FreeRTOS_RP2040.uf2 .

    ### Housekeeping
    rm -rf ${BasePath}/build > /dev/null 2>&1
    clear
    ls -ltrh
    pwd
    date

    exit 0
else
    echo "Outside Docker"
    exit 1
fi
