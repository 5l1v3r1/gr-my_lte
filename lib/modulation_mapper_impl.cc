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
#include "modulation_mapper_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  modulation_mapper::sptr
  modulation_mapper::make(string mod_type, uint32 set_align)
  {
    return gnuradio::get_initial_sptr
      (new modulation_mapper_impl(mod_type, set_align));
  }

  modulation_mapper_impl::modulation_mapper_impl(string mod_t, uint32 set_align)
    : gr::sync_decimator("modulation_mapper",
            gr::io_signature::make(1, 1, sizeof(uint8)),
            gr::io_signature::make(1, 1, sizeof(gr_complex)), mod_type_det(mod_t))
  {
      set_tag_propagation_policy(TPP_DONT);

      set_alignment(set_align);     // tell the scheduler about how many outputs is needed!
                                    //(used for large item passing)
  }

  uint32 modulation_mapper_impl::mod_type_det(string mod_t)
  {
      // determining modulation type
      if (!mod_t.compare("BPSK"))
      {
          modulation_t = LIBLTE_PHY_MODULATION_TYPE_BPSK;
          samps_per_symb = 1;
      }
      else if (!mod_t.compare("QPSK"))
      {
          modulation_t = LIBLTE_PHY_MODULATION_TYPE_QPSK;
          samps_per_symb = 2;
      }
      else if (!mod_t.compare("16QAM"))
      {
          modulation_t = LIBLTE_PHY_MODULATION_TYPE_16QAM;
          samps_per_symb = 4;
      }
      else if (!mod_t.compare("64QAM"))
      {
          modulation_t = LIBLTE_PHY_MODULATION_TYPE_64QAM;
          samps_per_symb = 6;
      }

      return samps_per_symb;
  }

  modulation_mapper_impl::~modulation_mapper_impl()
  {
  }

  int
  modulation_mapper_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
  {
      uint8 *in = (uint8 *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int ninput_items = (int)noutput_items*(int)samps_per_symb;
      // read tags
      vector<tag_t> all_tag;
      get_tags_in_window(all_tag,
                        0,
                        0,
                        ninput_items,
                        pmt::string_to_symbol("length"));

      uint32 N_bits = 0, M_symb = 0, nout_written = 0;

      if (all_tag.size()>0)
      {
          N_bits = pmt::to_long(all_tag[0].value);    // reading one packet
      }
      else
      {
          cout << "n input items: " << ninput_items << endl;
          cout << "samps per second " << samps_per_symb << endl;
          throw runtime_error("No tags read") ;
      }

      float *d_re;
      float *d_im;
      // checking enough input for tag "length"
      if (N_bits <= ninput_items)
      {
          d_re = new float [N_bits/samps_per_symb];
          d_im = new float [N_bits/samps_per_symb];

          // modulation ...
          modulation_mapper(in,
                            N_bits,
                            modulation_t,
                            d_re,
                            d_im,
                            &M_symb);

          // checking output buffer
          if (M_symb <= noutput_items)
          {
              // casting real and imaginary parts to gr_complex
              symb_to_gr_complex(d_re, d_im, M_symb, out);

              // adding tag ...
              add_item_tag(0,
                           nitems_written(0),
                           pmt::string_to_symbol("length"),
                           pmt::from_long(M_symb));
              nout_written = M_symb;

              // printing pbch
//              if (M_symb == 960)
//              {
//                  cout << "PBCH ======================================================" << endl;
//                  for (int i=0; i<960; i++)
//                  {
//                        cout << out[i] << endl;
//                  }
//              }

          }
          else
          {
              N_bits = 0;
              throw runtime_error("Not enough output items");
          }

          delete [] d_re;
          delete [] d_im;
//          cout << "modulation" << endl;
      }
      else
      {
//          cout << "Packet len: " << N_bits << "  input items: " << ninput_items << endl;
//          throw runtime_error("Not enough input items");
      }

      return nout_written;
  }

      void modulation_mapper_impl::modulation_mapper(uint8 *bits,
                             uint32                           N_bits,
                             LIBLTE_PHY_MODULATION_TYPE_ENUM  type,
                             float                           *d_re,
                             float                           *d_im,
                             uint32                          *M_symb)
      {
          float  one_over_sqrt_2  = 1/sqrt(2);
          float  one_over_sqrt_10 = 1/sqrt(10);
          float  one_over_sqrt_42 = 1/sqrt(42);
          uint32 i;
          uint32 input;

          switch(type)
          {
          case LIBLTE_PHY_MODULATION_TYPE_BPSK:
              // 3GPP TS 36.211 v10.1.0 section 7.1.1
              for(i=0; i<N_bits; i++)
              {
                  if(0 == bits[i])
                  {
                      d_re[i] = one_over_sqrt_2;
                      d_im[i] = one_over_sqrt_2;
                  }else{
                      d_re[i] = -one_over_sqrt_2;
                      d_im[i] = -one_over_sqrt_2;
                  }
              }
              *M_symb = N_bits;
              break;
          case LIBLTE_PHY_MODULATION_TYPE_QPSK:
              // 3GPP TS 36.211 v10.1.0 section 7.1.2
              for(i=0; i<(N_bits/2); i++)
              {
                  switch((bits[i*2] << 1) |
                         bits[i*2+1])
                  {
                  case 0:
                      d_re[i] = +one_over_sqrt_2;
                      d_im[i] = +one_over_sqrt_2;
                      break;
                  case 1:
                      d_re[i] = +one_over_sqrt_2;
                      d_im[i] = -one_over_sqrt_2;
                      break;
                  case 2:
                      d_re[i] = -one_over_sqrt_2;
                      d_im[i] = +one_over_sqrt_2;
                      break;
                  case 3:
                      d_re[i] = -one_over_sqrt_2;
                      d_im[i] = -one_over_sqrt_2;
                      break;
                  }
              }
              *M_symb = (N_bits/2);
              if((N_bits % 2) != 0)
              {
                  *M_symb = (N_bits/2) + 1;
                  // Add a trailing zero
                  if(0 == bits[N_bits-1])
                  {
                      d_re[i] = +one_over_sqrt_2;
                      d_im[i] = +one_over_sqrt_2;
                  }else{
                      d_re[i] = -one_over_sqrt_2;
                      d_im[i] = +one_over_sqrt_2;
                  }
              }
              break;
          case LIBLTE_PHY_MODULATION_TYPE_16QAM:
              // 3GPP TS 36.211 v10.1.0 section 7.1.3
              for(i=0; i<(N_bits/4); i++)
              {
                  switch((bits[i*4+0] << 3) |
                         (bits[i*4+1] << 2) |
                         (bits[i*4+2] << 1) |
                         bits[i*4+3])
                  {
                  case 0:
                      d_re[i] = +1*one_over_sqrt_10;
                      d_im[i] = +1*one_over_sqrt_10;
                      break;
                  case 1:
                      d_re[i] = +1*one_over_sqrt_10;
                      d_im[i] = +3*one_over_sqrt_10;
                      break;
                  case 2:
                      d_re[i] = +3*one_over_sqrt_10;
                      d_im[i] = +1*one_over_sqrt_10;
                      break;
                  case 3:
                      d_re[i] = +3*one_over_sqrt_10;
                      d_im[i] = +3*one_over_sqrt_10;
                      break;
                  case 4:
                      d_re[i] = +1*one_over_sqrt_10;
                      d_im[i] = -1*one_over_sqrt_10;
                      break;
                  case 5:
                      d_re[i] = +1*one_over_sqrt_10;
                      d_im[i] = -3*one_over_sqrt_10;
                      break;
                  case 6:
                      d_re[i] = +3*one_over_sqrt_10;
                      d_im[i] = -1*one_over_sqrt_10;
                      break;
                  case 7:
                      d_re[i] = +3*one_over_sqrt_10;
                      d_im[i] = -3*one_over_sqrt_10;
                      break;
                  case 8:
                      d_re[i] = -1*one_over_sqrt_10;
                      d_im[i] = +1*one_over_sqrt_10;
                      break;
                  case 9:
                      d_re[i] = -1*one_over_sqrt_10;
                      d_im[i] = +3*one_over_sqrt_10;
                      break;
                  case 10:
                      d_re[i] = -3*one_over_sqrt_10;
                      d_im[i] = +1*one_over_sqrt_10;
                      break;
                  case 11:
                      d_re[i] = -3*one_over_sqrt_10;
                      d_im[i] = +3*one_over_sqrt_10;
                      break;
                  case 12:
                      d_re[i] = -1*one_over_sqrt_10;
                      d_im[i] = -1*one_over_sqrt_10;
                      break;
                  case 13:
                      d_re[i] = -1*one_over_sqrt_10;
                      d_im[i] = -3*one_over_sqrt_10;
                      break;
                  case 14:
                      d_re[i] = -3*one_over_sqrt_10;
                      d_im[i] = -1*one_over_sqrt_10;
                      break;
                  case 15:
                      d_re[i] = -3*one_over_sqrt_10;
                      d_im[i] = -3*one_over_sqrt_10;
                      break;
                  }
              }
              *M_symb = (N_bits/4);
              if((N_bits % 4) != 0)
              {
                  *M_symb = (N_bits/4) + 1;
                  if((N_bits % 4) == 1)
                  {
                      input = bits[N_bits-1] << 3;
                  }else if((N_bits % 4) == 2){
                      input = ((bits[N_bits-2] << 3) |
                              (bits[N_bits-1] << 2));
                  }else if((N_bits % 4) == 3){
                      input = ((bits[N_bits-3] << 3) |
                               (bits[N_bits-2] << 2) |
                               (bits[N_bits-1] << 1));
                  }
                  switch(input)
                  {
                  case 0:
                      d_re[N_bits/4] = +1*one_over_sqrt_10;
                      d_im[N_bits/4] = +1*one_over_sqrt_10;
                      break;
                  case 1:
                      d_re[N_bits/4] = +1*one_over_sqrt_10;
                      d_im[N_bits/4] = +3*one_over_sqrt_10;
                      break;
                  case 2:
                      d_re[N_bits/4] = +3*one_over_sqrt_10;
                      d_im[N_bits/4] = +1*one_over_sqrt_10;
                      break;
                  case 3:
                      d_re[N_bits/4] = +3*one_over_sqrt_10;
                      d_im[N_bits/4] = +3*one_over_sqrt_10;
                      break;
                  case 4:
                      d_re[N_bits/4] = +1*one_over_sqrt_10;
                      d_im[N_bits/4] = -1*one_over_sqrt_10;
                      break;
                  case 5:
                      d_re[N_bits/4] = +1*one_over_sqrt_10;
                      d_im[N_bits/4] = -3*one_over_sqrt_10;
                      break;
                  case 6:
                      d_re[N_bits/4] = +3*one_over_sqrt_10;
                      d_im[N_bits/4] = -1*one_over_sqrt_10;
                      break;
                  case 7:
                      d_re[N_bits/4] = +3*one_over_sqrt_10;
                      d_im[N_bits/4] = -3*one_over_sqrt_10;
                      break;
                  case 8:
                      d_re[N_bits/4] = -1*one_over_sqrt_10;
                      d_im[N_bits/4] = +1*one_over_sqrt_10;
                      break;
                  case 9:
                      d_re[N_bits/4] = -1*one_over_sqrt_10;
                      d_im[N_bits/4] = +3*one_over_sqrt_10;
                      break;
                  case 10:
                      d_re[N_bits/4] = -3*one_over_sqrt_10;
                      d_im[N_bits/4] = +1*one_over_sqrt_10;
                      break;
                  case 11:
                      d_re[N_bits/4] = -3*one_over_sqrt_10;
                      d_im[N_bits/4] = +3*one_over_sqrt_10;
                      break;
                  case 12:
                      d_re[N_bits/4] = -1*one_over_sqrt_10;
                      d_im[N_bits/4] = -1*one_over_sqrt_10;
                      break;
                  case 13:
                      d_re[N_bits/4] = -1*one_over_sqrt_10;
                      d_im[N_bits/4] = -3*one_over_sqrt_10;
                      break;
                  case 14:
                      d_re[N_bits/4] = -3*one_over_sqrt_10;
                      d_im[N_bits/4] = -1*one_over_sqrt_10;
                      break;
                  case 15:
                      d_re[N_bits/4] = -3*one_over_sqrt_10;
                      d_im[N_bits/4] = -3*one_over_sqrt_10;
                      break;
                  }
              }
              break;
          case LIBLTE_PHY_MODULATION_TYPE_64QAM:
              // 3GPP TS 36.211 v10.1.0 section 7.1.4
              for(i=0; i<(N_bits/6); i++)
              {
                  switch((bits[i*6+0] << 5) |
                         (bits[i*6+1] << 4) |
                         (bits[i*6+2] << 3) |
                         (bits[i*6+3] << 2) |
                         (bits[i*6+4] << 1) |
                         bits[i*6+5])
                  {
                  case 0:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 1:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 2:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 3:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 4:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 5:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 6:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 7:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 8:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 9:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 10:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 11:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 12:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 13:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 14:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 15:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 16:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 17:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 18:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 19:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 20:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 21:
                      d_re[i] = +3*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 22:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 23:
                      d_re[i] = +1*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 24:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 25:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 26:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 27:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 28:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 29:
                      d_re[i] = +5*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 30:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 31:
                      d_re[i] = +7*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 32:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 33:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 34:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 35:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 36:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 37:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 38:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 39:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 40:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 41:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 42:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = +3*one_over_sqrt_42;
                      break;
                  case 43:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = +1*one_over_sqrt_42;
                      break;
                  case 44:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 45:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 46:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = +5*one_over_sqrt_42;
                      break;
                  case 47:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = +7*one_over_sqrt_42;
                      break;
                  case 48:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 49:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 50:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 51:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 52:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 53:
                      d_re[i] = -3*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 54:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 55:
                      d_re[i] = -1*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 56:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 57:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 58:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = -3*one_over_sqrt_42;
                      break;
                  case 59:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = -1*one_over_sqrt_42;
                      break;
                  case 60:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 61:
                      d_re[i] = -5*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  case 62:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = -5*one_over_sqrt_42;
                      break;
                  case 63:
                      d_re[i] = -7*one_over_sqrt_42;
                      d_im[i] = -7*one_over_sqrt_42;
                      break;
                  }
              }
              *M_symb = (N_bits/6);
              if((N_bits % 6) != 0)
              {
                  *M_symb = (N_bits/6) + 1;
                  if((N_bits % 6) == 1)
                  {
                      input = bits[N_bits-1] << 5;
                  }else if((N_bits % 6) == 2){
                      input = ((bits[N_bits-2] << 5) |
                               (bits[N_bits-1] << 4));
                  }else if((N_bits % 6) == 3){
                      input = ((bits[N_bits-3] << 5) |
                               (bits[N_bits-2] << 4) |
                               (bits[N_bits-1] << 3));
                  }else if((N_bits % 6) == 4){
                      input = ((bits[N_bits-4] << 5) |
                               (bits[N_bits-3] << 4) |
                               (bits[N_bits-2] << 3) |
                               (bits[N_bits-1] << 2));
                  }else if((N_bits % 6) == 5){
                      input = ((bits[N_bits-5] << 5) |
                               (bits[N_bits-4] << 4) |
                               (bits[N_bits-3] << 3) |
                               (bits[N_bits-2] << 2) |
                               (bits[N_bits-1] << 1));
                  }
                  switch(input)
                  {
                  case 0:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 1:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 2:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 3:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 4:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 5:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 6:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 7:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 8:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 9:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 10:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 11:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 12:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 13:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 14:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 15:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 16:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 17:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 18:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 19:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 20:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 21:
                      d_re[N_bits/6] = +3*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 22:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 23:
                      d_re[N_bits/6] = +1*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 24:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 25:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 26:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 27:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 28:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 29:
                      d_re[N_bits/6] = +5*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 30:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 31:
                      d_re[N_bits/6] = +7*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 32:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 33:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 34:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 35:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 36:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 37:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 38:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 39:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 40:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 41:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 42:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = +3*one_over_sqrt_42;
                      break;
                  case 43:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = +1*one_over_sqrt_42;
                      break;
                  case 44:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 45:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 46:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = +5*one_over_sqrt_42;
                      break;
                  case 47:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = +7*one_over_sqrt_42;
                      break;
                  case 48:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 49:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 50:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 51:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 52:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 53:
                      d_re[N_bits/6] = -3*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 54:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 55:
                      d_re[N_bits/6] = -1*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 56:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 57:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 58:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = -3*one_over_sqrt_42;
                      break;
                  case 59:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = -1*one_over_sqrt_42;
                      break;
                  case 60:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 61:
                      d_re[N_bits/6] = -5*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  case 62:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = -5*one_over_sqrt_42;
                      break;
                  case 63:
                      d_re[N_bits/6] = -7*one_over_sqrt_42;
                      d_im[N_bits/6] = -7*one_over_sqrt_42;
                      break;
                  }
              }
              break;
          }
      }

  void modulation_mapper_impl::symb_to_gr_complex(float *re, float *im, uint32 len, gr_complex *out)
  {
      for (uint32 i=0; i<len; i++)
      {
          out->real(*re);
          out->imag(*im);

          out++;
          re++;
          im++;
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

