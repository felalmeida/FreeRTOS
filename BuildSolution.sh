#!/usr/bin/bash

clear
BasePath='/home/dev/FreeRTOS'

MountOverlay=$(findmnt / | grep -i docker | wc -l)
if [ ${MountOverlay} -gt 0 ]; then
    cd ${BasePath}

    # Housekeeping
    rm -rf ${BasePath}/build > /dev/null 2>&1
    mkdir ${BasePath}/build
    rm /home/dev/FreeRTOS/FreeRTOS_RP2040.uf2 > /dev/null 2>&1

    # Build
    cd ${BasePath}/build
    cmake ../
    make
    cd ${BasePath}
    cp ./build/FreeRTOS_RP2040.uf2 .

    # Housekeeping
    rm -rf ${BasePath}/build > /dev/null 2>&1
    clear
    ls -ltrh
    pwd
    date
else
    echo "Outside Docker"
    exit 1
fi
