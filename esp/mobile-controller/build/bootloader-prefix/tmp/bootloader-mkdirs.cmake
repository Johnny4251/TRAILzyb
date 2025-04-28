# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/johnny/esp/esp-idf/components/bootloader/subproject"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/tmp"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/src/bootloader-stamp"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/src"
  "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/johnny/git/TRAILzyb/esp/mobile-controller/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
