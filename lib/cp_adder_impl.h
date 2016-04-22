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

#ifndef INCLUDED_MY_LTE_CP_ADDER_IMPL_H
#define INCLUDED_MY_LTE_CP_ADDER_IMPL_H

#include <my_lte/cp_adder.h>
//#include <stdio.h>

using namespace std;

namespace gr {
  namespace my_lte {

    class cp_adder_impl : public cp_adder
    {
     private:
        uint32 fft_size;
        bool d_normal_cp;
        int cp_l, cp_s, cp_e;

     public:
      cp_adder_impl(uint32 N_dl_prb, bool normal_cp);
      ~cp_adder_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      int output_vec_size(uint32 d_N_dl_prb);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CP_ADDER_IMPL_H */

