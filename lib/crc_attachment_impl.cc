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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "crc_attachment_impl.h"

namespace gr {
  namespace my_lte {

  crc_attachment::sptr
  crc_attachment::make(uint32 CRC_type, int N_ant, string ch_name, int ninput_needed, uint32 rnti)
  {
    return gnuradio::get_initial_sptr
      (new crc_attachment_impl(CRC_type, N_ant, ch_name, ninput_needed, rnti));
  }

  crc_attachment_impl::crc_attachment_impl(uint32 crc_type, int N_ant, string ch_name, int ninput_needed, uint32 rnti)
    : gr::block("crc_attatchment",
            gr::io_signature::make(1, 1, sizeof(uint8)),
                gr::io_signature::make(1, 1, sizeof(uint8))),
      crc_type(crc_type),
      N_ant(N_ant) ,
      d_key(pmt::string_to_symbol("length")),       // output tag
      is_bch( !ch_name.compare("BCH") ),            // checking if the channel is PBCH
      is_dci ( !ch_name.compare("DCI")),
      ninput_required(ninput_needed),
      d_rnti(rnti)                                  // used for DCI masking only
  {

      set_tag_propagation_policy(TPP_DONT);

      // determining the number of parity bits
      if(crc_type == CRC8)
          n_p_bits = 8;
      else if (crc_type == CRC16)
          n_p_bits = 16;
      else if (crc_type == CRC24A || crc_type == CRC24B)
          n_p_bits = 24;

      // set antennas' mask for bch channel
      if (is_bch)
      {
          for (gr_uint32 i=0; i<16; i++)
          {
              ant_mask_1[i] = 0;
              ant_mask_2[i] = 1;

              if (i%2)
                  ant_mask_4[i] = 1;
              else
                  ant_mask_4[i] = 0;
          }
      }
  }

  crc_attachment_impl::~crc_attachment_impl()
  {
  }

  void
  crc_attachment_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = ninput_required;
  }

  int
  crc_attachment_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];
      uint8 p_bits[30];     // 30= greater than max length of defined crc
      uint8  *ant_mask = NULL;

      vector<tag_t> v;
      get_tags_in_range(v,
                        0,
                        nitems_read(0),
                        nitems_read(0)+ninput_items[0],
              pmt::string_to_symbol("length"));

      gr_uint32 packet_len = 0, nout_written = 0;

      if (v.size()>0)
      {
          // getting the length of one packet
          packet_len = pmt::to_long(v[0].value);
      }
      else
      {
            throw runtime_error("No tags read");
      }

          // checking the output buffer is free enough
          if (packet_len + n_p_bits <= noutput_items)
          {
              if (ninput_items[0] >= packet_len & packet_len != 0)
              {
                  calc_crc(in,gr_uint32 (packet_len),crc_type,p_bits,n_p_bits);

                  // masking p_bits for PBCH
                  if (is_bch)
                  {
                      if(N_ant == 1)
                          {
                              ant_mask = ant_mask_1;
                          }else if(N_ant == 2){
                              ant_mask = ant_mask_2;
                          }else{
                              ant_mask = ant_mask_4;
                          }

                      for(int i=0; i<16; i++)
                          {

                              p_bits[i] = p_bits[i] ^ ant_mask[i];
//                              if (p_bits[i] == 0x00)
//                                  cout << "0 " ;
//                              else
//                                  cout << "1 ";
                          }
//                      cout << endl;

                  }
                  else if (is_dci)  // masking for DCI crc
                  {
                      uint8 x_rnti_bits[16];
                      // Convert RNTI to bit array
                      for(int i=0; i<16; i++)
                      {
                          x_rnti_bits[i] = (d_rnti >> (15-i)) & 1;
                          p_bits[i] = p_bits[i] ^ x_rnti_bits[i];
                      }

                  }
                  // attachment
                  memcpy(out,in,packet_len);
//                  out += packet_len;

                  memcpy(&out[packet_len] , p_bits, sizeof(uint8) * n_p_bits);

//                  if (packet_len == 2088)
//                  {
//                      cout << "PDSCH in+crc bits================================================  P bits" <<
//                           n_p_bits << "packet len:   " << packet_len << endl;
//                      for (int i=0; i<2088; i++)
//                      {
//                          if (out[i] == 0x00)
//                          {
//                              cout << "0 ";
//                          }
//                          else
//                          {
//                              cout << "1 ";
//                          }
//                      }
//                      cout << endl;
//                  }
                  // tagging outputs
                  add_item_tag(0,
                           nitems_written(0),
                           d_key,
                           pmt::from_long( long(packet_len + n_p_bits) ));

                  nout_written = packet_len + n_p_bits;

//                  cout << "crc " << endl;
              }
              else  // if there is not enough input items
              {
                  packet_len = 0;
              }
          }
          else     // not enough output items
          {
              packet_len = 0;
          }

          // we tell the runtime system every time we finish one attachment
          // Otherwise the data won't propogate through blocks

          consume_each (packet_len);
          return nout_written;

  }

  void crc_attachment_impl::calc_crc(uint8 *a_bits,
                gr_uint32  N_a_bits,
                gr_uint32  crc,
                uint8  *p_bits,
                gr_uint32  N_p_bits)
  {
      gr_uint32 i;
      gr_uint32 crc_rem   = 0;
      gr_uint32 crc_check = (1 << N_p_bits);
      vector<uint8>  tmp_array(N_a_bits + N_p_bits);

      // Initialize tmp_array
      memset(&tmp_array[0], 0, N_a_bits + N_p_bits);
      memcpy(&tmp_array[0], a_bits, N_a_bits);

      for(i=0; i<N_a_bits + N_p_bits; i++)
      {
          crc_rem <<= 1;
          crc_rem  |= tmp_array[i];

          if(crc_rem & crc_check)
          {
              crc_rem ^= crc;
          }
      }

      for(i=0; i<N_p_bits; i++)
      {
          p_bits[i] = (crc_rem >> (N_p_bits-1-i)) & 1;
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

