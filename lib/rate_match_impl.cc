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
#include "rate_match_impl.h"
#include <vector>

uint8 IC_PERM_TC[32] = { 0,16, 8,24, 4,20,12,28, 2,18,10,26, 6,22,14,30,
                         1,17, 9,25, 5,21,13,29, 3,19,11,27, 7,23,15,31};

namespace gr {
  namespace my_lte {

  rate_match::sptr
  rate_match::make(uint32 N_e_bits,
                   uint32 N_code_block,
                   int tx_mode,
                   uint32 N_soft,
                   int M_dl_harq,
                   int rv_idx,
                   string chan_name)
  {
    return gnuradio::get_initial_sptr
      (new rate_match_impl(N_e_bits, N_code_block, tx_mode, N_soft, M_dl_harq, rv_idx, chan_name));
  }

  rate_match_impl::rate_match_impl(uint32 N_e_bits,
                                   uint32 N_code_block,
                                   int tx_mode,
                                   uint32 N_soft,
                                   int M_dl_harq,
                                   int rv_idx,
                                   string chan_name)
    : gr::block("rate_match",
            gr::io_signature::make(1, 1, sizeof(uint8)),
            gr::io_signature::make(1, 1, sizeof(uint8))),
      d_N_e_bits(N_e_bits),
      d_N_code_block(N_code_block),
      d_tx_mode(tx_mode),
      d_N_soft(N_soft),
      d_M_dl_harq(M_dl_harq),
      d_rv_idx(rv_idx),
      channel_name(chan_name)
  {
      set_tag_propagation_policy(TPP_DONT);
      set_alignment(17676);
  }

  rate_match_impl::~rate_match_impl()
  {
  }

  void
  rate_match_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = 17600;
  }

