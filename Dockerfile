# /usr/bin/docker build --no-cache . -t felalmeida/arm-build
# /usr/bin/docker run -d -it --name arm-build --mount type=bind,source=${PWD},target=/home/dev felalmeida/arm-build:latest
# /usr/bin/docker exec -it arm-build /bin/bash

FROM ubuntu:22.04

LABEL name="Ubuntu For ARM Builds" \
  vendor="Ubuntu" \
  license="GPLv2" \
  build-date="20240602" \
  maintainer="Felipe Almeida <fel.almeida@gmail.com>" \
  version="1.0"

RUN echo "alias ll='clear;ls -ltrh;pwd;date'" >> /root/.bashrc \
  && echo "export PICO_SDK_PATH=/usr/local/pico-sdk" >> /root/.bashrc \
  && echo "export PICO_PROJECT_GEN_PATH=/usr/local/pico-project-generator" >> /root/.bashrc \
  && echo "export FREERTOS_KERNEL_PATH=/usr/local/freertos" >> /root/.bashrc \
  && echo "export PATH=${PATH}:.:${PICO_SDK_PATH}:${PICO_PROJECT_GEN_PATH}:${FREERTOS_KERNEL_PATH}" >> /root/.bashrc \
  && ln -fs /usr/share/zoneinfo/America/Sao_Paulo /etc/localtime

RUN apt-get clean \
  && apt-get update -y \
  && apt-get upgrade -y \
  && apt-get install -y software-properties-common cron bc jq net-tools vim locales tzdata wget screen git \
  && apt-get install -y g++ g++-arm-linux-gnueabi cmake gcc-arm-none-eabi build-essential pkg-config minicom gdb-multiarch automake autoconf texinfo libtool libftdi-dev libusb-1.0-0-dev

RUN apt-get clean \
  && rm -rf /var/lib/apt/lists/*

ARG SDK_PATH=/usr/local/pico-sdk
ARG EXAMPLES_PATH=/usr/local/pico-examples
ARG PROJECT_GEN_PATH=/usr/local/pico-project-generator
ARG FREERTOS_KERNEL_PATH=/usr/local/freertos

# Raspberry Pi Pico SDK
RUN git clone https://github.com/raspberrypi/pico-sdk $SDK_PATH && \
    cd $SDK_PATH && \
    git submodule update --init

# Raspberry Pi Pico Examples
RUN git clone https://github.com/raspberrypi/pico-examples.git $EXAMPLES_PATH && \
    cd $EXAMPLES_PATH && \
    git submodule update --init

# Raspberry Pi Pico Project Generator
RUN git clone https://github.com/raspberrypi/pico-project-generator.git $PROJECT_GEN_PATH && \
    cd $PROJECT_GEN_PATH && \
    git submodule update --init

# FreeRTOS
RUN git clone https://github.com/FreeRTOS/FreeRTOS-Kernel $FREERTOS_KERNEL_PATH && \
    cd $FREERTOS_KERNEL_PATH && \
    git submodule update --init --recursive

# Picotool installation
ENV PICO_SDK_PATH=$SDK_PATH
RUN git clone https://github.com/raspberrypi/picotool.git /home/picotool && \
    cd /home/picotool && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 && \
    cp /home/picotool/build/picotool /bin/picotool && \
    rm -rf /home/picotool
