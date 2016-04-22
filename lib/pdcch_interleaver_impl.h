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

#ifndef INCLUDED_MY_LTE_PDCCH_INTERLEAVER_IMPL_H
#define INCLUDED_MY_LTE_PDCCH_INTERLEAVER_IMPL_H

#include <my_lte/pdcch_interleaver.h>

#define TX_NULL_BIT 100

namespace gr {
  namespace my_lte {

    class pdcch_interleaver_impl : public pdcch_interleaver
    {
     private:
      int d_n_ant, d_cell_id;

     public:
      pdcch_interleaver_impl(int N_ant, int N_cell_id);
      ~pdcch_interleaver_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);

      void pdcch_interleave(const gr_complex *in,
                            int N_input,
                            int N_cell_id, gr_complex *out, int *N_out);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PDCCH_INTERLEAVER_IMPL_H */

