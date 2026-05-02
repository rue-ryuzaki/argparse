FROM ubuntu:26.04

ARG GCC_MAJOR=16

RUN <<INSTALL_GCC
  apt-get update
  apt-get install -y curl cmake gcc-${GCC_MAJOR} g++-${GCC_MAJOR}
  update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-${GCC_MAJOR} 100
  update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-${GCC_MAJOR} 100
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_MAJOR} 100
  update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_MAJOR} 100
INSTALL_GCC
