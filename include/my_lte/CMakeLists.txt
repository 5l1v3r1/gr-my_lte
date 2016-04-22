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
# Install public header files
########################################################################
install(FILES
    api.h
    lte_random_bit_gen.h
    crc_attachment.h
    code_block_segmentation.h
    turbo_encoder.h
    rate_match.h
    scrambler.h
    modulation_mapper.h
    layer_mapper.h
    precoder.h
    conv_enc.h
    rate_match_conv.h
    code_block_concatenation.h
    resource_mapper.h
    cp_adder.h
    pdcch_mux.h
    dci_gen.h
    cfi_gen.h
    phich_orthogonalization.h
    repeater.h
    phich_alignment.h
    phich_grouping.h
    pdcch_interleaver.h
    scrambler_phich.h DESTINATION include/my_lte
)
