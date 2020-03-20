UDPI
========================

## Introduction

The UDPI (Universal Deep Packet Inspection) project is a reference framework to build a high performance solution
for Deep Packet Inspection, integrated with the general purpose FD.io VPP stack.
It leverages industry regex matching library to provide a rich set of features,
which can be used in IPS/IDS, Web Firewall and similar applications.

For more information on UDPI and its features please visit the
[UDPI website](https://wiki.fd.io/view/UDPI)


## Changes

Details of the changes leading up to this version of UDPI can be found under
@ref release notes.

## Quick Start ##(in CentOS cmake = cmake3, gcc = devtoolset-7, follow vpp)
```
From the code tree root

if you have not any sepcical request for udpi compile
you should use the Makefile in the code tree root (suggest to install all the vpp packages first)

Just run blew command

$ make install-dep
$ make build-install-package-hyperscan
$ make build-package

then you can use
$ rpm -i --force build-root/build-packge/udpi*.rpm # you may meet conflict form vpp-devel, but this is not a conflict, force install
or
$ dpkg -i build-root/build-package/udpi*.dep


else you should use cmake directly, like blew


(VPP installed with DEB or RPM pkg)
$ cd udpi-plugin
$ mkdir -p build
$ cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr #(add "-DCMKAE_INSTALL_LIBDIR=lib -DCMAKE_PROGRAM_PATH:PATH="/opt/rh/devtoolset-7/root/bin"" and change cmake to cmake3 on centos and it's releated)
$ make package

(VPP source code -- build type RELEASE)
$ cd udpi-plugin
$ mkdir -p build
$ cd build
$ cmake .. -DVPP_HOME=<vpp dir>/build-root/install-vpp-native/vpp -DCMAKE_INSTALL_PREFIX=<vpp src>/build-root/install-vpp-native/vpp  #(add "-DCMKAE_INSTALL_LIBDIR=lib -DCMAKE_PROGRAM_PATH:PATH="/opt/rh/devtoolset-7/root/bin"" and change cmake to cmake3 on centos and it's releated)
$ make
$ sudo make install

(VPP source code -- build type DEBUG)
$ cd udpi-plugin
$ mkdir -p build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DVPP_HOME=<vpp dir>/build-root/install-vpp_debug-native/vpp -DCMAKE_INSTALL_PREFIX=<vpp src>/build-root/install-vpp_debug-native/vpp  #(add "-DCMKAE_INSTALL_LIBDIR=lib -DCMAKE_PROGRAM_PATH:PATH="/opt/rh/devtoolset-7/root/bin"" and change cmake to cmake3 on centos and it's releated)
$ make
$ sudo make install

CMAKE variables:
- CMKAE_INSTALL_LIBDIR -- set the library directory. In CentOS, it is lib or lib64, in debian and it's related, it is lib/x86_64-linux-gnu
- CMAKE_INSTALL_PREFIX -- set the install directory for the udpi-plugin. This is the common path to the lib folder containing vpp_plugins and vpp_api_test_plugins folders. Default is /usr/local.
- VPP_HOME -- set the directory containing the include and lib directories of vpp.
```

## Using udpi plugin ##

### Platforms ###

udpi-plugin has been tested in:

- Ubuntu 16.04 LTS (x86_64)
- Ubuntu 18.04 LTS (x86_64)
- Debian Stable/Testing
- Red Hat Enterprise Linux 7
- CentOS 7


### Dependencies ###

Build dependencies:

- Hyperscan

- VPP 20.01
  - DEB packages (can be found https://packagecloud.io/fdio/release/install):
  - libvppinfra-dev
  - vpp-dev

Running dependencies:

- VPP 20.01
  - DEB packages (can be found https://packagecloud.io/fdio/release/install):
  - vpp
  - vpp-plugin-core
  - vpp-plugin-dpdk (only to use DPDK compatible nics)

Hardware support (not mandatory):

- [DPDK](http://DPDK.org/) compatible nics

## Getting started ##
In order to start, the udpi plugin requires a running instance of VPP
The steps required to successfully start udpi are:

- Setup the host to run VPP
- Configure VPP to use DPDK compatible nics
- Start VPP
- Configure VPP interfaces
- Configure and start udpi

Detailed information for configuring VPP can be found at [https://wiki.fd.io/view/VPP](https://wiki.fd.io/view/VPP).

### Setup the host for VPP ###

Hugepages must be enabled in the system

```
$ sudo sysctl -w vm.nr_hugepages=1024
```

In order to use a DPDK interface, the `uio` and `uio_pci_generic` or `vfio_pci` modules need to be loaded in the kernel

```
$ sudo modprobe uio
$ sudo modprobe uio_pci_generic
$ sudo modprobe vfio_pci
```

If the DPDK interface we want to assign to VPP is up, we must bring it down

```
$ sudo ifconfig <interface_name> down
```
or
```
$ sudo ip link set <interface_name> down
```

### Configure VPP ###
The file /etc/VPP/startup.conf contains a set of parameters to setup VPP at startup.
The following example sets up VPP to use a DPDK interfaces:

``` shell
unix {
  nodaemon
  log /tmp/vpp.log
  full-coredump
}

api-trace {
  on
}

api-segment {
  gid vpp
}

dpdk {
  dev 0000:08:00.0
}

plugins {
        ## Disable all plugins by default and then selectively enable specific plugins

        ## Enable all plugins by default and then selectively disable specific plugins
}
```
Where `0000:08:00.0` must be replaced with the actual PCI address of the DPDK interface

### Start VPP ###

VPP can be started as a process or a service:

``` shell
Start VPP as a service in Ubuntu 16.04
$ sudo systemctl start vpp

Start VPP as a process in both 16.04
$ sudo vpp -c /etc/vpp/startup.conf

```

### Configure udpi plugin ###
The udpi plugin can be configured either using the VPP command-line interface (CLI), through the VPP binary api

see src/dpi_plugin_doc.md in details. 

#### udpi plugin CLI ####

The CLI commands for the udpi plugin start all with the udpi keyword. To see the full list of command available type:
...(to be added)

## License ##

This software is distributed under the following license:

```
Copyright (c) 2018-2019 Intel and/or its affiliates.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

```
