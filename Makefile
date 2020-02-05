# Copyright (c) 2018 Intel and/or its affiliates.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


export WS_ROOT=$(CURDIR)
export BR=$(WS_ROOT)/build-root
PLATFORM?=UDPI

##############
#OS Detection#
##############
ifneq ($(shell uname),Darwin)
OS_ID        = $(shell grep '^ID=' /etc/os-release | cut -f2- -d= | sed -e 's/\"//g')
OS_VERSION_ID= $(shell grep '^VERSION_ID=' /etc/os-release | cut -f2- -d= | sed -e 's/\"//g')
endif

ifeq ($(filter ubuntu debian,$(OS_ID)),$(OS_ID))
PKG=deb
cmake=cmake
else ifeq ($(filter rhel centos fedora opensuse opensuse-leap opensuse-tumbleweed,$(OS_ID)),$(OS_ID))
PKG=rpm
cmake=cmake3
endif

#####
#DEB#
#####
#Dependencies to build
DEB_DEPENDS = curl build-essential autoconf automake ccache git cmake wget coreutils ragel
DEB_DEPENDS += libboost-dev vpp vpp-dev python*-ply
#####
#RPM#
#####
#Dependencies to build
RPM_DEPENDS = curl autoconf automake ccache cmake3 wget gcc gcc-c++ git gtest gtest-devel
RPM_DEPENDS += ragel python-sphinx boost169-devel vpp vpp-devel python*-ply

.PHONY: help install-dep build build-package build-install-package-hyperscan checkstyle distclean

help:
	@echo "Make Targets:"
	@echo " install-dep                       - install software dependencies"
	@echo " build-package                     - build rpm or deb package"
	@echo " build-install-package-hyperscan   - build rpm or deb package for hyperscan"
	@echo " checkstyle                        - checkstyle"
	@echo " distclean                         - remove all build directory"

install-dep:
ifeq ($(filter ubuntu debian,$(OS_ID)),$(OS_ID))
ifeq ($(OS_VERSION_ID),14.04)
	@sudo -E apt-get -y --force-yes install software-properties-common
endif
	@sudo -E apt-get update
	@curl -s https://packagecloud.io/install/repositories/fdio/2001/script.deb.sh | sudo bash
	@sudo -E apt-get $(APT_ARGS) -y --force-yes install $(DEB_DEPENDS)
else ifeq ($(OS_ID),centos)
	@curl -s https://packagecloud.io/install/repositories/fdio/2001/script.rpm.sh | sudo bash
	@sudo -E yum install -y $(RPM_DEPENDS) epel-release centos-release-scl devtoolset-7
else
	$(error "This option currently works only on Ubuntu, Debian, Centos or openSUSE systems")
endif

build-install-package-hyperscan:
	@rm -rf $(BR)/build-package-hyperscan/;
	@mkdir -p $(BR)/build-package-hyperscan/; cd $(BR)/build-package-hyperscan/;\
	git clone https://github.com/intel/hyperscan.git; cd hyperscan; \
	git apply $(BR)/../0001-build-package-for-hyperscan.patch; make build-install-package;

build-package:
ifeq ($(filter ubuntu debian,$(OS_ID)),$(OS_ID))
	@mkdir -p $(BR)/build-package/; cd $(BR)/build-package/;\
        $(cmake) -DCMAKE_BUILD_TYPE=Release\
        -DCMAKE_INSTALL_PREFIX=/usr $(WS_ROOT)/;\
        make package -j$(nproc);
else ifeq ($(OS_ID),centos)
	@mkdir -p $(BR)/build-package/; cd $(BR)/build-package/;\
        $(cmake) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_LIBDIR=lib\
        -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PROGRAM_PATH:PATH="/opt/rh/devtoolset-7/root/bin"\
        $(WS_ROOT)/;\
        make package -j$(nproc);
endif
	@# NEW INSTRUCTIONS TO BUILD-PACKAGE MUST BE DECLARED ON A NEW LINE WITH
	@# '@' NOT WITH ';' ELSE BUILD-PACKAGE WILL NOT RETURN THE CORRECT
	@# RETURN CODE FOR JENKINS CI
	@rm -rf $(BR)/build-package/_CPack_Packages;

checkstyle:
	@$(BR)/../scripts/checkstyle.sh

distclean:
	@rm -rf $(BR)/build-package*
