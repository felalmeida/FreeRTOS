# /usr/bin/docker build --no-cache . -t felalmeida/arm-build
# cd ..;/usr/bin/docker run -d -it --name arm-build --mount type=bind,source=${PWD},target=/home/dev felalmeida/arm-build:latest
# /usr/bin/docker exec -it arm-build /bin/bash

FROM ubuntu:22.04

LABEL name="Ubuntu For ARM Builds" \
    vendor="Ubuntu" \
    license="GPLv2" \
    build-date="20241217" \
    maintainer="Felipe Almeida <fel.almeida@gmail.com>" \
    version="1.0"

ARG ARG_PICO_SDK_PATH=/usr/local/pico-sdk
ARG ARG_EXAMPLES_PATH=/usr/local/pico-examples
ARG ARG_PROJECT_GEN_PATH=/usr/local/pico-project-generator
ARG ARG_FREERTOS_PATH=/usr/local/freertos
ARG ARG_FREERTOS_KERNEL_PATH=${ARG_FREERTOS_PATH}/FreeRTOS/Source
ARG ARG_PICOTOOL_PATH=/bin

ENV PICO_SDK_PATH=${ARG_PICO_SDK_PATH}
ENV PICOTOOL_PATH=${ARG_PICOTOOL_PATH}

RUN echo "alias ll='clear;ls -ltrh;pwd;date'" >> /root/.bashrc \
    && echo "export PICO_SDK_PATH="${ARG_PICO_SDK_PATH} >> /root/.bashrc \
    && echo "export PICO_PROJECT_GEN_PATH="${ARG_PROJECT_GEN_PATH} >> /root/.bashrc \
    && echo "export FREERTOS_PATH="${ARG_FREERTOS_PATH} >> /root/.bashrc \
    && echo "export FREERTOS_KERNEL_PATH="${ARG_FREERTOS_KERNEL_PATH} >> /root/.bashrc \
    && echo "export PICOTOOL_OVERRIDE_DIR="${ARG_PICOTOOL_PATH} >> /root/.bashrc \
    && echo "export PATH=\${PATH}:.:\${PICO_SDK_PATH}:\${PICO_PROJECT_GEN_PATH}:\${FREERTOS_KERNEL_PATH}:\${PICOTOOL_OVERRIDE_DIR}" >> /root/.bashrc \
    && ln -fs /usr/share/zoneinfo/America/Sao_Paulo /etc/localtime

RUN apt-get clean \
    && apt-get update -y \
    && apt-get upgrade -y \
    && apt-get install -y software-properties-common cron bc jq net-tools vim locales tzdata wget screen git \
    && apt-get install -y g++ g++-arm-linux-gnueabi cmake gcc-arm-none-eabi build-essential pkg-config minicom gdb-multiarch automake autoconf texinfo libtool libftdi-dev libusb-1.0-0-dev

RUN apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Raspberry Pi Pico SDK
RUN git clone https://github.com/raspberrypi/pico-sdk ${ARG_PICO_SDK_PATH} && \
    cd ${ARG_PICO_SDK_PATH} && \
    git submodule update --init --recursive

# Raspberry Pi Pico Examples
RUN git clone https://github.com/raspberrypi/pico-examples.git ${ARG_EXAMPLES_PATH} && \
    cd ${ARG_EXAMPLES_PATH} && \
    git submodule update --init --recursive

# Raspberry Pi Pico Project Generator
RUN git clone https://github.com/raspberrypi/pico-project-generator.git ${ARG_PROJECT_GEN_PATH} && \
    cd ${ARG_PROJECT_GEN_PATH} && \
    git submodule update --init --recursive

COPY ./Findpicotool.cmake /usr/local/pico-sdk/tools/Findpicotool.cmake

# FreeRTOS (RUN git clone https://github.com/FreeRTOS/FreeRTOS-Kernel ${ARG_FREERTOS_KERNEL_PATH} && \ )
RUN git clone https://github.com/FreeRTOS/FreeRTOS.git ${ARG_FREERTOS_PATH} && \
    cd ${ARG_FREERTOS_PATH} && \
    git submodule update --init --recursive

# Picotool Installation
RUN git clone https://github.com/raspberrypi/picotool.git /home/picotool && \
    cd /home/picotool && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 && \
    cp /home/picotool/build/picotool ${ARG_PICOTOOL_PATH}/picotool && \
    rm -rf /home/picotool
