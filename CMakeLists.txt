# Copyright (C) 2019 Adolfo E. García
#
# This file is part of STG-8nn-Scaffold.
#
# STG-8nn-Scaffold is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# STG-8nn-Scaffold is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

##
## Add the scaffold include file
##

set(SCAFFOLD_DIR stg-8nn-scaffold)
include(${SCAFFOLD_DIR}/CMakeLists_scaffold.txt.in)

## Override app configuration with app_config.h
add_definitions(-DAPP_ENABLE_CUSTOM_BUILD_CONFIG)

##
## Add the blinky source files to the scaffold's lists
##
set(BLINKY_DIR blinky)

set(BLINKY_SOURCES
  ${BLINKY_DIR}/app_config.h
  ${BLINKY_DIR}/led.h
  ${BLINKY_DIR}/led.c
  ${BLINKY_DIR}/blinky.c
)

set(BLINKY_INCLUDE ${BLINKY_DIR})

list(APPEND SCAFFOLD_SOURCES ${BLINKY_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${BLINKY_INCLUDE})

##
## Configure build
##

set(OUTPUT_NAME firmware)

##
## elf definition
##

add_executable(${OUTPUT_NAME}.elf ${SCAFFOLD_SOURCES})
target_include_directories(${OUTPUT_NAME}.elf PRIVATE ${SCAFFOLD_INCLUDE})

##
## linker script and bin file
##

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  ## Alternatively, you can use a modified copy of the STM32F091CCUx_FLASH.ld file instead.
  add_custom_command(
    TARGET ${OUTPUT_NAME}.elf PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${SCAFFOLD_DIR}/dependencies/stm32cube/gnu/STM32F091CCUx_FLASH.ld ${PROJECT_BINARY_DIR}/stg8nn.ld
  )

  add_custom_command(TARGET ${OUTPUT_NAME}.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.bin
  )
endif()

##
## tester libraries
##
if(NOT "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  target_link_libraries(${OUTPUT_NAME}.elf pthread)
endif()
