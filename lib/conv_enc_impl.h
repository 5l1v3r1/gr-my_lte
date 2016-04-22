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

#ifndef INCLUDED_MY_LTE_CONV_ENC_IMPL_H
#define INCLUDED_MY_LTE_CONV_ENC_IMPL_H

#include <my_lte/conv_enc.h>

namespace gr {
  namespace my_lte {

  class conv_enc_impl : public conv_enc
  {
   private:
      uint32 constraint_len, rate;
      vector<uint32> matrix_gen;
      bool tail;
      pmt::pmt_t d_pmt_id;
      uint8 out_bits[1000*3];       // as long as possible

   public:
    conv_enc_impl(uint32 constraint_length,
                  uint32 rate,
                  vector<uint32> g,
                  bool tail_bit);
    ~conv_enc_impl();

//      void forecast(uint32noutput_items, gr_vector_uint32&ninput_items_required);

    void conv_encode(uint8          *c_bits,
                     uint32         N_c_bits,
                     uint32         constraint_len,
                     uint32         rate,
                     vector<uint32> g,
                     bool           tail_bit,
                     uint8          *d_bits,
                     uint32         *N_d_bits);

    // Where all the action really happens
    int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CONV_ENC_IMPL_H */

