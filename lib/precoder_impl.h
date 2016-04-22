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

#ifndef INCLUDED_MY_LTE_PRECODER_IMPL_H
#define INCLUDED_MY_LTE_PRECODER_IMPL_H

#include <my_lte/precoder.h>

namespace gr {
  namespace my_lte {

  class precoder_impl : public precoder
  {
   private:
      uint32 N_antenna;
      uint32 d_n_in_items_required;
      bool d_is_phich, d_cp_normal;
      uint32 phich_count;   // for counting PHICH at the inputs (we need 3 of them for one PHICH)
      uint32 d_n_phich_group;   // group number for PHICH
      gr_complex y_symb_phich0[12],y_symb_phich1[12],y_symb_phich2[12],y_symb_phich3[12];   // for PHICH

   public:
    precoder_impl(uint32 N_ant, uint32 set_align, bool is_phich, int n_phich_group, bool normal_cp);
    ~precoder_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);

    int general_work(int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);
  };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PRECODER_IMPL_H */

