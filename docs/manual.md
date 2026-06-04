# Manual Installation for Wyrm

### Compilers and CMake
wyrm supports both the [GCC](https://gcc.gnu.org/) and [Clang](https://clang.llvm.org/) compiler tool chains and targets the [C++ 17](https://en.cppreference.com/cpp/17) standard. wyrm also uses [CMake](https://cmake.org/) as its build system.

**Ubuntu/Debian (GCC)**
```shell
sudo apt update
sudo apt install build-essential make cmake tar curl zip unzip git
```

**Fedora (GCC)**
```shell
sudo dnf update
sudo dnf install gcc gcc-c++ make cmake tar curl zip unzip git
```

### NatNet SDK
[NatNet](https://optitrack.com/software/natnet-sdk) is a Software library developed by OptiTrack, and is used to create client applications for OptiTrack camera systems.

1. Install the [NatNet SDK](https://optitrack.com/software/natnet-sdk) to your ~/Downloads folder.
2. Install the NatNetSDK library and headers locally:
    ```shell
    mkdir -p /tmp/NatNetSDK
    tar -xf ~/Downloads/tar_archive_name.tar -C /tmp/NatNetSDK
    mkdir -p ~/.local/include/NatNet
    cp /tmp/NatNetSDK/include/* ~/.local/include/NatNet/
    cp /tmp/NatNetSDK/lib/libNatNet.so ~/.local/lib/
    rm -rf /tmp/NatNetSDK
    ```

### Zenoh
[Zenoh](https://zenoh.io/) is a communication protocal, and software stack that wyrm uses to publish data from the NatNet client.
1. Install the [Rust toolchain](https://rust-lang.org/tools/install/)
2. Install [Zenoh-c](https://github.com/eclipse-zenoh/zenoh-c):
    ```shell
    git clone https://github.com/eclipse-zenoh/zenoh-c.git /tmp/zenoh-c
    cd /tmp/zenoh-c && mkdir build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=~/.local
    cmake --build .
    cmake --install .
    cd / && rm -rf /tmp/zenoh-c
    ```
3. Install [Zenoh-cpp](https://github.com/eclipse-zenoh/zenoh-cpp):
    ```shell
    git clone https://github.com/eclipse-zenoh/zenoh-cpp.git /tmp/zenoh-cpp
    cd /tmp/zenoh-cpp && mkdir build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=~/.local -DCMAKE_PREFIX_PATH=~/.local
    cmake --build .
    cmake --install .
    cd / && rm -rf /tmp/zenoh-cpp
    ```

### Fast-CDR
[Fast-CDR](https://github.com/eProsima/Fast-CDR) is a data serialization library for C++.
```shell
git clone https://github.com/eProsima/Fast-CDR.git /tmp/Fast-CDR
cd /tmp/Fast-CDR && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build . --target install
cd / && rm -rf /tmp/Fast-CDR
```

### Environment
Add a CMake path to the newly installed libraries:
```shell
echo 'export CMAKE_PREFIX_PATH=$HOME/.local:$CMAKE_PREFIX_PATH' >> ~/.bashrc
source ~/.bashrc
```