# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/anye/esp/esp-idf-4.3/components/bootloader/subproject"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix/tmp"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix/src/bootloader-stamp"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix/src"
  "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/anye/Desktop/lv_port_esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
