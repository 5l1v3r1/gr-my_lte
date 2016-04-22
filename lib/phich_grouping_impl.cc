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
#include "phich_grouping_impl.h"

int N_FS_phich(bool normal_cp){
    if(normal_cp)
        return 8;
    else
        return 4;
}

namespace gr {
  namespace my_lte {

    phich_grouping::sptr
    phich_grouping::make(bool normal_cp, int N_phich_group)
    {
      return gnuradio::get_initial_sptr
        (new phich_grouping_impl(normal_cp, N_phich_group));
    }

    phich_grouping_impl::phich_grouping_impl(bool normal_cp, int N_phich_group)
      : gr::block("phich_grouping",
                  gr::io_signature::make(N_phich_group * N_FS_phich(normal_cp), N_phich_group * N_FS_phich(normal_cp) , sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_normal_cp(normal_cp),
      d_N_phich_group(N_phich_group)
    {
        set_tag_propagation_policy(TPP_DONT);
    }

    phich_grouping_impl::~phich_grouping_impl()
    {
    }

    void
    phich_grouping_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for (int i=0; i<d_N_phich_group * N_FS_phich(d_normal_cp); i++)
        {
            ninput_items_required[i] = 12;
        }
    }

    int
    phich_grouping_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *in[48];//(d_N_phich_group * N_FS_phich(d_normal_cp));
        gr_complex *out = (gr_complex *) output_items[0];

        int *phich_in_len = new int [d_N_phich_group* N_FS_phich(d_normal_cp)];

        int x = 0;
        // reading tags
        vector<tag_t> tag_len[48];   //[d_N_phich_group * N_FS_phich(d_normal_cp)];
        for (int i=0; i<d_N_phich_group * N_FS_phich(d_normal_cp); i++)
        {
            in[i] = (gr_complex *) input_items[i];

            get_tags_in_window(tag_len[i],
                               i,
                               0,
                               ninput_items[i],
                               pmt::string_to_symbol("length"));
            if(tag_len[i].size()>0)
            {
                phich_in_len[i] = pmt::to_long(tag_len[i][0].value);
                x++;
            }

        }

//        gr_complex y_p[d_N_phich_group][12];
//        gr_complex y__p_tilt[d_N_phich_group/2][12];

        gr_complex y_p[48][12];
        gr_complex y__p_tilt[48/2][12];

        if (x == d_N_phich_group * N_FS_phich(d_normal_cp)) // x == d_N_phich_group) is for checking all the input tags are read
        {
            // checking enough input items
            for (int j=0; j<d_N_phich_group * N_FS_phich(d_normal_cp); j++)
            {
                if (phich_in_len[j] > ninput_items[j])
                {
                    throw runtime_error("Not enough input items");
                }

            }
                // grouping ...
            for (int i=0; i<d_N_phich_group ;i++)
            {
                for (int k=0; k<phich_in_len[i]; k++)
                {
                    y_p[i][k] = 0;
                    for (int j=0; j<N_FS_phich(d_normal_cp); j++)
                    {
                        y_p[i][k] = y_p[i][k] + ((gr_complex*)input_items[i*8+j])[k];   //in[i*8+j][k];
                    }
                }

            }

            // ckecking enough output items
            if(12 * d_N_phich_group <= noutput_items)
            {
                // PHICH mapping unit not needed for normal cp!
                if (!d_normal_cp)
                {
                    for (int i=0; i<d_N_phich_group-1; i+=2)
                    {
                        for(int k=0; k<12; k++)
                        {
                            y__p_tilt[i/2][k] = y_p[i][k] + y_p[i+1][k];
                        }
                    }

                    memcpy(out, y__p_tilt, 12 * d_N_phich_group* sizeof(gr_complex));
                    produce(0, 12 * d_N_phich_group);
                }
                else
                {
                    memcpy(out, y_p, 12 * d_N_phich_group * sizeof(gr_complex));
                    produce(0, 12 * d_N_phich_group);
                }
            }
            else
            {
                cout << "<phich_grouping Block> Not enough output items" << endl;
            }

            // adding tag
            add_item_tag(0,
                         nitems_written(0),
                         pmt::string_to_symbol("length"),
                         pmt::from_long(12 * d_N_phich_group));

//            cout << "PHICH grouping" << endl;

            consume_each(12);

        }
        else
        {   // if tags didn't read
            throw runtime_error("No tags read");
            produce(0, 0);
            consume_each(0);
        }

//        delete []in;
        delete []phich_in_len;
//        delete []tag_len;

//        for (int u=0; u<d_N_phich_group; u++)
//            delete []y_p[u];

//        for (int u=0; u<d_N_phich_group/2; u++)
//            delete []y__p_tilt;

//        delete []y_p;
//        delete []y__p_tilt;

        return WORK_CALLED_PRODUCE;
    }

  } /* namespace my_lte */
} /* namespace gr */

