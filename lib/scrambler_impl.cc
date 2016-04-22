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
#include "scrambler_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  scrambler::sptr
  scrambler::make(uint32 N_id_cell, string name, short rnti, uint32 set_align)
  {
    return gnuradio::get_initial_sptr
      (new scrambler_impl(N_id_cell, name, rnti, set_align));
  }

  scrambler_impl::scrambler_impl(uint32 initial, string name, short rnti, uint32 set_align)
    : gr::sync_block("scrambler",
                     gr::io_signature::make(1, 1, sizeof(uint8)),
                     gr::io_signature::make(1, 1, sizeof(uint8))),
      N_id_cell(initial),
      CHname(name),
      RNTI(rnti),
      init(!CHname.compare("PBCH"))  // determining the initialization period of the scrambler
  {
      count = 0;
      cinit = 0;

      // propogation tag policy, propogating through all outputs!
      set_tag_propagation_policy(TPP_ALL_TO_ALL);
      // aligning output buffer size
      set_alignment(set_align);
  }

  scrambler_impl::~scrambler_impl()
  {
  }

  int
  scrambler_impl::work(int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
  {
      const uint8 *in = (const uint8 *) input_items[0];
      uint8 *out = (uint8 *) output_items[0];

      uint32 packet_len = 0, nout_written = 0;

      // reading tags
      vector<tag_t> all_tag;
      get_tags_in_range(all_tag,
                        0,
                        nitems_read(0),
                        nitems_read(0)+noutput_items,
                        pmt::string_to_symbol("length"));

      if(all_tag.size() > 0)
            packet_len = pmt::to_long(all_tag[0].value);
      else
      {
          throw runtime_error("No tags read");
      }

      if (init)   // scrambler for PBCH
      {
          generate_prs_c(N_id_cell,packet_len , bch_scramb);
          // checking enough inputs/outputs
          if(packet_len + nout_written <= noutput_items)
          {
//              cout << "PBCH ============================================ with packet len=  " << packet_len << endl;
              for (uint32 j=0; j<packet_len ; j++)
              {
                  *out = *in ^ bch_scramb[j];

//                  if (*out == 0x00)
//                  {
//                    cout << "0 ";
//                  }
//                  else
//                  {
//                      cout << "1 ";
//                  }

                  in++;
                  out++;
                  nout_written++;
              }
//                cout << endl;

//              cout << "scrambler0" << endl;
          }
          else
          {
              cout << "packet len=  " << packet_len << endl;
              cout << "I/O items=  " << noutput_items << endl;
//              throw runtime_error("Not enough input/output items");
              cout << "<scrambler>: Not enough input/output items" << endl;
          }
      }
      else
      {
          // scrambling for PDSCH & DCI & PCFICH
          uint32 scramb_seq[100000];    // enough large array

          // checking output buffer and input buffer simultaneousely
          if (packet_len + nout_written <= noutput_items)
          {
              // at start of a subframe (count = subframe number)
              update_cinit(uint32 ((count)%20) );
              generate_prs_c(cinit,packet_len , scramb_seq);

              for (uint32 j=0; j<packet_len ; j++)
              {
                  out[j] = in[j] ^ scramb_seq[j];
                  nout_written++;

              }

//              if (!CHname.compare("PDSCH"))
//              {
//                  cout << "::::::::::::::::::::;PDSCH scrambled bits :::::::::::::::::::::  "
//                          << packet_len << endl;

//                  for (uint32 j=0; j<packet_len ; j++)
//                  {
//                      if (out[j] == 0x00)
//                          cout << "0 ";
//                      else
//                          cout << "1 ";
//                  }
//                  cout << endl;
//              }

//              cout << "scrambler1" << endl; //<< all_tag.size()<<endl;
          }
          else
          {
              cout << "packet len=  " << packet_len << endl;
              cout << "I/O items=  " << noutput_items << endl;
//              throw runtime_error("Not enough input/output items");
              cout << "<scrambler>: Not enough input/output items" << endl;
          }

      }

      count++;                // next sub frame or frame (for PBCH)
      return nout_written;   // returning each packet in each subframe
  }

  void scrambler_impl::generate_prs_c(uint32  c_init,
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

  void scrambler_impl::update_cinit(uint32 sbfrm_num)
  {
      if (!CHname.compare("PDSCH"))
      {
          // FIXME: Only handling 1 codeword
//          cinit = (RNTI << 14) | (0 << 13) | (sbfrm_num << 9) | N_id_cell;
          cinit = (RNTI << 14) | (0 << 13) | (0 << 9) | N_id_cell;
      }
      else if (!CHname.compare("PMCH"))
      {
          cinit = (sbfrm_num << 9) + N_id_cell;
      }
      else if (!CHname.compare("DCI") )
      {
          cinit = (sbfrm_num << 9) + N_id_cell;
      }
      else    // PCFICH
      {
          cinit = (((sbfrm_num + 1)*(2*N_id_cell + 1)) << 9) + N_id_cell;
      }

  }


  } /* namespace my_lte */
} /* namespace gr */

