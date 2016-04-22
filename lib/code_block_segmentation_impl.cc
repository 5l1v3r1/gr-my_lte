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
#include "code_block_segmentation_impl.h"
#include <vector>

#define TURBO_INT_K_TABLE_SIZE 188

namespace gr {
  namespace my_lte {

  code_block_segmentation::sptr
  code_block_segmentation::make()
  {
    return gnuradio::get_initial_sptr
      (new code_block_segmentation_impl());
  }

  code_block_segmentation_impl::code_block_segmentation_impl()
    : gr::block("code_block_segmentation",
            gr::io_signature::make(1, 1, sizeof(uint8)),
                gr::io_signature::make(1, 1, sizeof(uint8)))
  {
      set_tag_propagation_policy(TPP_DONT);
      set_alignment(8000);
  }

  /*
   * Our virtual destructor.
   */
  code_block_segmentation_impl::~code_block_segmentation_impl()
  {
  }

  void
  code_block_segmentation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = 2 * noutput_items;     // Just as a role of thumb
  }

  int
  code_block_segmentation_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      uint8 *out[3];
      out[0] = (uint8 *) output_items[0];
//      out[1] = (uint8 *) output_items[1];
//      out[2] = (uint8 *) output_items[2];

      uint32 packet_len = 0;
      vector<tag_t> all_tag;

      // reading one packet at a time
      get_tags_in_window(all_tag, 0, 0, ninput_items[0],pmt::string_to_symbol("length"));

      if (all_tag.size()>0)
      {
          packet_len = pmt::to_long(all_tag[0].value);
      }

      // checking enough input
      if (packet_len <= ninput_items[0] & packet_len != 0)
      {
//          cout << "=================== input bits  " << packet_len <<endl;
//          for(int a=0; a<packet_len; a++)
//          {
//              if (in[a] == 0x00)
//              {
//                  cout << "0 ";
//              }
//              else
//              {
//                  cout << "1 ";
//              }
//          }
//          cout << endl;

          code_blk_segmentation(in,
                                packet_len,
                                &N_code_block,
                                &N_filler_bits,
                                c_bits,
                                N_c_bits);

          // producing outputs
          for (int i =0; i<N_code_block; i++)
          {
              // checking enough output
              if (noutput_items >= N_c_bits[i])
              {
                  for (int j=0; j < N_c_bits[i]; j++)
                  {
                      out[i][j] = c_bits[i*N_C_BIT_MAX+j];
                  }

                  // adding tags
                  add_item_tag(i, nitems_written(i),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(N_c_bits[i]));

                  // tagging the number of filler bits just for first code block
                  if (i == 0)
                  {
                      add_item_tag(i, nitems_written(i),
                                   pmt::string_to_symbol("N_filler_bits"),
                                   pmt::from_long(N_filler_bits));
                  }
                  else
                  {
                      add_item_tag(i, nitems_written(i),
                                   pmt::string_to_symbol("N_filler_bits"),
                                   pmt::from_long(0));
                  }

//                  cout << "::::::::::::::::::::;PDSCH output of seg bits :::::::::::::::::::::  "
//                       << N_c_bits[i] << endl;

//                  for (uint32 g=0; g<N_c_bits[i] ; g++)
//                  {
//                      if (out[i][g] == 0x00)
//                          cout << "0 ";
//                      else
//                          cout << "1 ";
//                  }
//                  cout << endl;

                  // produce output
                  produce(i, N_c_bits[i]);

//                  cout << "code block segmentation" << endl;
              }
              else
              {
                  throw runtime_error("Not enough output items");
              }
          }
      }
      else
      {
          packet_len = 0;
          throw runtime_error("Not enough input items");
      }

      consume_each (packet_len);

