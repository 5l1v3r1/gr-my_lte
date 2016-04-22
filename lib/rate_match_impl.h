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

#ifndef INCLUDED_MY_LTE_RATE_MATCH_IMPL_H
#define INCLUDED_MY_LTE_RATE_MATCH_IMPL_H

#include <my_lte/rate_match.h>

namespace gr {
  namespace my_lte {

  class rate_match_impl : public rate_match
  {
   private:
      uint32 d_N_e_bits;
      uint32 d_N_code_block;
      int d_tx_mode;
      uint32 d_N_soft;
      int d_M_dl_harq;
      int d_rv_idx;
      uint8 e_bits[3*6144];   // 3*6144 = max of output of encoder
      string channel_name;

   public:
    rate_match_impl(uint32 N_e_bits,
                    uint32 N_code_block,
                    int tx_mode,
                    uint32 N_soft,
                    int M_dl_harq,
                    int rv_idx,
                    string chan_name);
    ~rate_match_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);

    int general_work(int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    void rate_match_turbo(uint8                     *d_bits,
                          uint32                     N_d_bits,
                          uint32                     N_codeblocks,
                          uint32                     tx_mode,
                          uint32                     N_soft,
                          uint32                     M_dl_harq,
                          string                     chan_name,
                          uint32                     rv_idx,
                          uint32 N_e_bits, uint8 *e_bits);
  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RATE_MATCH_IMPL_H */

