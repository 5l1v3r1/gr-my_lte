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

#ifndef INCLUDED_MY_LTE_SCRAMBLER_PHICH_IMPL_H
#define INCLUDED_MY_LTE_SCRAMBLER_PHICH_IMPL_H

#include <my_lte/scrambler_phich.h>

using namespace std;

namespace gr {
  namespace my_lte {

    class scrambler_phich_impl : public scrambler_phich
    {
    private:
       // Nothing to declare in this block.
       uint32 cinit;
       uint32 N_id_cell;
       uint32 count;   // counting frames or subframes (depends on the channel)
       uint16 RNTI;

    public:
     scrambler_phich_impl(uint32 initial, short rnti, uint32 set_align);
     ~scrambler_phich_impl();

     // Where all the action really happens
     int work(int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items);

     void generate_prs_c(uint32  c_init,
                         uint32  len,
                         uint32 *c);

     void update_cinit(uint32 subfrm_num);

    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_SCRAMBLER_PHICH_IMPL_H */