      // FIXME: We assume that  all the codeblocks have the same length (in general they aren't)
      return WORK_CALLED_PRODUCE;
  }

  void code_block_segmentation_impl::code_blk_segmentation(uint8  *b_bits,
                                                      uint32  N_b_bits,
                                                      uint32 *N_codeblocks,
                                                      uint32 *N_filler_bits,
                                                      uint8 *c_bits,
                                                      uint32 *N_c_bits)
  {
      uint32 Z = 6144;
      uint32 L;
      uint32 B;
      uint32 C;
      uint32 F;
      uint32 B_prime;
      uint32 K_plus  = 0;
      uint32 K_minus = 0;
      uint32 K_delta;
      uint32 K_r = 0;
      uint32 C_plus;
      uint32 C_minus;
      uint32 k;
      uint32 s;
      uint32 r;
      int32  i;
      uint8  p_cb_bits[24];

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

      // Determine L, C, B', K+, C+, K-, and C-
      if(N_b_bits <= Z)
      {
          L       = 0;
          C       = 1;
          B_prime = N_b_bits;
          for(i=0; i<TURBO_INT_K_TABLE_SIZE; i++){
              if(C*TURBO_INT_K_TABLE[i] >= B_prime)
              {
                  K_plus = TURBO_INT_K_TABLE[i];
                  break;
              }
          }
          K_minus = 0;
          C_plus  = 1;
          C_minus = 0;
      }else{
          B       = N_b_bits;
          L       = 24;
          C       = (uint32)ceilf((float)B/(float)(Z-L));
          B_prime = B + C*L;
         for(i=0; i<TURBO_INT_K_TABLE_SIZE; i++)
          {
              if(C*TURBO_INT_K_TABLE[i] >= B_prime)
              {
                  K_plus = TURBO_INT_K_TABLE[i];
                  break;
              }
          }
          for(i=TURBO_INT_K_TABLE_SIZE-1; i>=0; i--)
          {
              if(TURBO_INT_K_TABLE[i] < K_plus)
              {
                  K_minus = TURBO_INT_K_TABLE[i];
                  break;
              }
          }
          K_delta = K_plus - K_minus;
          C_minus = (C*K_plus - B_prime)/K_delta;
          C_plus  = C - C_minus;
      }

      //// Determine the number of filler bits and add them
      *N_codeblocks  = C;
      F              = C_plus*K_plus + C_minus*K_minus - B_prime;
      *N_filler_bits = F;
      for(i=0; i<(int32)F; i++)
      {
          c_bits[i] = TX_NULL_SYMB ;
      }

      //// Add the input bits
      k = F;
      s = 0;
      for(r=0; r<C; r++)
      {
      //    // Determine the K for this code block
          if(r < C_minus)
          {
              K_r = K_minus;
          }else{
              K_r = K_plus;
          }
          N_c_bits[r] = K_r -L ;

          // Add the input bits
          while(k < K_r - L)
          {
              c_bits[r*N_C_BIT_MAX+k] = b_bits[s++];
              k++;
          }

          // Add CRC if more than 1 code block is needed
          if(C > 1)
          {
              calc_crc(&c_bits[r*N_C_BIT_MAX], N_c_bits[r], CRC24B, p_cb_bits, L);

              N_c_bits[r] += L;
              while(k < K_r)
              {
                  c_bits[r*N_C_BIT_MAX+k] = p_cb_bits[k+L-K_r];
                  k++;
              }

          }
          k = 0;

      }

  }

  void code_block_segmentation_impl::calc_crc(uint8 *a_bits,
                gr_uint32  N_a_bits,
                gr_uint32  crc,
                uint8  *p_bits,
                gr_uint32  N_p_bits)
  {
      gr_uint32 i;
      gr_uint32 crc_rem   = 0;
      gr_uint32 crc_check = (1 << N_p_bits);
      uint8  *tmp_array = new uint8 [N_a_bits + N_p_bits];

      // Initialize tmp_array
      memset(tmp_array, 0, N_a_bits + N_p_bits);
      memcpy(tmp_array, a_bits, N_a_bits);

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

      delete []tmp_array;

  }

  } /* namespace my_lte */
} /* namespace gr */

