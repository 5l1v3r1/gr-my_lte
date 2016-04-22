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
#include "code_block_concatenation_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  code_block_concatenation::sptr
  code_block_concatenation::make(uint32 N_code_block, uint32 N_e_bit_max, uint32 set_align)
  {
      return gnuradio::get_initial_sptr
              (new code_block_concatenation_impl(N_code_block, N_e_bit_max, set_align));;
  }

  code_block_concatenation_impl::code_block_concatenation_impl(uint32 N_code_block,
                                                               uint32 N_e_bit_max, uint32 set_align)
    : gr::block("code_block_concatenation",
            gr::io_signature::make(1, N_code_block, sizeof(uint8)),
            gr::io_signature::make(1, 1, sizeof(uint8))),
      N_code_block(N_code_block),
      N_e_bit_max(N_e_bit_max)
  {
      set_tag_propagation_policy(TPP_DONT);
      set_alignment(set_align);
  }

  code_block_concatenation_impl::~code_block_concatenation_impl()
  {
  }

  void
  code_block_concatenation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      // as an approximation
      for (int i = 0; i<N_code_block; i++)
          ninput_items_required[i] = 100;
  }

  int
  code_block_concatenation_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      uint8 *out = (uint8 *) output_items[0];

      uint8 **in = new uint8*[N_code_block];
      uint32 *packet_len = new uint32 [N_code_block];
      uint32 total_out = 0;
      vector<tag_t> all_tag;
      int n = 0;

      for(int i=0; i<N_code_block; i++)
      {
          in[i] = (uint8 *) input_items[i];
          packet_len[i] = 0;

          //reading tags from all inputs
          get_tags_in_window(all_tag,
                             i,
                             0,
                             ninput_items[i],
                             pmt::string_to_symbol("length"));

          if (all_tag.size()>0)
          {
              packet_len[i] = pmt::to_long(all_tag[0].value);
          }
          else
          {
              throw runtime_error("No tags read");
          }

          if(packet_len[i] <= ninput_items[i])
          {
              total_out += packet_len[i];
              n++;
          }
          else
          {
              throw runtime_error("Not enough input items");
          }
      }

      // enough output & all tags received
      if (noutput_items >= total_out & n==N_code_block)
      {
          uint32 idx = 0;
          for (int i=0; i<N_code_block; i++)
          {
              for (uint32 j=0; j<packet_len[i]; j++)
              {
                  out[idx] = in[i][j];
                  idx++;
              }

          }

          // add tag
          add_item_tag(0,
                       nitems_written(0),
                       pmt::string_to_symbol("length"),
                       pmt::from_long(total_out));

//          cout << "code block concatenation" << endl;
      }
      else
      {
          total_out = 0;
          for (int i=0 ; i<N_code_block; i++)
          {
              packet_len[i] = 0;
          }
          throw runtime_error("Not enough output items");
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      for (int i = 0; i<N_code_block; i++)
      {
          consume (i, packet_len[i]);
      }

      // releasing allocated memory //////////////////////
      delete []in;
      delete []packet_len;

      // Tell runtime system how many output items we produced.
      return total_out;
  }

  } /* namespace my_lte */
} /* namespace gr */

