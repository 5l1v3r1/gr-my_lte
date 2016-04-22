# Install script for directory: /home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_lte_random_bit_gen.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_crc_attachment.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_code_block_segmentation.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_turbo_encoder.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_rate_match.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_scrambler.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_modulation_mapper.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_layer_mapper.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_precoder.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_conv_enc.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_rate_match_conv.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_code_block_concatenation.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_resource_mapper.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_cp_adder.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_pdcch_mux.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_dci_gen.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_cfi_gen.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_phich_orthogonalization.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_repeater.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_phich_alignment.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_phich_grouping.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_pdcch_interleaver.xml"
    "/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/grc/my_lte_scrambler_phich.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

