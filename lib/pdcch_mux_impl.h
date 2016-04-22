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

#ifndef INCLUDED_MY_LTE_PDCCH_MUX_IMPL_H
#define INCLUDED_MY_LTE_PDCCH_MUX_IMPL_H

#include <my_lte/pdcch_mux.h>

namespace gr {
  namespace my_lte {

    class pdcch_mux_impl : public pdcch_mux
    {
     private:
        int d_n_pdcch, N_CCE;
        vector<int> d_agg_level;

     public:
      pdcch_mux_impl(int n_pdcch, int n_cce);
      ~pdcch_mux_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PDCCH_MUX_IMPL_H */

#define NIL 0xF0
