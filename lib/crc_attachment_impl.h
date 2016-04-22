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

#ifndef INCLUDED_MY_LTE_CRC_ATTACHMENT_IMPL_H
#define INCLUDED_MY_LTE_CRC_ATTACHMENT_IMPL_H

#include <my_lte/crc_attachment.h>

namespace gr {
  namespace my_lte {

  class crc_attachment_impl : public crc_attachment
  {
   private:
    // Nothing to declare in this block.
      uint32 crc_type, d_rnti;
      pmt::pmt_t d_key;
      gr_uint32 n_p_bits;        // number of parity bits generated
      int ninput_required;

      // for PBCH
      bool is_bch, is_dci;
      uint8   ant_mask_1[16];
      uint8   ant_mask_2[16];
      uint8   ant_mask_4[16];
      int N_ant;

   public:
    crc_attachment_impl(uint32 crc_type, int N_ant, string ch_name, int ninput_needed, uint32 rnti);
    ~crc_attachment_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);


    int general_work(int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    void calc_crc(uint8 *a_bits,
                  gr_uint32  N_a_bits,
                  gr_uint32  crc,
                  uint8  *p_bits,
                  gr_uint32  N_p_bits);
  };


  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CRC_ATTACHMENT_IMPL_H */

