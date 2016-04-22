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

#ifndef INCLUDED_MY_LTE_RATE_MATCH_CONV_IMPL_H
#define INCLUDED_MY_LTE_RATE_MATCH_CONV_IMPL_H

#include <my_lte/rate_match_conv.h>

#define TX_NULL_BIT 100

namespace gr {
  namespace my_lte {

  class rate_match_conv_impl : public rate_match_conv
  {
   private:
    // Nothing to declare in this block.
      uint32 N_out_bits;
      uint8 out_bits[1920*2];       // as long as possible

      // Rate Match Conv
      uint8 rmc_tmp[1024];          // 1024 = 32(col) * 32(row)
      uint8 rmc_sb_mat[32][32];
      uint8 rmc_sb_perm_mat[32][32];
      uint8 rmc_w[3*1024];

   public:
    rate_match_conv_impl(uint32 N_e_bits);
    ~rate_match_conv_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);

    int general_work(int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    void rate_match_conv(uint8 *d_bits,
                         uint32 N_d_bits,
                         uint32 N_e_bits,
                         uint8 *e_bits);

  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RATE_MATCH_CONV_IMPL_H */

