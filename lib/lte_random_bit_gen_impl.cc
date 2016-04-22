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
#include "lte_random_bit_gen_impl.h"
#include <stdlib.h>
#include <time.h>
#include <bitset>
#include <unistd.h>
//#include <boost/thread/condition_variable.hpp>
//#include <boost/thread/condition.hpp>

namespace gr {
  namespace my_lte {

  lte_random_bit_gen::sptr
  lte_random_bit_gen::make(int packet_len, uint32 usleep_val)
  {
    return gnuradio::get_initial_sptr
      (new lte_random_bit_gen_impl(packet_len, usleep_val));
  }

  /*
   * The private constructor
   */
  lte_random_bit_gen_impl::lte_random_bit_gen_impl(int packet_len, uint32 usleep_val)
    : gr::sync_block("lte_random_bit_gen",
            gr::io_signature::make(0, 0, 0),
            gr::io_signature::make(1, 1, sizeof(uint8))),
      packet_length(packet_len),
      is_disable(false),
      d_usleep_val(usleep_val),
      count(0)
  {
      srand (time(NULL)); // randomizing the seed

      set_alignment(packet_length);

      // message input port
      message_port_register_in(pmt::mp("disable"));

      set_msg_handler(pmt::mp("disable"),
       boost::bind(&lte_random_bit_gen_impl::msg_handler, this, _1));

  }

  lte_random_bit_gen_impl::~lte_random_bit_gen_impl()
  {
  }

  int
  lte_random_bit_gen_impl::work(int noutput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
  {
      uint8 *out = (uint8 *) output_items[0];

      int random_val, nout_written = 0;
      usleep(d_usleep_val);

     uint8 b_bch[24] = {1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1};
//     uint8 b_bch[24] = {1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1};
//     uint8 b_bch[24] = {0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0};

      for (int i=0; i<packet_length; i++)
      {
          random_val = rand();
          bitset<8> b(random_val);

          if (packet_length == 24)  // for bch
          {
              bitset<8> nframe(count%4);    // frame number
              for(int i=0; i<8; i++) b_bch[13-i] = nframe[i];
              bitset<3> bw(2);   // 5MHz bandwidth
              for(int i=0; i<3; i++) b_bch[2-i] = bw[i];
              *out = b_bch[i];
          }
//          else if (packet_length == 2088)   // for PDSCH
//          {
//              *out = 0x01;
//          }
          else
          {
              if (b[0] == 0)
                  *out = 1;
              else
                  *out = 0;
          }
          out++;
          nout_written++;
      }
      count++;
      // add tag
      add_item_tag(0,
                   nitems_written(0),
                   pmt::string_to_symbol("length"),
                   pmt::from_long(packet_length));

      //          cout << "bit generator " << endl ;

      return nout_written;

  }

  void lte_random_bit_gen_impl::msg_handler(pmt::pmt_t msg)
  {

      if (pmt::to_bool(msg))
      {
        is_disable = true;
//        cout << "\t\tReceiving message in source and enabling... \n";
      }
  }

  } /* namespace my_lte */
} /* namespace gr */

