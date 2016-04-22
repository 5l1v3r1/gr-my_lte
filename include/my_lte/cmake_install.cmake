# Install script for directory: /home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/my_lte" TYPE FILE FILES
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/api.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/lte_random_bit_gen.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/crc_attachment.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/code_block_segmentation.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/turbo_encoder.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/rate_match.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/scrambler.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/modulation_mapper.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/layer_mapper.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/precoder.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/conv_enc.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/rate_match_conv.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/code_block_concatenation.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/resource_mapper.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/cp_adder.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/pdcch_mux.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/dci_gen.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/cfi_gen.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/phich_orthogonalization.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/repeater.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/phich_alignment.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/phich_grouping.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/pdcch_interleaver.h"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include/my_lte/scrambler_phich.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

