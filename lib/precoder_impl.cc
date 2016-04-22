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
#include "precoder_impl.h"
#include <vector>
#include <complex>

namespace gr {
  namespace my_lte {

  precoder::sptr
  precoder::make(uint32 N_ant, uint32 set_align, bool is_phich, int n_phich_group, bool normal_cp)
  {
    return gnuradio::get_initial_sptr
      (new precoder_impl(N_ant, set_align, is_phich, n_phich_group, normal_cp));
  }

  /*
   * The private constructor
   */
  precoder_impl::precoder_impl(uint32 N_ant, uint32 set_align, bool is_phich, int n_phich_group, bool normal_cp)
    : gr::block("precoder",
                gr::io_signature::make(1, N_ant, sizeof(gr_complex)),
                gr::io_signature::make(1, N_ant, sizeof(gr_complex))),
      N_antenna(N_ant),
      d_n_in_items_required(set_align),
      d_is_phich(is_phich),
      phich_count(0),
      d_n_phich_group(n_phich_group),
      d_cp_normal(normal_cp)
  {
      set_tag_propagation_policy(TPP_DONT);

      set_alignment(set_align);
  }

  precoder_impl::~precoder_impl()
  {
  }

  void
  precoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
  {
      ninput_items_required[0] = d_n_in_items_required;
  }

