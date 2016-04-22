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

#ifndef INCLUDED_MY_LTE_MODULATION_MAPPER_IMPL_H
#define INCLUDED_MY_LTE_MODULATION_MAPPER_IMPL_H

#include <my_lte/modulation_mapper.h>

namespace gr {
  namespace my_lte {

  // modulation types
  typedef enum{
      LIBLTE_PHY_MODULATION_TYPE_BPSK = 0,
      LIBLTE_PHY_MODULATION_TYPE_QPSK,
      LIBLTE_PHY_MODULATION_TYPE_16QAM,
      LIBLTE_PHY_MODULATION_TYPE_64QAM,
  }LIBLTE_PHY_MODULATION_TYPE_ENUM;

    class modulation_mapper_impl : public modulation_mapper
    {
     private:
      // modulation type
        LIBLTE_PHY_MODULATION_TYPE_ENUM modulation_t;
        uint32 samps_per_symb;      // number of samps per modulation symbol

     public:
      modulation_mapper_impl(string mod_t, uint32 set_align);
      ~modulation_mapper_impl();

      // Where all the action really happens
//      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void modulation_mapper(uint8                           *bits,
                                  uint32                           N_bits,
                                  LIBLTE_PHY_MODULATION_TYPE_ENUM  type,
                                  float                           *d_re,
                                  float                           *d_im,
                                  uint32                          *M_symb);

      void symb_to_gr_complex(float *re, float *im, uint32 len, gr_complex *out);

      uint32 mod_type_det(string mod_t);

    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_MODULATION_MAPPER_IMPL_H */

