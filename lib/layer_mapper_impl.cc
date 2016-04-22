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
#include "layer_mapper_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {

  layer_mapper::sptr
  layer_mapper::make(uint32 N_ant, uint32 set_align)
  {
    return gnuradio::get_initial_sptr
      (new layer_mapper_impl(N_ant, set_align));
  }

  /*
   * The private constructor
   */
  layer_mapper_impl::layer_mapper_impl(uint32 N_ant, uint32 set_align)
    : gr::block("layer_mapper",
            gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, N_ant, sizeof(gr_complex))),
      N_antenna(N_ant),
      d_n_in_items_required(set_align)
  {
      set_tag_propagation_policy(TPP_DONT);

      set_alignment(set_align);
  }

  layer_mapper_impl::~layer_mapper_impl()
  {
  }

  void
  layer_mapper_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = d_n_in_items_required;
  }

  int
  layer_mapper_impl::general_work (int noutput_items,
                                   gr_vector_int &ninput_items,
                                   gr_vector_const_void_star &input_items,
                                   gr_vector_void_star &output_items)
  {
      const gr_complex *d_symb = (gr_complex *) input_items[0];
      vector<gr_complex*> x_symb(N_antenna);
      int nout_written =0;

      for (int n=0; n<N_antenna; n++)
      {
          x_symb[n] = (gr_complex *) output_items[n];
      }

      // reading tags
      uint32 M_symb = 0;      // must be zero because of consume_each ...
      uint32 M_symb_layer = 0;

      vector<tag_t> all_tag;
      get_tags_in_range(all_tag,
                        0,
                        nitems_read(0),
                        nitems_read(0) + ninput_items[0],
                        pmt::string_to_symbol("length"));

      // if read any ...
      if (all_tag.size()>0)
      {
          M_symb = pmt::to_long(all_tag[0].value);
      }
      else
          throw runtime_error("No tags read");

      // for adding 2 NULL symbols where N ant = 4 and the input length%4 != 0
      gr_complex d_symb_prim[M_symb+2];

      if (N_antenna == 1)
      {
          M_symb_layer = M_symb;

          // checking enough inputs
          if(M_symb <= ninput_items[0])
          {

              // checking enough output
              if((M_symb_layer <= noutput_items) & (M_symb_layer != 0))
              {
                  for (uint32 i=0; i<M_symb_layer; i++)
                  {
                      *x_symb[0] = *d_symb;

                      x_symb[0]++;
                      d_symb++;
                      nout_written++;
                  }
                  // add tag
                  add_item_tag(0,
                               nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));

//                  cout << "layer mapper" << endl;
              }
              else
              {
                  throw runtime_error("Not enough output items");
              }
          }
          else
          {
              throw runtime_error("Not enough input items");
          }

      }
      else if (N_antenna == 2)
      {
          M_symb_layer = M_symb/2;

          // checking enough input and output
          if (M_symb <= ninput_items[0])
          {
              if (M_symb_layer <= noutput_items)
              {
                  for (uint32 i=0; i<M_symb_layer; i++)
                  {
                      *x_symb[0] = *d_symb;
                      *x_symb[1] = *(d_symb+1);

                      d_symb += 2;
                      x_symb[0]++;
                      x_symb[1]++;
                      nout_written++;
                  }
                  // add tags
                  add_item_tag(0,
                               nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));
                  add_item_tag(1,
                               nitems_written(1),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));

                  cout << "layer mapper" << endl;
              }
              else{
                  throw runtime_error("Not enough output items");
              }
          }
          else {
              throw runtime_error("Not enough input items");
          }
      }
      else // 4 antennas
      {
          if (M_symb%4 == 0)
          {
              M_symb_layer = M_symb /4;
          }
          else
          {
              M_symb_layer = (M_symb + 2)/4;

              // two adding NULL symbols
              memcpy(d_symb_prim, d_symb, M_symb * sizeof(gr_complex));
              d_symb_prim[M_symb] = NULL;
              d_symb_prim[M_symb+1] = NULL;

           }

          // checking enough input and output buffer
          if(M_symb <= ninput_items[0])
          {

              if (M_symb_layer <= noutput_items)
              {
                  for( uint32 i=0; i< M_symb_layer ; i++)
                  {
                      if(M_symb%4 == 0)
                      {
                          *x_symb[0] = *d_symb;
                          *x_symb[1] = *(d_symb+1);
                          *x_symb[2] = *(d_symb+2);
                          *x_symb[3] = *(d_symb+3);
                      }
                      else
                      { // NULL symbol inserted !
                          *x_symb[0] = *d_symb_prim;
                          *x_symb[1] = *(d_symb_prim+1);
                          *x_symb[2] = *(d_symb_prim+2);
                          *x_symb[3] = *(d_symb_prim+3);
                      }

                      x_symb[0]++;
                      x_symb[1]++;
                      x_symb[2]++;
                      x_symb[3]++;
                      d_symb += 4;
                      nout_written++;

                  }
                  // add tags
                  add_item_tag(0,
                               nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));
                  add_item_tag(1,
                               nitems_written(2),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));
                  add_item_tag(2,
                               nitems_written(3),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));
                  add_item_tag(3,
                               nitems_written(4),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_layer));

                  cout << "layer mapper" << endl;
              }
              else
              {
                  throw runtime_error("Not enough output items");
              }
          }
          else
          {
              throw runtime_error("Not enough input items");
          }
      }
      consume_each(M_symb);

      // Tell runtime system how many output items we produced.
      return nout_written;
  }

  } /* namespace my_lte */
} /* namespace gr */

