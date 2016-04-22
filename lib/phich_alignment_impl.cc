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
#include "phich_alignment_impl.h"


int interpolat_fact(bool normal_cp)
{
    if (normal_cp)
        return 1;
    else
        return 2;
}

namespace gr {
  namespace my_lte {

    phich_alignment::sptr
    phich_alignment::make(bool normal_cp, int n_phich_group)
    {
      return gnuradio::get_initial_sptr
        (new phich_alignment_impl(normal_cp, n_phich_group));
    }

    phich_alignment_impl::phich_alignment_impl(bool normal_cp, int n_phich_group)
      : gr::sync_interpolator("phich_alignment",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, 1, sizeof(gr_complex)), interpolat_fact(normal_cp)),
        is_normal_cp(normal_cp),
        d_n_phich_group(n_phich_group)
    {
        set_tag_propagation_policy(TPP_DONT);
    }

    phich_alignment_impl::~phich_alignment_impl()
    {
    }

    int
    phich_alignment_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        int ninput = noutput_items/interpolat_fact(is_normal_cp);

        vector<tag_t> tag_len;
        int packet_len = 0;
        get_tags_in_window(tag_len,
                           0,
                           0,
                           ninput,
                           pmt::string_to_symbol("length"));

        if (tag_len.size()>0)
        {
            packet_len = pmt::to_long(tag_len[0].value);

            // checking enough input items @ runtime
            if( packet_len <= ninput)
            {
                if (is_normal_cp)
                {
                    // do nothing with input (just put to the output just one packet received!)
                    memcpy(out, in, sizeof(gr_complex) * packet_len);
                    add_item_tag(0,
                                 nitems_written(0),
                                 pmt::string_to_symbol("length"),
                                 tag_len[0].value);

                    //            cout <<"output of PHICH alignment  " << out[j] << endl;
                    //            cout << "PHICH alignment" << endl;

                    return packet_len;
                }
                else
                {   // extended cp alignment: 3GPP TS 36.211 version 10.1.0 Release 10 section 6.9.2

                    if (d_n_phich_group%2 == 0)
                    {
                        for (int i=0; i<packet_len/2; i++)
                        {
                            int temp_ar[]= {0,0};
                            memcpy(&out[4*i], &in[2*i], 2 * sizeof(gr_complex));
                            memcpy(&out[4*i+2], temp_ar, 2 * sizeof(gr_complex));
                        }
                    }
                    else
                    {
                        for (int i=0; i<packet_len/2; i++)
                        {
                            int temp_ar[]= {0,0};
                            memcpy(&out[4*i], temp_ar, 2 * sizeof(gr_complex));
                            memcpy(&out[4*i+2], &in[2*i], 2 * sizeof(gr_complex));
                        }
                    }

                    //                cout << "PHICH alignment" << endl;
                    // adding tag ...
                    add_item_tag(0,
                                 nitems_written(0),
                                 pmt::string_to_symbol("length"),
                                 pmt::from_long(packet_len*2));

                    return packet_len*2;

                }
            }
            else
            {
                throw runtime_error("Not enough input items");
                return 0;
            }
        }
        else
        {
            throw runtime_error("No tags read");
            return 0;
        }


    }

  } /* namespace my_lte */
} /* namespace gr */

