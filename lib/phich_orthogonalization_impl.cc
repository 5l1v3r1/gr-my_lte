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
#include "phich_orthogonalization_impl.h"

int N_FS(bool cp);

// table 6.9.1-2
gr_complex W_normal[8][4]     = {{1,1,1,1},
                                 {1,-1,1,-1},
                                 {1,1,-1,-1},
                                 {1,-1,-1,1},
                                 {gr_complex(0,1),gr_complex(0,1),gr_complex(0,1),gr_complex(0,1)},
                                 {gr_complex(0,1),gr_complex(0,-1),gr_complex(0,1),gr_complex(0,-1)},
                                 {gr_complex(0,1),gr_complex(0,1),gr_complex(0,-1),gr_complex(0,-1)},
                                 {gr_complex(0,1),gr_complex(0,-1),gr_complex(0,-1),gr_complex(0,1)}};
gr_complex W_extended[4][4]   = {{1,1},
                                 {1,-1},
                                 {gr_complex(0,1),gr_complex(0,1)},
                                 {gr_complex(0,1),gr_complex(0,-1)}};

namespace gr {
  namespace my_lte {

    phich_orthogonalization::sptr
    phich_orthogonalization::make(bool normal_cp, int N_PHICH_seq)
    {
      return gnuradio::get_initial_sptr
        (new phich_orthogonalization_impl(normal_cp, N_PHICH_seq));
    }

    /*
     * The private constructor
     */
    phich_orthogonalization_impl::phich_orthogonalization_impl(bool normal_cp, int N_PHICH_seq)
      : gr::sync_interpolator("phich_orthogonalization",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              gr::io_signature::make(1, 1, sizeof(gr_complex)), N_FS(normal_cp)),
        d_normal_cp(normal_cp),
        d_N_PHICH_seq(N_PHICH_seq)
    {
        set_tag_propagation_policy(TPP_DONT);
    }

    phich_orthogonalization_impl::~phich_orthogonalization_impl()
    {
    }

    int
    phich_orthogonalization_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        vector<tag_t> tag_len;
        get_tags_in_window(tag_len,
                           0,
                           0,
                           noutput_items/N_FS(d_normal_cp),
                           pmt::string_to_symbol("length"));

        if(tag_len.size()>0)
        {
            int nout = pmt::to_long(tag_len[0].value)*N_FS(d_normal_cp);

            // checking enough input/output items
            if (nout <= noutput_items)
            {
                if(d_normal_cp)
                {
                    for(int i=0; i<nout; i++)
                    {
                        out[i] = W_normal[d_N_PHICH_seq][i%N_FS(d_normal_cp)] * in[i/N_FS(d_normal_cp)];
                        //                    cout << "output of ORTH   " << out[i] << endl;
                    }
                }
                else
                {   // extended cp

                    for(int i=0; i<pmt::to_long(tag_len[0].value)*N_FS(d_normal_cp); i++)
                    {
                        out[i] = W_extended[d_N_PHICH_seq][i%N_FS(d_normal_cp)] * in[i/N_FS(d_normal_cp)];
                    }
                }
                //            cout << "PHICH Orthogonalization" << endl;

                // adding tag ...
                add_item_tag(0,
                             nitems_written(0),
                             pmt::string_to_symbol("length"),
                             pmt::from_long(3 * N_FS(d_normal_cp)));

                return 3 * N_FS(d_normal_cp);
            }
            else
            {
                cout << "<phich_orthogonalization Block> Not enough input/output items" << endl;
            }
        }
        else
        {
            throw runtime_error("No tags read");
        }

        return 0;
    }

  } /* namespace my_lte */
} /* namespace gr */

int N_FS(bool cp)
{
    if(cp)
        return 4;
    else
        return 2;
}