  int
  rate_match_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];

      uint32 packet_len = 0, nout_written = 0;
      vector<tag_t> all_tag;

      get_tags_in_window(all_tag,
                         0,
                         0,
                         ninput_items[0],
                         pmt::string_to_symbol("length"));

      if (all_tag.size()>0)
      {
          packet_len = pmt::to_long(all_tag[0].value);
      }
      else
      {
          cout<< "N input items:  " << ninput_items[0] << endl;
          throw runtime_error("No tags read");
      }

      if (packet_len <= ninput_items[0] & packet_len != 0)
      {
          // Doing rate match
          rate_match_turbo(in,
                           packet_len,
                           d_N_code_block,
                           d_tx_mode,
                           d_N_soft,
                           d_M_dl_harq,
                           channel_name,
                           d_rv_idx,
                           d_N_e_bits,
                           e_bits);

          // checking output buffer
          if(d_N_e_bits <= noutput_items)
          {
              for(uint32 i=0; i<d_N_e_bits; i++)
              {
                  out[i] = e_bits[i];
              }
              //adding tags
              add_item_tag(0,
                           nitems_written(0),
                           pmt::string_to_symbol("length"),
                           pmt::from_long(d_N_e_bits));
              //              cout << "rate match" << endl;
              nout_written = d_N_e_bits;

//              cout << "::::::::::::::::::::;PDSCH output of Turbo bits :::::::::::::::::::::  "
//                   << packet_len << endl;

//              for (uint32 j=0; j<packet_len ; j++)
//              {
//                  if (in[j] == 0x00)
//                      cout << "0 ";
//                  else
//                      cout << "1 ";
//              }
//              cout << endl;
          }
          else
          {
              cout << "packet len=  " << packet_len << endl;
              cout << "Output items=  " << noutput_items << endl;
              throw runtime_error("Not enough output items");
          }

      }
      else
      {
          packet_len = 0;
          throw runtime_error("Not enough input items");
      }

      consume_each (packet_len);

      // Tell runtime system how many output items we produced.
      return nout_written;
  }

  void rate_match_impl::rate_match_turbo(uint8                     *d_bits,
                                         uint32                     N_d_bits,
                                         uint32                     N_codeblocks,
                                         uint32                     tx_mode,
                                         uint32                     N_soft,
                                         uint32                     M_dl_harq,
                                         string                     chan_name,
                                         uint32                     rv_idx,
                                         uint32                     N_e_bits,
                                         uint8                     *e_bits)
  {
      uint32 C_tc_sb = 32; // Step 1: Assign C_tc_sb to 32
      uint32 R_tc_sb;
      uint32 w_idx = 0;
      uint32 d_idx;
      uint32 pi_idx;
      uint32 N_dummy;
      uint32 K_mimo;
      uint32 N_ir;
      uint32 N_cb;
      uint32 idx;
      uint32 K_pi;
      uint32 K_w;
      uint32 k_0;
      uint32 i;
      uint32 j;
      uint32 k;
      uint32 x;

      // Rate Match Turbo
      uint8 rmt_tmp[6144];
      uint8 rmt_sb_mat[192][32];
      uint8 rmt_sb_perm_mat[192][32];
      uint8 rmt_y[6144];
      uint8 rmt_w[18432];

      // Sub-block interleaving
      // Step 2: Determine the number of rows
      R_tc_sb = 0;
      while((N_d_bits/3) > (C_tc_sb*R_tc_sb))
      {
          R_tc_sb++;
      }

      // Steps 3, 4, and 5
      for(x=0; x<3; x++)
      {
          // Step 3: Pack data into matrix and pad with dummy
          if((N_d_bits/3) < (C_tc_sb*R_tc_sb))
          {
              N_dummy = C_tc_sb*R_tc_sb - (N_d_bits/3);
          }else{
              N_dummy = 0;
          }
          for(i=0; i<N_dummy; i++)
          {
               rmt_tmp[i] = TX_NULL_BIT;
          }
          d_idx = 0;
          for(i=N_dummy; i<C_tc_sb*R_tc_sb; i++)
          {
               rmt_tmp[i] = d_bits[(N_d_bits/3)*x+d_idx];
              d_idx++;
          }
          idx = 0;
          for(i=0; i<R_tc_sb; i++)
          {
              for(j=0; j<C_tc_sb; j++)
              {
                   rmt_sb_mat[i][j] =  rmt_tmp[idx++];
              }
          }

          w_idx = 0;
          if(x != 2)
          {
              // Step 4: Inter-column permutation
              for(i=0; i<R_tc_sb; i++)
              {
                  for(j=0; j<C_tc_sb; j++)
                  {
                       rmt_sb_perm_mat[i][j] =  rmt_sb_mat[i][IC_PERM_TC[j]];
                  }
              }
              // Step 5: Read out the bits
              idx  = 0;
              K_pi = R_tc_sb*C_tc_sb;
              for(j=0; j<C_tc_sb; j++)
              {
                  for(i=0; i<R_tc_sb; i++)
                  {
                      if(x == 0)
                      {
                           rmt_w[w_idx++] =  rmt_sb_perm_mat[i][j];
                      }else{
                           rmt_w[K_pi+(2*w_idx)] =  rmt_sb_perm_mat[i][j];
                          w_idx++;
                      }
                  }
              }
          }else{
              // Step 4: Permutation for the last output
              K_pi = R_tc_sb*C_tc_sb;
              idx  = 0;
              for(i=0; i<R_tc_sb; i++)
              {
                  for(j=0; j<C_tc_sb; j++)
                  {
                       rmt_y[idx++] =  rmt_sb_mat[i][j];
                  }
              }
              for(i=0; i<K_pi; i++)
              {
                  pi_idx                              = (IC_PERM_TC[i/R_tc_sb]+C_tc_sb*(i%R_tc_sb)+1) % K_pi;
                   rmt_w[K_pi+(2*w_idx)+1] =  rmt_y[pi_idx];
                  w_idx++;
              }
          }
      }

      // Bit collection, selection, and transmission
      // Create circular buffer
      K_w = 3*K_pi;
      if(tx_mode == 3 ||
         tx_mode == 4 ||
         tx_mode == 8 ||
         tx_mode == 9)
      {
          K_mimo = 2;
      }else{
          K_mimo = 1;
      }
      if(M_dl_harq < 8)
      {
          N_ir = N_soft/(K_mimo*M_dl_harq);
      }else{
          N_ir = N_soft/(K_mimo*8);
      }

      if(!chan_name.compare("PDSCH"))
      {
          if((N_ir/N_codeblocks) < K_w)
          {
              N_cb = N_ir/N_codeblocks;
          }else{
              N_cb = K_w;
          }
      }else if (!chan_name.compare("PMCH")){
          N_cb = K_w;
      }else{
          cout << "The type of channel must be 'PDSCH' or 'PMCH' " << endl;
      }

      k_0 = R_tc_sb*(2*(uint32)ceilf((float)N_cb/(float)(8*R_tc_sb))*rv_idx+2);
      k   = 0;
      j   = 0;
      while(k < N_e_bits)
      {
          if( rmt_w[(k_0+j)%N_cb] != TX_NULL_BIT)
          {
              e_bits[k++] =  rmt_w[(k_0+j)%N_cb];
          }
          j++;
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

