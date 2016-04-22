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
#include "repeater_impl.h"

namespace gr {
  namespace my_lte {

    repeater::sptr
    repeater::make(int n)
    {
      return gnuradio::get_initial_sptr
        (new repeater_impl(n));
    }

    repeater_impl::repeater_impl(int num)
      : gr::sync_interpolator("repeater",
              gr::io_signature::make(1,1, sizeof(uint8)),
              gr::io_signature::make(1,1, sizeof(uint8)), num),
        d_num(num)
    {}

    repeater_impl::~repeater_impl()
    {
    }

    int
    repeater_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const uint8 *in = (const uint8 *) input_items[0];
        uint8 *out = (uint8 *) output_items[0];

        vector<tag_t> tag_len;
        get_tags_in_window(tag_len,
                           0,
                           0,
                           noutput_items/d_num,
                           pmt::string_to_symbol("length"));

        if(tag_len.size()>0)
        {
            int idx = 0;
            for(int i=0; i<pmt::to_long(tag_len[0].value); i++)
            {
                for (int j=0; j<d_num; j++)
                {
                    out[idx] = in[i];
                    idx++;
                }
            }

            // adding tag
            add_item_tag(0,
                         nitems_written(0),
                         pmt::string_to_symbol("length"),
                         pmt::from_long(idx));
//            cout << "repeater " << endl;

            return idx;
        }
        else
        {
            throw runtime_error("No tags read");
        }

        return 0;
    }

  } /* namespace my_lte */
} /* namespace gr */

