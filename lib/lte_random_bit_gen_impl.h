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

#ifndef INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_IMPL_H
#define INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_IMPL_H

#include <my_lte/lte_random_bit_gen.h>

namespace gr {
  namespace my_lte {

  class lte_random_bit_gen_impl : public lte_random_bit_gen
  {
  private:
      int packet_length;
      uint32 d_usleep_val;
      bool is_disable;
      uint32 count;
   public:
    lte_random_bit_gen_impl(int packet_len, uint32 usleep_val);
    ~lte_random_bit_gen_impl();

    // Where all the action really happens
    int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

    void msg_handler(pmt::pmt_t msg);

  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_IMPL_H */

