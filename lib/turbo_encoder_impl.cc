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
#include "turbo_encoder_impl.h"
#include <vector>

// Turbo Internal Interleaver from 3GPP TS 36.212 v10.1.0 table 5.1.3-3
#define TURBO_INT_K_TABLE_SIZE 188
uint32 TURBO_INT_K_TABLE[188] = {  40,  48,  56,  64,  72,  80,  88,  96, 104, 112,
                                  120, 128, 136, 144, 152, 160, 168, 176, 184, 192,
                                  200, 208, 216, 224, 232, 240, 248, 256, 264, 272,
                                  280, 288, 296, 304, 312, 320, 328, 336, 344, 352,
                                  360, 368, 376, 384, 392, 400, 408, 416, 424, 432,
                                  440, 448, 456, 464, 472, 480, 488, 496, 504, 512,
                                  528, 544, 560, 576, 592, 608, 624, 640, 656, 672,
                                  688, 704, 720, 736, 752, 768, 784, 800, 816, 832,
                                  848, 864, 880, 896, 912, 928, 944, 960, 976, 992,
                                 1008,1024,1056,1088,1120,1152,1184,1216,1248,1280,
                                 1312,1344,1376,1408,1440,1472,1504,1536,1568,1600,
                                 1632,1664,1696,1728,1760,1792,1824,1856,1888,1920,
                                 1952,1984,2016,2048,2112,2176,2240,2304,2368,2432,
                                 2496,2560,2624,2688,2752,2816,2880,2944,3008,3072,
                                 3136,3200,3264,3328,3392,3456,3520,3584,3648,3712,
                                 3776,3840,3904,3968,4032,4096,4160,4224,4288,4352,
                                 4416,4480,4544,4608,4672,4736,4800,4864,4928,4992,
                                 5056,5120,5184,5248,5312,5376,5440,5504,5568,5632,
                                 5696,5760,5824,5888,5952,6016,6080,6144};
uint32 TURBO_INT_F1_TABLE[188] = {  3,  7, 19,  7,  7, 11,  5, 11,  7, 41,103, 15,  9,
                                   17,  9, 21,101, 21, 57, 23, 13, 27, 11, 27, 85, 29,
                                   33, 15, 17, 33,103, 19, 19, 37, 19, 21, 21,115,193,
                                   21,133, 81, 45, 23,243,151,155, 25, 51, 47, 91, 29,
                                   29,247, 29, 89, 91,157, 55, 31, 17, 35,227, 65, 19,
                                   37, 41, 39,185, 43, 21,155, 79,139, 23,217, 25, 17,
                                  127, 25,239, 17,137,215, 29, 15,147, 29, 59, 65, 55,
                                   31, 17,171, 67, 35, 19, 39, 19,199, 21,211, 21, 43,
                                  149, 45, 49, 71, 13, 17, 25,183, 55,127, 27, 29, 29,
                                   57, 45, 31, 59,185,113, 31, 17,171,209,253,367,265,
                                  181, 39, 27,127,143, 43, 29, 45,157, 47, 13,111,443,
                                   51, 51,451,257, 57,313,271,179,331,363,375,127, 31,
                                   33, 43, 33,477, 35,233,357,337, 37, 71, 71, 37, 39,
                                  127, 39, 39, 31,113, 41,251, 43, 21, 43, 45, 45,161,
                                   89,323, 47, 23, 47,263};
uint32 TURBO_INT_F2_TABLE[188] = { 10, 12, 42, 16, 18, 20, 22, 24, 26, 84, 90, 32, 34,
                                  108, 38,120, 84, 44, 46, 48, 50, 52, 36, 56, 58, 60,
                                   62, 32,198, 68,210, 36, 74, 76, 78,120, 82, 84, 86,
                                   44, 90, 46, 94, 48, 98, 40,102, 52,106, 72,110,168,
                                  114, 58,118,180,122, 62, 84, 64, 66, 68,420, 96, 74,
                                   76,234, 80, 82,252, 86, 44,120, 92, 94, 48, 98, 80,
                                  102, 52,106, 48,110,112,114, 58,118, 60,122,124, 84,
                                   64, 66,204,140, 72, 74, 76, 78,240, 82,252, 86, 88,
                                   60, 92,846, 48, 28, 80,102,104,954, 96,110,112,114,
                                  116,354,120,610,124,420, 64, 66,136,420,216,444,456,
                                  468, 80,164,504,172, 88,300, 92,188, 96, 28,240,204,
                                  104,212,192,220,336,228,232,236,120,244,248,168, 64,
                                  130,264,134,408,138,280,142,480,146,444,120,152,462,
                                  234,158, 80, 96,902,166,336,170, 86,174,176,178,120,
                                  182,184,186, 94,190,480};


