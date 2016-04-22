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
#include "pdcch_interleaver_impl.h"

namespace gr {
  namespace my_lte {

  // permutation indexes for interleaver
  int IC_PERM_CC[32] = { 1,17, 9,25, 5,21,13,29, 3,19,11,27, 7,23,15,31,
                           0,16, 8,24, 4,20,12,28, 2,18,10,26, 6,22,14,30};

    pdcch_interleaver::sptr
    pdcch_interleaver::make(int N_ant, int N_cell_id)
    {
      return gnuradio::get_initial_sptr
        (new pdcch_interleaver_impl(N_ant, N_cell_id));
    }

    pdcch_interleaver_impl::pdcch_interleaver_impl(int N_ant, int N_cell_id)
      : gr::sync_block("pdcch_interleaver",
              gr::io_signature::make(1, N_ant, sizeof(gr_complex)),
              gr::io_signature::make(1, N_ant, sizeof(gr_complex))),
        d_n_ant(N_ant),
        d_cell_id(N_cell_id)
    {}

    pdcch_interleaver_impl::~pdcch_interleaver_impl()
    {
    }

    int
    pdcch_interleaver_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        vector<const gr_complex*> in(d_n_ant);
        vector<gr_complex*> out(d_n_ant);

        int count = 0, pack_len[d_n_ant];

        for (int i=0; i<d_n_ant; i++)
        {
            in[i] = (const gr_complex *) input_items[i];
            out[i] = (gr_complex *) output_items[i];

            // reading tags
            vector<tag_t> tag_len;
            get_tags_in_window(tag_len,
                               i,
                               0,
                               noutput_items,
                               pmt::string_to_symbol("length"));

            if(tag_len.size()>0)
            {
                pack_len[i] = pmt::to_long(tag_len[0].value);
                count ++;
            }
        }

        if (pack_len[0] < noutput_items)
        {
            cout << "<pdcch_interleaver Block> There is not enough input items" << endl;
        }

        int N_out = 0;
        if (count == d_n_ant)   // checking all the input tags available
        {

            for(int n = 0; n<d_n_ant; n++)
            {
                pdcch_interleave(in[n],
                                 pack_len[n],
                                 d_cell_id,
                                 out[n],
                                 &N_out);

                // adding output tag
                add_item_tag(0,
                             nitems_written(n),
                             pmt::string_to_symbol("length"),
                             pmt::from_long(N_out));

            }

//            cout << "PDCCH interleaver" << endl;
        }

        return N_out;
    }

    void pdcch_interleaver_impl::pdcch_interleave(const gr_complex *in,
                                                  int N_input,
                                                  int N_cell_id,
                                                  gr_complex *out,
                                                  int *N_out)
    {
        int C_cc = 32;
        int R_cc = 0;
        int N_dummy = 0;
        int idx  = 0;

        // preparing for interleaving
        int num_quad = N_input/4;
        int* tmp = new int [C_cc+num_quad];       // max of vector length with both input and dummy bits
        vector<int> position;

        for (int i=0; i<num_quad; i++)
            position.push_back(i);

        // interleaver matrix
        R_cc = 0;
        while(num_quad > (C_cc*R_cc))
        {
            R_cc++;
        }

        // FIXME: Variable length arrays!!!
        int sb_mat[100][100];             // max size possible (32,32)
        int perm_mat[100][100];

        // Pack positions into matrix and pad with dummy
        if(num_quad < (C_cc*R_cc))
        {
            N_dummy = C_cc*R_cc - num_quad;
        }else{
            N_dummy = 0;
        }
        for(int i=0; i<N_dummy; i++)
        {
            tmp[i] = TX_NULL_BIT;
        }
        idx = 0;
        for(int i=N_dummy; i<C_cc*R_cc; i++)
        {
            tmp[i] = position[idx];
            idx++;
        }
        idx = 0;
        for(int i=0; i<R_cc; i++)
        {
            for(int j=0; j<C_cc; j++)
            {
                sb_mat[i][j] = tmp[idx];
                idx++;
            }
        }

        // Step 4: Inter-column permutation
        for(int i=0; i<R_cc; i++)
        {
            for(int j=0; j<C_cc; j++)
            {
                perm_mat[i][j] = sb_mat[i][IC_PERM_CC[j]];
            }
        }

        int* w_p ;
        int* W;
        int* W_bar;
        try{
        w_p = new int [R_cc * C_cc];
        if (!w_p) cout << "F 1" << endl;
        W = new int [R_cc*C_cc]; // - N_dummy];
        if (!W) cout << "F 2" << endl;
        W_bar = new int [R_cc*C_cc];  // - N_dummy];
        if (!W_bar) cout << "F 3" << endl;
        }
        catch (...)
          {
            std::cerr << "bad_alloc caught: " << endl;
          }

        idx = 0;
        // Step 5: Read out the posistions
        for(int j=0; j<C_cc; j++)
        {
            for(int i=0; i<R_cc; i++)
            {
                w_p[idx] = perm_mat[i][j];
                idx++;
            }
        }

        // removing NULL elements
        int k = 0;
        for(int i=0; i<idx; i++)
        {
            if (w_p[i] != TX_NULL_BIT)
            {
                W[k] = w_p[i];
                k++;
            }
        }

        // cyclic shift
        for(int i=0; i<R_cc*C_cc - N_dummy ; i++)
        {
            W_bar[i] = W[(i+N_cell_id)%num_quad];
        }

        // producing output
        idx = 0;
        for (int i=0; i<R_cc*C_cc - N_dummy ; i++)
        {
            // quadruplets
            for(int g=0; g<4; g++)
            {
                out[idx] = in[4*W_bar[i]+g];
                idx++;
            }
        }

        *N_out = idx;
        delete [] W;
        delete [] W_bar;
        delete [] w_p;
        delete [] tmp;
    }

  } /* namespace my_lte */
} /* namespace gr */

