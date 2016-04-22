/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_IMPL_H
#define INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_IMPL_H

#include <my_lte/code_block_segmentation.h>

#define N_C_BIT_MAX 6144
#define TX_NULL_SYMB 100

namespace gr {
  namespace my_lte {

  class code_block_segmentation_impl : public code_block_segmentation
  {
   private:
      uint8 c_bits[20*N_C_BIT_MAX];      //to generate all the code blocks
      uint32 N_code_block;
      uint32 N_filler_bits;
      uint32 N_c_bits[20];            // we assume that the max of n codeblocks is 20

   public:
    code_block_segmentation_impl();
    ~code_block_segmentation_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);

    int general_work(int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    void code_blk_segmentation(uint8  *b_bits,
                               uint32  N_b_bits,
                               uint32 *N_codeblocks,
                               uint32 *N_filler_bits,
                               uint8 *c_bits,
                               uint32 *N_c_bits);

    void calc_crc(uint8 *a_bits,
                  gr_uint32  N_a_bits,
                  gr_uint32  crc,
                  uint8  *p_bits,
                  gr_uint32  N_p_bits);

  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_IMPL_H */

