/* -*- c++ -*- */

#define MY_LTE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "my_lte_swig_doc.i"

%{
#include "my_lte/lte_random_bit_gen.h"
#include "my_lte/crc_attachment.h"
#include "my_lte/code_block_segmentation.h"
#include "my_lte/turbo_encoder.h"
#include "my_lte/rate_match.h"
#include "my_lte/scrambler.h"
#include "my_lte/modulation_mapper.h"
#include "my_lte/layer_mapper.h"
#include "my_lte/precoder.h"
#include "my_lte/conv_enc.h"
#include "my_lte/rate_match_conv.h"
#include "my_lte/code_block_concatenation.h"
#include "my_lte/resource_mapper.h"
#include "my_lte/cp_adder.h"
#include "my_lte/pdcch_mux.h"
#include "my_lte/dci_gen.h"
#include "my_lte/cfi_gen.h"
#include "my_lte/phich_orthogonalization.h"
#include "my_lte/repeater.h"
#include "my_lte/phich_alignment.h"
#include "my_lte/phich_grouping.h"
#include "my_lte/pdcch_interleaver.h"
#include "my_lte/scrambler_phich.h"
%}


%include "my_lte/lte_random_bit_gen.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, lte_random_bit_gen);
%include "my_lte/crc_attachment.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, crc_attachment);
%include "my_lte/code_block_segmentation.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, code_block_segmentation);
%include "my_lte/turbo_encoder.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, turbo_encoder);
%include "my_lte/rate_match.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, rate_match);
%include "my_lte/scrambler.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, scrambler);
%include "my_lte/modulation_mapper.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, modulation_mapper);
%include "my_lte/layer_mapper.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, layer_mapper);
%include "my_lte/precoder.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, precoder);
%include "my_lte/conv_enc.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, conv_enc);
%include "my_lte/rate_match_conv.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, rate_match_conv);
%include "my_lte/code_block_concatenation.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, code_block_concatenation);
%include "my_lte/resource_mapper.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, resource_mapper);
%include "my_lte/cp_adder.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, cp_adder);
%include "my_lte/pdcch_mux.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, pdcch_mux);
%include "my_lte/dci_gen.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, dci_gen);
%include "my_lte/cfi_gen.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, cfi_gen);
%include "my_lte/phich_orthogonalization.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, phich_orthogonalization);
%include "my_lte/repeater.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, repeater);
%include "my_lte/phich_alignment.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, phich_alignment);
%include "my_lte/phich_grouping.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, phich_grouping);
%include "my_lte/pdcch_interleaver.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, pdcch_interleaver);
%include "my_lte/scrambler_phich.h"
GR_SWIG_BLOCK_MAGIC2(my_lte, scrambler_phich);
