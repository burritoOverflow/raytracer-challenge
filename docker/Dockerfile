FROM ubuntu:jammy

ADD . /raytracer-challenge

RUN \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y zsh neovim vim git wget && \
    apt-get install -y build-essential libssl-dev clang-15

# install a newer version of cmake
RUN wget -v https://github.com/Kitware/CMake/releases/download/v3.27.8/cmake-3.27.8.tar.gz
RUN tar -xzvf cmake-3.27.8.tar.gz
RUN cd cmake-3.27.8/ && ./bootstrap && make -j $(nproc) VERBOSE=1 && make install

# build + install ninja
RUN git clone https://github.com/ninja-build/ninja.git 
RUN cd ninja && git checkout release && cmake -S . -B build && make -C build/ -j $(nproc) VERBOSE=1 && make -C build/ install

WORKDIR /raytracer-challenge

RUN ls -la
RUN cmake -S . -B build_docker -G Ninja -D BUILD_SHARED_LIBS=ON -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_COMPILER=clang++-15  -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
RUN cmake --build build_docker/ --verbose
RUN ctest --test-dir build_docker/ -VV
