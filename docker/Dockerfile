FROM ubuntu:jammy

ADD . /raytracer-challenge

RUN \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y zsh neovim vim git wget && \
    apt-get install -y build-essential ninja-build libssl-dev

# install a newer version of cmake
RUN wget -v https://github.com/Kitware/CMake/releases/download/v3.27.8/cmake-3.27.8.tar.gz
RUN tar -xzvf cmake-3.27.8.tar.gz
RUN cd cmake-3.27.8/ && ./bootstrap && make -j $(nproc) && make install

WORKDIR /raytracer-challenge

RUN ls -la
RUN cmake -S . -B build_docker -G Ninja
RUN cmake --build build_docker/ --verbose
RUN ctest --test-dir build_docker/ -VV
