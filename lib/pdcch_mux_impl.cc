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
#include "pdcch_mux_impl.h"
#include <algorithm>

namespace gr {
  namespace my_lte {

    pdcch_mux::sptr
    pdcch_mux::make(int n_pdcch, int n_cce)
    {
      return gnuradio::get_initial_sptr
        (new pdcch_mux_impl(n_pdcch, n_cce));
    }

    pdcch_mux_impl::pdcch_mux_impl(int n_pdcch, int n_cce)
      : gr::block("pdcch_mux",
              gr::io_signature::make(1, n_pdcch, sizeof(uint8)),
              gr::io_signature::make(1, 1, sizeof(uint8))),
        d_n_pdcch(n_pdcch),
        N_CCE(n_cce)
    {
        set_tag_propagation_policy(TPP_DONT);
    }

    pdcch_mux_impl::~pdcch_mux_impl()
    {
    }

    void
    pdcch_mux_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = N_CCE*8*9;   // about the whole CCE available for tx
    }

    int
    pdcch_mux_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        vector<const uint8*> in(d_n_pdcch);
        for (int i=0; i<d_n_pdcch; i++)
        {
            in[i] = (const uint8 *) input_items[i];
        }
        uint8 *out = (uint8 *) output_items[0];

        // reading tags
        vector<tag_t> len_tag;
        vector<int> in_len;

        for (int i=0; i<d_n_pdcch; i++)
        {
            get_tags_in_window(len_tag,
                               i,
                               0,
                               ninput_items[i],
                               pmt::string_to_symbol("length"));

            if (len_tag.size()>0)
            {
                in_len.push_back(pmt::to_long(len_tag[0].value));

                // checking aggregation levels
                if (!(in_len[i] == 72 || in_len[i] == 144 ||
                     in_len[i] == 288|| in_len[i] == 576))
                {
                        throw runtime_error("The aggregation level isn't correct");
                }

            }
        }
        // filling output vector in decreasing order of input lengths
        int nwritten = 0;
        if (in_len.size() == d_n_pdcch)     // all the input tags read
        {
            // checking enough output items
            if (noutput_items >= N_CCE * 9 * 8)
            {
                vector<int> in_len_prime;
                in_len_prime.insert(in_len_prime.begin(), in_len.begin(), in_len.end());

                // for checking all the inputs are consumed
                vector<int> check, check_1;
                check.assign(d_n_pdcch,0);
                check_1.assign(d_n_pdcch, 1);
                // sorting in_len(s) in ascending order
                sort(in_len.begin(), in_len.end());

                int iter = d_n_pdcch -1, pos = 0;
                // copying to output ...
                while (check != check_1)
                {

                    for (int i=0; i<d_n_pdcch; i++)
                    {
                        if (in_len[iter] == in_len_prime[i] & check[i] != 1)
                        {
                            memcpy(out+pos, in[i], in_len[iter] * sizeof(uint8));
                            pos += in_len[iter];
                            check[i] = 1;
                        }
                    }
                    iter--;
                }

                // (<NIL> elements): padding zeros if anything remained from N_CCE * 9 * 8
                vector<uint8> pad_z;
                pad_z.assign(N_CCE*9*8-pos,0);
                memcpy(out+pos, &pad_z[0], (N_CCE*9*8-pos) * sizeof(uint8));

                nwritten = N_CCE * 9 * 8;

                // adding tag ...
                add_item_tag(0,
                             nitems_written(0),
                             pmt::string_to_symbol("length"),
                             pmt::from_long(nwritten));

                //            cout << "pdcch mux" << endl;
                // consuming inputs
                for (int i=0 ; i<d_n_pdcch; i++)
                {
                    consume(i, in_len_prime[i]);
                }
            }
            else
            {
                cout << "<pdcch_mux Block> Not enough output items" << endl;
            }
        }
        else
        {
            consume_each(0);
            throw runtime_error("Not enough input tags");
        }

        return nwritten;
    }

  } /* namespace my_lte */
} /* namespace gr */

