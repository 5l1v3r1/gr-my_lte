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
#include "conv_enc_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  conv_enc::sptr
  conv_enc::make(uint32 constraint_length,
                 uint32 rate,
                 vector<uint32> g,
                 bool tail_bit)
  {
    return gnuradio::get_initial_sptr
      (new conv_enc_impl(constraint_length, rate, g, tail_bit));
  }

  conv_enc_impl::conv_enc_impl(uint32 constraint_length, uint32 rate, vector<uint32> g, bool tail_bit)
    : gr::sync_interpolator("conv_enc",
                     gr::io_signature::make(1, 1, sizeof(uint8)),
                     gr::io_signature::make(1, 1, sizeof(uint8)),
                            rate) ,
      constraint_len(constraint_length),
      rate(rate),
      matrix_gen(g),
      tail(tail_bit),
      d_pmt_id(pmt::string_to_symbol("conve_enc"))

  {
      set_tag_propagation_policy(TPP_DONT);
  }

  conv_enc_impl::~conv_enc_impl()
  {
  }

  int
  conv_enc_impl::work(int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];
      uint32 ninput =  floor(noutput_items / rate) ; // determinig the ninput_items

      // getting lengths of packets
      vector <tag_t> v;
      get_tags_in_range(v, 0, nitems_read(0), nitems_read(0) + int64_t (ninput) ,
                        pmt::string_to_symbol("length") );

      uint32 packet_len = 0;
      uint32 nout_written= 0, noutput_items_out = 0;

      if (v.size()>0)
          packet_len = pmt::to_long(v[0].value);  // reading one packet

      // we don't need to check output buffer becausse it's interpolator
      if (ninput>= packet_len)       // checking enough inputs
      {
          // encoding
          conv_encode(in,
                      packet_len,
                      constraint_len,
                      rate,
                      matrix_gen,
                      tail,
                      out_bits,
                      &noutput_items_out);

          memcpy(out, out_bits, noutput_items_out * sizeof(uint8));
          // outputs
          nout_written = noutput_items_out;

          // adding tags
          add_item_tag(0,
                       nitems_written(0),                       // the abs offset (because we put out each received packet
                       pmt::string_to_symbol("length"),
                       pmt::from_long(noutput_items_out),       // the length of outputs
                       d_pmt_id);                               // Id of this block

//          if (noutput_items_out == 120)
//          {
//              cout << "PBCH ======================================================" << endl;
//              for (int i=0; i<nout_written; i++)
//              {
//                  if (out[i] == 0x00)
//                  {
//                      cout << "0 ";
//                  }
//                  else
//                  {
//                      cout << "1 ";
//                  }
//              }
//              cout << endl;
//          }

//          cout << "conv encoder" << endl;
      }

      return nout_written;

  }


  void conv_enc_impl::conv_encode(uint8 *c_bits,
                   uint32             N_c_bits,
                   uint32             constrauint32_len,
                   uint32             rate,
                   vector<uint32>     g,
                   bool               tail_bit,
                   uint8             *d_bits,
                   uint32            *N_d_bits)
  {
      uint32 i;
      uint32 j;
      uint32 k;
      vector<uint8>  s_reg(constrauint32_len);
      uint8  *g_array[3];
      g_array[0] = new uint8 [constrauint32_len];
      g_array[1] = new uint8 [constrauint32_len];
      g_array[2] = new uint8 [constrauint32_len];

      // Initialize the shift register
      if(tail_bit)
      {
          for(i=0; i<constrauint32_len; i++)
          {
              s_reg[i] = c_bits[N_c_bits-i-1];
          }
      }else{
          for(i=0; i<constrauint32_len; i++)
          {
              s_reg[i] = 0;
          }
      }

      // Convert g from octal to binary array
      for(i=0; i<rate; i++)
      {
          for(j=0; j<constrauint32_len; j++)
          {
              g_array[i][j] = (g[i] >> (constrauint32_len-j-1)) & 1;
          }
      }

      // Convolutionally encode input
      for(i=0; i<N_c_bits; i++)
      {
          // Add next bit to shift register
          for(j=constrauint32_len-1; j>0; j--)
          {
              s_reg[j] = s_reg[j-1];
          }
          s_reg[0] = c_bits[i];

          // Determine the output bits
          for(j=0; j<rate; j++)
          {
              d_bits[i*rate + j] = 0;
              for(k=0; k<constrauint32_len; k++)
              {
                  d_bits[i*rate + j] += s_reg[k]*g_array[j][k];
              }
              d_bits[i*rate + j] %= 2;
          }
      }

      *N_d_bits = N_c_bits*rate;

      delete []g_array[0];
      delete []g_array[1];
      delete []g_array[2];

  }

  } /* namespace my_lte */
} /* namespace gr */

