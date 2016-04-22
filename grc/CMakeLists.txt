# Copyright 2011 Free Software Foundation, Inc.
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

install(FILES
    my_lte_lte_random_bit_gen.xml
    my_lte_crc_attachment.xml
    my_lte_code_block_segmentation.xml
    my_lte_turbo_encoder.xml
    my_lte_rate_match.xml
    my_lte_scrambler.xml
    my_lte_modulation_mapper.xml
    my_lte_layer_mapper.xml
    my_lte_precoder.xml
    my_lte_conv_enc.xml
    my_lte_rate_match_conv.xml
    my_lte_code_block_concatenation.xml
    my_lte_resource_mapper.xml
    my_lte_cp_adder.xml
    my_lte_pdcch_mux.xml
    my_lte_dci_gen.xml
    my_lte_cfi_gen.xml
    my_lte_phich_orthogonalization.xml
    my_lte_repeater.xml
    my_lte_phich_alignment.xml
    my_lte_phich_grouping.xml
    my_lte_pdcch_interleaver.xml
    my_lte_scrambler_phich.xml DESTINATION share/gnuradio/grc/blocks
)
