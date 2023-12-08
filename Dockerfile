FROM debian:bookworm-slim
LABEL maintainer "mbaum0@pm.me"

RUN apt-get update \
    && apt-get upgrade \
    && apt-get install -y \
        sudo \
        build-essential \
        vim \
        curl \
        cmake \
        valgrind \
        libcppunit-dev \
        libsdl2-dev \
        libsdl2-image-dev \
        libsdl2-ttf-dev \
        libsdl2-mixer-dev \
        mesa-utils \
        libgl1-mesa-glx \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /valgrind