  int
  precoder_impl::general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
  {
      const gr_complex *x_symb[N_antenna];
      gr_complex *y_symb[N_antenna];

      for (int n=0; n<N_antenna; n++)
      {
          x_symb[n] = (gr_complex *) input_items[n];
          y_symb[n] = (gr_complex *) output_items[n];

      }

      uint32 M_symb_layer = 0, M_symb_ap = 0, nout_written = 0;
      vector<tag_t> all_tag;

      // reading tags
      get_tags_in_window(all_tag,
                         0,
                         0,
                         ninput_items[0],
                         pmt::string_to_symbol("length"));

      if (all_tag.size() >0)
          M_symb_layer = pmt::to_long(all_tag[0].value);      // reading one packet
      else
          throw runtime_error("\tprecoder didn't have been read tags");

      if(N_antenna == 1)
      {
          M_symb_ap = M_symb_layer;

          // checking enough inputs
          if (M_symb_layer <= ninput_items[0])
          {
              // checking output buffer
              if (M_symb_ap <= noutput_items)
              {
                  for (uint32 i=0; i<M_symb_ap; i++)
                  {
                      *y_symb[0] = *x_symb[0];

                      y_symb[0]++;
                      x_symb[0]++;
                      nout_written++;
                  }

//                  cout << "precoder" << endl;

                  // adding tag
                  add_item_tag(0, nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_ap));
                  produce(0, M_symb_ap);

                  // printing pbch
//                  if (M_symb_layer == 960)
//                  {
//                      cout << "PBCH ======================================================" << endl;
//                      for (int i=0; i<960; i++)
//                      {
//                            cout << y_symb[0][i] << endl;
//                      }
//                  }
              }
              else
              {
                  throw runtime_error("not enough output items");
                  produce(0,0);
              }
          }
          else
          {
              throw runtime_error("Not enough input items");
          }

      }
      else if (N_antenna == 2)
      {
          M_symb_ap = 2*M_symb_layer;

          // checking enough inputs
          if (M_symb_layer <= ninput_items[0])
          {
              // checking output buffer
              if (M_symb_ap <= noutput_items)
              {
                  for (uint32 i=0; i<M_symb_layer; i++)
                  {
                      *y_symb[0] = *x_symb[0]/gr_complex (sqrt(2));
                      y_symb[0]++;
                      *y_symb[0] = *x_symb[1]/gr_complex (sqrt(2));
                      y_symb[0]++;

                      *y_symb[1] = -conj(*x_symb[1])/gr_complex (sqrt(2));
                      y_symb[1]++;
                      *y_symb[1] = conj(*x_symb[0])/gr_complex (sqrt(2));
                      y_symb[1]++;

                      x_symb[0]++;
                      x_symb[1]++;
                      nout_written +=2;
                  }

                  cout << "precoder" << endl;

                  // adding tag
                  add_item_tag(0, nitems_written(0),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_ap));

                  add_item_tag(1, nitems_written(1),
                               pmt::string_to_symbol("length"),
                               pmt::from_long(M_symb_ap));
                  produce(0, M_symb_ap);
                  produce(1, M_symb_ap);
              }
              else
              {
                  throw runtime_error("Not enough output items");
                  produce(0,0);
                  produce(1,0);
              }
          }
          else
          {
              throw runtime_error("Not enough input items");
          }

      }
      else    // 4 antennas
      {

          M_symb_ap = 4*M_symb_layer-2;

          // checking enough inputs
          if (M_symb_layer <= ninput_items[0])
          {
              // checking output buffer
              if (M_symb_ap <= noutput_items)
              {

                  if(d_is_phich)
                  {

                      if ( ((phich_count+d_n_phich_group)%2 == 0 & d_cp_normal) ||
                           (((phich_count+d_n_phich_group/2)%2 == 0 & !d_cp_normal)))
                      {

                          for (uint32 i=0; i<M_symb_layer; i++)
                          {
                              y_symb_phich0[0+4*phich_count] = *x_symb[0]/gr_complex (sqrt(2));
                              y_symb_phich0[1+4*phich_count] = *x_symb[1]/gr_complex (sqrt(2));
                              y_symb_phich0[2+4*phich_count] = *x_symb[2]/gr_complex (sqrt(2));
                              y_symb_phich0[3+4*phich_count] = *x_symb[3]/gr_complex (sqrt(2));

                              y_symb_phich1[0+4*phich_count] =0;
                              y_symb_phich1[1+4*phich_count] =0;
                              y_symb_phich1[2+4*phich_count] =0;
                              y_symb_phich1[3+4*phich_count] =0;

                              y_symb_phich2[0+4*phich_count] =-conj(*x_symb[1])/gr_complex (sqrt(2));
                              y_symb_phich2[1+4*phich_count] =conj(*x_symb[0])/gr_complex (sqrt(2));
                              y_symb_phich2[2+4*phich_count] =-conj(*x_symb[3])/gr_complex (sqrt(2));
                              y_symb_phich2[3+4*phich_count] = conj(*x_symb[2])/gr_complex (sqrt(2));

                              y_symb_phich3[0+4*phich_count] =0;
                              y_symb_phich3[1+4*phich_count] =0;
                              y_symb_phich3[2+4*phich_count] =0;
                              y_symb_phich3[3+4*phich_count] =0;

                              x_symb[0]++;
                              x_symb[1]++;
                              x_symb[2]++;
                              x_symb[3]++;
                              nout_written +=4;
                          }

                      }
                      else
                      {

                          for (uint32 i=0; i<M_symb_layer; i++)
                          {
                              y_symb_phich0[0+4*phich_count] =0;
                              y_symb_phich0[1+4*phich_count] =0;
                              y_symb_phich0[2+4*phich_count] =0;
                              y_symb_phich0[3+4*phich_count] =0;

                              y_symb_phich1[0+4*phich_count] = *x_symb[0]/gr_complex (sqrt(2));
                              y_symb_phich1[1+4*phich_count] = *x_symb[1]/gr_complex (sqrt(2));
                              y_symb_phich1[2+4*phich_count] = *x_symb[2]/gr_complex (sqrt(2));
                              y_symb_phich1[3+4*phich_count] = *x_symb[3]/gr_complex (sqrt(2));

                              y_symb_phich2[0+4*phich_count] =0;
                              y_symb_phich2[1+4*phich_count] =0;
                              y_symb_phich2[2+4*phich_count] =0;
                              y_symb_phich2[3+4*phich_count] =0;

                              y_symb_phich3[0+4*phich_count] =-conj(*x_symb[1])/gr_complex (sqrt(2));
                              y_symb_phich3[1+4*phich_count] =conj(*x_symb[0])/gr_complex (sqrt(2));
                              y_symb_phich3[2+4*phich_count] =-conj(*x_symb[3])/gr_complex (sqrt(2));
                              y_symb_phich3[3+4*phich_count] = conj(*x_symb[2])/gr_complex (sqrt(2));


                              x_symb[0]++;
                              x_symb[1]++;
                              x_symb[2]++;
                              x_symb[3]++;
                              nout_written +=4;
                          }
                      }


                      // we are not

                      if ( (phich_count == 2))
                      {
                          phich_count = 0;

                          memcpy(y_symb[0], y_symb_phich0, 12 *sizeof(gr_complex));
                          memcpy(y_symb[1], y_symb_phich1, 12 *sizeof(gr_complex));
                          memcpy(y_symb[2], y_symb_phich2, 12 *sizeof(gr_complex));
                          memcpy(y_symb[3], y_symb_phich3, 12 *sizeof(gr_complex));

                          // adding tag
                          add_item_tag(0, nitems_written(0),
                                       pmt::string_to_symbol("length"),
                                       pmt::from_long(12));

                          add_item_tag(1, nitems_written(1),
                                       pmt::string_to_symbol("length"),
                                       pmt::from_long(12));

                          add_item_tag(2, nitems_written(2),
                                       pmt::string_to_symbol("length"),
                                       pmt::from_long(12));

                          add_item_tag(3, nitems_written(3),
                                       pmt::string_to_symbol("length"),
                                       pmt::from_long(12));

                          produce(0, 12);
                          produce(1, 12);
                          produce(2, 12);
                          produce(3, 12);
                      }
                      else
                      {
                          produce(0, 0);
                          produce(1, 0);
                          produce(2, 0);
                          produce(3, 0);
                      }
                      phich_count++;
                  }
                  else
                  {
                    // isn't PHICH channel
                      for (uint32 i=0; i<M_symb_layer; i++)
                      {
                          *y_symb[0] = *x_symb[0]/gr_complex (sqrt(2));
                          y_symb[0]++;
                          *y_symb[0] = *x_symb[1]/gr_complex (sqrt(2));
                          y_symb[0]++;
                          *y_symb[0] =0;
                          y_symb[0]++;
                          *y_symb[0] =0;
                          y_symb[0]++;

                          *y_symb[1] =0;
                          y_symb[1]++;
                          *y_symb[1] =0;
                          y_symb[1]++;
                          *y_symb[1] = *x_symb[2]/gr_complex (sqrt(2));
                          y_symb[1]++;
                          *y_symb[1] = *x_symb[3]/gr_complex (sqrt(2));
                          y_symb[1]++;

                          *y_symb[2] =-conj(*x_symb[1])/gr_complex (sqrt(2));
                          y_symb[2]++;
                          *y_symb[2] =conj(*x_symb[0])/gr_complex (sqrt(2));
                          y_symb[2]++;
                          *y_symb[2] =0;
                          y_symb[2]++;
                          *y_symb[2] =0;
                          y_symb[2]++;

                          *y_symb[3] =0;
                          y_symb[3]++;
                          *y_symb[3] =0;
                          y_symb[3]++;
                          *y_symb[3] = -conj(*x_symb[3])/gr_complex (sqrt(2));
                          y_symb[3]++;
                          *y_symb[3] = conj(*x_symb[2])/gr_complex (sqrt(2));
                          y_symb[3]++;

                          x_symb[0]++;
                          x_symb[1]++;
                          x_symb[2]++;
                          x_symb[3]++;
                          nout_written +=4;
                      }

                      // adding tag
                      add_item_tag(0, nitems_written(0),
                                   pmt::string_to_symbol("length"),
                                   pmt::from_long(M_symb_ap));

                      add_item_tag(1, nitems_written(1),
                                   pmt::string_to_symbol("length"),
                                   pmt::from_long(M_symb_ap));

                      add_item_tag(2, nitems_written(2),
                                   pmt::string_to_symbol("length"),
                                   pmt::from_long(M_symb_ap));

                      add_item_tag(3, nitems_written(3),
                                   pmt::string_to_symbol("length"),
                                   pmt::from_long(M_symb_ap));
                      produce(0, M_symb_ap);
                      produce(1, M_symb_ap);
                      produce(2, M_symb_ap);
                      produce(3, M_symb_ap);
                  } // is phich


              }// checking output
              else
              {
                  throw runtime_error("Not enough output items");
                  produce(0, 0);
                  produce(1, 0);
                  produce(2, 0);
                  produce(3, 0);
              }

              cout << "precoder" << endl;
          }
          else
          {
              throw runtime_error("Not enough input items");
          }
      }

      consume_each (M_symb_layer);


      return WORK_CALLED_PRODUCE;
  }

  } /* namespace my_lte */
} /* namespace gr */

