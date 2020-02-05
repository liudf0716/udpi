# Copyright (c) 2017-2019 Cisco, Intel and/or its affiliates.
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

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(udpi-plugin)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 11)

# Check for memfd_create syscall
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS ( "__NR_memfd_create" "sys/syscall.h" HAVE_MEMFD_CREATE )
if ( HAVE_MEMFD_CREATE )
    add_definitions ( -DHAVE_MEMFD_CREATE )
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(UDPI_PLUGIN udpi-plugin)
endif()

include (Packager)

# Dependencies

message(STATUS "Looking for Hyperscan")
find_path(HYPERSCAN_INCLUDE_DIR NAMES hs/hs.h)
find_library(HYPERSCAN_LIB1 NAMES hs)
find_library(HYPERSCAN_LIB2 NAMES hs_runtime)
set (HYPERSCAN_LIB ${HYPERSCAN_LIB1} ${HYPERSCAN_LIB2})
if(HYPERSCAN_INCLUDE_DIR AND HYPERSCAN_LIB)
  include_directories(${HYPERSCAN_INCLUDE_DIR})
  message(STATUS "Found Hyperscan in ${HYPERSCAN_INCLUDE_DIR}")
else()
  message(WARNING "-- Hyperscan not found - dpi_plugin disabled")
endif()

find_package(VPP REQUIRED)

include_directories(${VPP_INCLUDE_DIR})

set(UDPI_PLUGIN_FILES
    src/dpi.c
    src/dpi_api.c
    src/dpi_cli.c
    src/dpi_node.c
    src/protocols/dpi_ssl.c
)

set(UDPI_PLUGIN_HEADER_FILES
    src/dpi_app_match.h
    src/dpi.h
    )

set(UDPI_API_TEST_SOURCE_FILES
    )

set(UDPI_API_GENERATED_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.h)

set(UDPI_VAPI_GENERATED_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.vapi.h
    ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.vapi.hpp)

if (NOT VPP_HOME)
    set(VPP_HOME /usr)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

SET(UDPI_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} CACHE STRING "udpi_install_prefix")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
   set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -march=native -O3 -g")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -march=native -O0 -g")
   add_definitions(-DCLIB_DEBUG -fPIC -fstack-protector-all)
endif()

execute_process(
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi
    )
# These files are missing from vpp binary distribution
execute_process(
    COMMAND curl https://git.fd.io/vpp/plain/src/vpp-api/vapi/vapi_json_parser.py?h=stable/2001 -o ${CMAKE_BINARY_DIR}/vapi_json_parser.py
    COMMAND curl https://git.fd.io/vpp/plain/src/vpp-api/vapi/vapi_c_gen.py?h=stable/2001  -o ${CMAKE_BINARY_DIR}/vapi_c_gen.py
    COMMAND curl https://git.fd.io/vpp/plain/src/vpp-api/vapi/vapi_cpp_gen.py?h=stable/2001  -o ${CMAKE_BINARY_DIR}/vapi_cpp_gen.py
    )

add_custom_command(
    COMMAND chmod +x  ${CMAKE_BINARY_DIR}/vapi_json_parser.py ${CMAKE_BINARY_DIR}/vapi_c_gen.py ${CMAKE_BINARY_DIR}/vapi_cpp_gen.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.h
    COMMAND ${VPP_HOME}/bin/vppapigen ARGS --includedir ${CMAKE_CURRENT_BINARY_DIR} --input ${CMAKE_CURRENT_SOURCE_DIR}/src/dpi.api --output ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.h --outputdir ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/dpi.api
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.json
    COMMAND ${VPP_HOME}/bin/vppapigen ARGS JSON --includedir ${CMAKE_CURRENT_BINARY_DIR} --input ${CMAKE_CURRENT_SOURCE_DIR}/src/dpi.api --output ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.json --outputdir ${CMAKE_CURRENT_BINARY_DIR}/vapi/
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.vapi.h
    COMMAND ${CMAKE_BINARY_DIR}/vapi_c_gen.py ARGS ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.json
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.vapi.hpp
    COMMAND ${CMAKE_BINARY_DIR}/vapi_cpp_gen.py ARGS ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi/dpi.api.json
        )

include_directories(SYSTEM)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUDPI_VPP_PLUGIN=1")
add_library(udpi_plugin SHARED
    ${UDPI_PLUGIN_FILES}
    ${UDPI_API_GENERATED_FILES}
    ${UDPI_VAPI_GENERATED_FILES})
target_link_libraries(udpi_plugin ${HYPERSCAN_LIB})

file(COPY ${UDPI_PLUGIN_HEADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins/udpi)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/vpp_plugins)

add_library(udpi_api_test_plugin SHARED
    ${UDPI_API_TEST_SOURCE_FILES}
    ${UDPI_API_GENERATED_FILES})

set(VPP_INSTALL_PLUGIN ${UDPI_INSTALL_PREFIX}/vpp_plugins)
set(VPP_INSTALL_API_TEST_PLUGIN ${UDPI_INSTALL_PREFIX}/vpp_api_test_plugins CACHE STRING "vpp_install_api_test_plugin")

set_target_properties(udpi_plugin
        PROPERTIES
        LINKER_LANGUAGE C
        INSTALL_RPATH ${VPP_INSTALL_PLUGIN}
        PREFIX "")
set_target_properties(udpi_api_test_plugin
        PROPERTIES
        LINKER_LANGUAGE C
        PREFIX "")

install(DIRECTORY
        DESTINATION ${VPP_INSTALL_PLUGIN}
        COMPONENT ${UDPI_PLUGIN})
install(TARGETS udpi_plugin
        DESTINATION ${VPP_INSTALL_PLUGIN}
        COMPONENT ${UDPI_PLUGIN})

#install(DIRECTORY
#        DESTINATION ${VPP_INSTALL_API_TEST_PLUGIN}
#        COMPONENT ${UDPI_PLUGIN})
#install(TARGETS udpi_api_test_plugin
#        DESTINATION ${VPP_INSTALL_API_TEST_PLUGIN}
#        COMPONENT ${UDPI_PLUGIN})

install(FILES ${UDPI_API_HEADER_FILES} ${UDPI_API_GENERATED_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/vpp_plugins/udpi
        COMPONENT ${UDPI_PLUGIN}-dev)

install(FILES ${UDPI_VAPI_GENERATED_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/vapi
        COMPONENT ${UDPI_PLUGIN}-dev)

make_packages()
