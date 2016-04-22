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
#include "rate_match_conv_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  rate_match_conv::sptr
  rate_match_conv::make(uint32 N_e_bits)
  {
    return gnuradio::get_initial_sptr
      (new rate_match_conv_impl(N_e_bits));
  }

  rate_match_conv_impl::rate_match_conv_impl(uint32 N_e_bits)
    : gr::block("rate_match_conv",
                gr::io_signature::make(1, 1, sizeof(uint8)),
            gr::io_signature::make(1, 1, sizeof(uint8))),
      N_out_bits(N_e_bits)
  {
      set_tag_propagation_policy(TPP_DONT);
      set_alignment(N_e_bits);
  }

  rate_match_conv_impl::~rate_match_conv_impl()
  {
  }

  void
  rate_match_conv_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = 120;        // FIXME: how to determine input numbers?
  }

  int
  rate_match_conv_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];

      // reading tags
      vector <tag_t> v;
      get_tags_in_window(v,
                         0,
                         0,
                         ninput_items[0],
                        pmt::string_to_symbol("length") );

      int packet_len = 0;
      int input_remained = 0, nout_written = 0;

      if (v.size()>0) // reading if available
          packet_len = pmt::to_long(v[0].value);  // reading one packet
      else
          throw runtime_error("No tags read");

      // checking availibity of the inputs
      if(packet_len <= ninput_items[0])
      {
          rate_match_conv(in,
                          packet_len,
                          N_out_bits,
                          out_bits);

          // checking the enough output buffer
          if(N_out_bits <= noutput_items)
          {

              memcpy(out, out_bits, N_out_bits * sizeof(uint8));
              nout_written = N_out_bits;
              input_remained = packet_len;

              // tagging output
              add_item_tag(0,
                           nitems_written(0),
                           pmt::string_to_symbol("length"),
                           pmt::from_long(N_out_bits));

//              if (nout_written == 1920)
//              {
//                  cout << "PBCH ======================================================" << endl;
//                  for (int i=0; i<1920; i++)
//                  {
//                      if (out[i] == 0x00)
//                      {
//                          cout << "0 ";
//                      }
//                      else
//                      {
//                          cout << "1 ";
//                      }
//                  }
//                  cout << endl;
//              }
              // cout<< "rate match conv" << endl;
          }
          else
          {
              cout << "out bits= " << N_out_bits << endl;
              cout << "N output items= " <<noutput_items<< endl;
//              throw runtime_error("Not enough output items");
          }
      }
      else
      {
          throw runtime_error("Not enough output items");
      }

      // we tell the runtime system every time we finish one loop!
      // Otherwise the data won't propogate through blocks
      consume_each (input_remained);

      return nout_written;

  }

  void rate_match_conv_impl::rate_match_conv(uint8 *d_bits,
                       uint32             N_d_bits,
                       uint32             N_e_bits,
                       uint8             *e_bits)
  {
      uint32 C_cc_sb = 32; // Step 1: Assign C_cc_sb to 32
      uint32 R_cc_sb;
      uint32 w_idx = 0;
      uint32 d_idx;
      uint32 N_dummy;
      uint32 idx;
      uint32 K_pi;
      uint32 K_w;
      uint32 i;
      uint32 j;
      uint32 k;
      uint32 x;

      // permutation indexes for column-wise interleaver
      uint8 IC_PERM_CC[32] = { 1,17, 9,25, 5,21,13,29, 3,19,11,27, 7,23,15,31,
                               0,16, 8,24, 4,20,12,28, 2,18,10,26, 6,22,14,30};

      // Sub-block interleaving
      // Step 2: Determine the number of rows
      R_cc_sb = 0;
      while((N_d_bits/3) > (C_cc_sb*R_cc_sb))
      {
          R_cc_sb++;
      }

      // Steps 3, 4, and 5
      for(x=0; x<3; x++)
      {
          // Step 3: Pack data into matrix and pad with dummy
          if((N_d_bits/3) < (C_cc_sb*R_cc_sb))
          {
              N_dummy = C_cc_sb*R_cc_sb - (N_d_bits/3);
          }else{
              N_dummy = 0;
          }
          for(i=0; i<N_dummy; i++)
          {
              rmc_tmp[i] = TX_NULL_BIT;
          }
          d_idx = 0;
          for(i=N_dummy; i<C_cc_sb*R_cc_sb; i++)
          {
              rmc_tmp[i] = d_bits[d_idx*3+x];
              d_idx++;
          }
          idx = 0;
          for(i=0; i<R_cc_sb; i++)
          {
              for(j=0; j<C_cc_sb; j++)
              {
                  rmc_sb_mat[i][j] = rmc_tmp[idx++];
              }
          }

          // Step 4: Inter-column permutation
          for(i=0; i<R_cc_sb; i++)
          {
              for(j=0; j<C_cc_sb; j++)
              {
                  rmc_sb_perm_mat[i][j] = rmc_sb_mat[i][IC_PERM_CC[j]];
              }
          }

          // Step 5: Read out the bits
          for(j=0; j<C_cc_sb; j++)
          {
              for(i=0; i<R_cc_sb; i++)
              {
                  rmc_w[w_idx++] = rmc_sb_perm_mat[i][j];
              }
          }
      }
      K_pi = R_cc_sb*C_cc_sb;

      // Bit collection, selection, and transmission
      // Create circular buffer
      K_w = 3*K_pi;
      k   = 0;
      j   = 0;
      while(k < N_e_bits)
      {
          if(rmc_w[j%K_w] != TX_NULL_BIT)
          {
              e_bits[k++] = rmc_w[j%K_w];
          }
          j++;
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