namespace gr {
  namespace my_lte {

  turbo_encoder::sptr
  turbo_encoder::make()
  {
    return gnuradio::get_initial_sptr
      (new turbo_encoder_impl());
  }

  /*
   * The private constructor
   */
  turbo_encoder_impl::turbo_encoder_impl()
    : gr::block("turbo_encoder",
            gr::io_signature::make(1, 1, sizeof(uint8)),
            gr::io_signature::make(1, 1, sizeof(uint8)))
  {
      set_tag_propagation_policy(TPP_DONT);
      set_alignment(20000);
  }

  turbo_encoder_impl::~turbo_encoder_impl()
  {
  }

  void
  turbo_encoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = noutput_items/3;
  }

  int
  turbo_encoder_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];

      uint32 packet_len = 0;
      N_filler_bits = 0;
      N_d_bits = 0;
      vector<tag_t> tag_len, tag_filler_bit;

      get_tags_in_window(tag_len,
                         0,
                         0,
                         ninput_items[0],
              pmt::string_to_symbol("length"));
      get_tags_in_window(tag_filler_bit,
                         0,
                         0,
                         ninput_items[0],
              pmt::string_to_symbol("N_filler_bits"));

      if(tag_len.size()>0 & tag_filler_bit.size()>0)
      {
          packet_len = pmt::to_long(tag_len[0].value);
          N_filler_bits = pmt::to_long(tag_filler_bit[0].value);

          // checking enough input
          if(packet_len <= ninput_items[0])
          {
              turbo_encode(in,
                           packet_len,
                           N_filler_bits,
                           d_bits,
                           &N_d_bits);
              // checking enough output
              if (N_d_bits <= noutput_items)
              {
                  for(uint32 i=0; i<N_d_bits; i++)
                  {
                      out[i] = d_bits[i];
                  }

                  // adding tags
                  add_item_tag(0,
                               nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(N_d_bits));

                  //cout << "turbo encoder" << endl;

//                  cout << "::::::::::::::::::::;PDSCH output of Turbo bits :::::::::::::::::::::  "
//                       << N_d_bits << endl;

//                  for (uint32 j=0; j<packet_len ; j++)
//                  {
//                      if (in[j] == 0x00)
//                          cout << "0 ";
//                      else
//                          cout << "1 ";
//                  }
//                  cout << endl;

              }
              else
              {
                  throw runtime_error("Not enough output items");
              }

          }
          else    // possible state when we read tags but we don't have enough input!
          {
              packet_len = 0;
              throw runtime_error("Not enough input items");
          }

      }
      else
      {
          throw runtime_error("No tags read");
      }

      consume_each (packet_len);

      return N_d_bits;
  }

  void turbo_encoder_impl::turbo_encode(uint8             *c_bits,
                                        uint32             N_c_bits,
                                        uint32             N_fill_bits,
                                        uint8             *d_bits,
                                        uint32            *N_d_bits)
  {
      uint32 i;
      uint32 N_branch_bits = N_c_bits + 4;

      // Turbo encode
      uint8 te_z[6244];
      uint8 te_fb1[6244];
      uint8 te_c_prime[6244];
      uint8 te_z_prime[6244];
      uint8 te_x_prime[6244];

      //counting filler bits and add zeros instead of <NULL> to the c_bits
      // based on 3GPP Ts 36.212 version 10.1.0 section 5.1.3.2.1
      for (uint32 i=0; i<N_fill_bits; i++)
      {
          c_bits[i] = 0;
      }

      // Construct z
      turbo_constituent_encoder(c_bits,
                                N_c_bits,
                                te_z,
                                te_fb1);

      // Construct c_prime
      turbo_internal_interleaver(c_bits,
                                 N_c_bits,
                                 te_c_prime);

      // Construct z_prime
      turbo_constituent_encoder(te_c_prime,
                                N_c_bits,
                                te_z_prime,
                                te_x_prime);

      // Construct d_bits
      for(i=0; i<N_c_bits; i++)
      {
          //accounting filler bits !!
  //        if (N_fill_bits > 0 & i<N_fill_bits)
  //        {
  //            d_bits[i]                   = NULL;
  //            d_bits[N_branch_bits+i]     = NULL;
  //            d_bits[2*N_branch_bits+i]   = phy_struct->te_z_prime[i];
  //        }else {
              d_bits[i]                 = c_bits[i];
              d_bits[N_branch_bits+i]   = te_z[i];
              d_bits[2*N_branch_bits+i] = te_z_prime[i];
  //        }
      }
      d_bits[N_c_bits]                   = te_fb1[N_c_bits-1];
      d_bits[N_c_bits+1]                 = te_z[N_c_bits+1];
      d_bits[N_c_bits+2]                 = te_x_prime[N_c_bits];
      d_bits[N_c_bits+3]                 = te_z_prime[N_c_bits+1];
      d_bits[N_branch_bits+N_c_bits]     = te_z[N_c_bits];
      d_bits[N_branch_bits+N_c_bits+1]   = te_fb1[N_c_bits+1];
      d_bits[N_branch_bits+N_c_bits+2]   = te_z_prime[N_c_bits];
      d_bits[N_branch_bits+N_c_bits+3]   = te_x_prime[N_c_bits+2];
      d_bits[2*N_branch_bits+N_c_bits]   = te_fb1[N_c_bits];
      d_bits[2*N_branch_bits+N_c_bits+1] = te_z[N_c_bits+1];
      d_bits[2*N_branch_bits+N_c_bits+2] = te_x_prime[N_c_bits+1];
      d_bits[2*N_branch_bits+N_c_bits+3] = te_z_prime[N_c_bits+2];

      // accounting filler bits
      for (uint32 i=0; i<N_fill_bits; i++)
      {
          d_bits[i]                 = TX_NULL_BIT;
          d_bits[N_branch_bits+i]   = TX_NULL_BIT;
      }

      *N_d_bits = N_branch_bits*3;
  }

  void turbo_encoder_impl::turbo_constituent_encoder(uint8  *in_bits,
                                                     uint32  N_in_bits,
                                                     uint8  *out_bits,
                                                     uint8  *fb_bits)
  {
      uint32 i;
      uint32 j;
      uint32 constraint_len = 4;
      uint8  s_reg[4]    = {0,0,0,0};
      uint8  g_array[4]  = {1,1,0,1};
      uint8  fb_array[4] = {0,0,1,1};

      // Convolutionally encode input
      for(i=0; i<N_in_bits; i++)
      {
          // Sequence the shift register
          for(j=constraint_len-1; j>0; j--)
          {
              s_reg[j] = s_reg[j-1];
          }

          // Calculate the feedback bit
          fb_bits[i] = 0;
          for(j=0; j<constraint_len; j++)
          {
              fb_bits[i] += s_reg[j]*fb_array[j];
          }
          fb_bits[i] %= 2;

          // Add the next bit to the shift register
          s_reg[0] = (fb_bits[i] + in_bits[i]) % 2;

          // Calculate the output bit
          out_bits[i] = 0;
          for(j=0; j<constraint_len; j++)
          {
              out_bits[i] += s_reg[j]*g_array[j];
          }
          out_bits[i] %= 2;
      }

      // Trellis termination
      for(i=N_in_bits; i<N_in_bits+4; i++)
      {
          // Sequence the shift register
          for(j=constraint_len-1; j>0; j--)
          {
              s_reg[j] = s_reg[j-1];
          }

          // Calculate the feedback bit
          fb_bits[i] = 0;
          for(j=0; j<constraint_len; j++)
          {
              fb_bits[i] += s_reg[j]*fb_array[j];
          }
          fb_bits[i] %= 2;

          // Add the next bit to the shift register
          s_reg[0] = (fb_bits[i] + fb_bits[i]) % 2;

          // Calculate the output bit
          out_bits[i] = 0;
          for(j=0; j<constraint_len; j++)
          {
              out_bits[i] += s_reg[j]*g_array[j];
          }
          out_bits[i] %= 2;
      }
  }

  ///////////////////////// Turbo internal interleavers ///////////////////////

  void turbo_encoder_impl::turbo_internal_interleaver(uint8  *in_bits,
                                  uint32  N_in_bits,
                                  uint8  *out_bits)
  {
      uint32 i;
      uint32 f1 = 0;
      uint32 f2 = 0;
      uint32 idx;

      // Determine f1 and f2
      for(i=0; i<TURBO_INT_K_TABLE_SIZE; i++)
      {
          if(N_in_bits == TURBO_INT_K_TABLE[i])
          {
              f1 = TURBO_INT_F1_TABLE[i];
              f2 = TURBO_INT_F2_TABLE[i];
              break;
          }
      }

      for(i=0; i<N_in_bits; i++)
      {
          idx         = (f1*i + f2*i*i) % N_in_bits;
          out_bits[i] = in_bits[idx];
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

