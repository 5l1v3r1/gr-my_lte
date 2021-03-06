# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND my_lte_sources
    lte_random_bit_gen_impl.cc
    crc_attachment_impl.cc
    code_block_segmentation_impl.cc
    turbo_encoder_impl.cc
    rate_match_impl.cc
    scrambler_impl.cc
    modulation_mapper_impl.cc
    layer_mapper_impl.cc
    precoder_impl.cc
    conv_enc_impl.cc
    rate_match_conv_impl.cc
    code_block_concatenation_impl.cc
    resource_mapper_impl.cc
    cp_adder_impl.cc
    pdcch_mux_impl.cc
    dci_gen_impl.cc
    cfi_gen_impl.cc
    phich_orthogonalization_impl.cc
    repeater_impl.cc
    phich_alignment_impl.cc
    phich_grouping_impl.cc
    pdcch_interleaver_impl.cc
    scrambler_phich_impl.cc
)

set(my_lte_sources "${my_lte_sources}" PARENT_SCOPE)
if(NOT my_lte_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT my_lte_sources)

add_library(gnuradio-my_lte SHARED ${my_lte_sources})
target_link_libraries(gnuradio-my_lte ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-my_lte PROPERTIES DEFINE_SYMBOL "gnuradio_my_lte_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-my_lte PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-my_lte
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_my_lte_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_my_lte.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_my_lte.cc
)

add_executable(test-my_lte ${test_my_lte_sources})

target_link_libraries(
  test-my_lte
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${GNURADIO_ALL_LIBRARIES}
  gnuradio-my_lte
)

GR_ADD_TEST(test_my_lte test-my_lte)
