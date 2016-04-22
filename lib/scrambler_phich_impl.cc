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
#include "scrambler_phich_impl.h"
#include <cmath>

namespace gr {
  namespace my_lte {

  scrambler_phich::sptr
  scrambler_phich::make(uint32 N_id_cell, short rnti, uint32 set_align)
  {
    return gnuradio::get_initial_sptr
      (new scrambler_phich_impl(N_id_cell, rnti, set_align));
  }

  scrambler_phich_impl::scrambler_phich_impl(uint32 initial, short rnti, uint32 set_align)
    : gr::sync_block("scrambler",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      N_id_cell(initial),
      RNTI(rnti)
  {
      count = 0;
      cinit = 0;

      // propogation tag policy, propogating through all outputs!
      set_tag_propagation_policy(TPP_ALL_TO_ALL);
      // aligning output buffer size
      set_alignment(set_align);
  }

  scrambler_phich_impl::~scrambler_phich_impl()
  {
  }

  int
  scrambler_phich_impl::work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items)
  {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      uint32 packet_len = 0, nout_written = 0;

      // reading tags
      vector<tag_t> all_tag;
      //        uint32 inp = nitems_read(0);
      get_tags_in_range(all_tag,
                        0,
                        nitems_read(0),
                        nitems_read(0)+noutput_items,
                        pmt::string_to_symbol("length"));

      if (all_tag.size()>0)
            packet_len = pmt::to_long(all_tag[0].value);
      else
      {
          throw runtime_error("No tags read");
      }

      uint32 scramb_seq[100000];    // enough large array

      // checking output buffer and input buffer simultaneousely
      if (packet_len <= noutput_items)
      {
          // at start of a subframe
          update_cinit(uint32 (count%20) );
          generate_prs_c(cinit, packet_len , scramb_seq);

          for (uint32 j=0; j<packet_len ; j++)
          {
              // for PHICH (see 3GPP TS 36.211 version 10.1.0 Release 10 section 6.9.1)
              out[j] = in[j] * (gr_complex(1)-gr_complex(2)*gr_complex(scramb_seq[j]));
//              cout <<"output of PHICH scrambler   " << out[j] << endl;
              nout_written++;

          }
//          cout << "PHICH scrambler" << endl; //<< all_tag.size()<<endl;
      }
      else
      {
          cout << "<scrambler phich Block> Not enough input/output items" << endl;
      }

      count++;                // next sub frame or frame (for PBCH)
      return nout_written;   // returing each packet in each subframe
  }

  void scrambler_phich_impl::generate_prs_c(uint32  c_init,
                      uint32  len,
                      uint32 *c)
  {
      uint32 i;
      uint32 x1;
      uint32 x2;
      uint8  new_bit1;
      uint8  new_bit2;

      // Initialize the 2nd m-sequence
      x2 = c_init;

      // Advance the 2nd m-sequence
      for(i=0; i<(1600-31); i++)
      {
          new_bit2 = ((x2 >> 3) ^ (x2 >> 2) ^ (x2 >> 1) ^ x2) & 0x1;

          x2 = (x2 >> 1) | (new_bit2 << 30);
      }

      // Initialize the 1st m-sequence
      x1 = 0x54D21B24; // This is the result of advancing the initial value of 0x00000001

      // Generate c
      for(i=0; i<len; i++)
      {
          new_bit1 = ((x1 >> 3) ^ x1) & 0x1;
          new_bit2 = ((x2 >> 3) ^ (x2 >> 2) ^ (x2 >> 1) ^ x2) & 0x1;

          x1 = (x1 >> 1) | (new_bit1 << 30);
          x2 = (x2 >> 1) | (new_bit2 << 30);

          c[i] = new_bit1 ^ new_bit2;
      }
  }

  void scrambler_phich_impl::update_cinit(uint32 sbfrm_num)
  {
          // PHICH channels
          cinit = (((sbfrm_num + 1)*(2*N_id_cell + 1)) << 9) + N_id_cell;

  }

  } /* namespace my_lte */
} /* namespace gr */

