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
#include "cp_adder_impl.h"

namespace gr {
  namespace my_lte {

    cp_adder::sptr
    cp_adder::make(uint32 N_dl_prb, bool normal_cp)
    {
      return gnuradio::get_initial_sptr
        (new cp_adder_impl(N_dl_prb, normal_cp));
    }

    cp_adder_impl::cp_adder_impl(uint32 N_dl_prb, bool normal_cp)
      : gr::block("cp_adder",
              gr::io_signature::make(1, 1, output_vec_size(N_dl_prb)*sizeof(gr_complex)),
                  gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_normal_cp(normal_cp)

    {
        set_tag_propagation_policy(TPP_DONT);
        set_max_noutput_items(fft_size+400);
    }

    int cp_adder_impl::output_vec_size(uint32 d_N_dl_prb)
    {
        if (d_N_dl_prb < 12)
        {
            fft_size = 128;
            cp_l = 10;
            cp_s = 9;
            cp_e = 32;
            return fft_size;
        }
        else if (d_N_dl_prb < 25)
        {
            fft_size = 256;
            cp_l = 20;
            cp_s = 18;
            cp_e = 64;
            return fft_size;
        }
        else if (d_N_dl_prb < 50)
        {
            fft_size = 512;
            cp_l = 40;
            cp_s = 36;
            cp_e = 128;
            return fft_size;
        }
        else if (d_N_dl_prb < 75)
        {
            fft_size = 1024;
            cp_l = 80;
            cp_s = 72;
            cp_e = 256;
            return fft_size;
        }
        else if (d_N_dl_prb < 110)
        {
            fft_size = 1536;
            cp_l = 120;
            cp_s = 108;
            cp_e = 384;
            return fft_size;
        }
        else
        {
            fft_size = 2048;
            cp_l = 160;
            cp_s = 144;
            cp_e = 512;
            return fft_size;
        }
    }

    cp_adder_impl::~cp_adder_impl()
    {
    }

    void
    cp_adder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = 1;
    }

    int
    cp_adder_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];


        if (ninput_items[0]>0)
        {
            if (d_normal_cp)
            {
                vector<tag_t> L_tag;
                get_tags_in_window(L_tag,
                                   0,
                                   0,
                                   ninput_items[0],
                                   pmt::string_to_symbol("L"));

                if (noutput_items >= fft_size + cp_l)
                {
                    if(L_tag.size()>0)
                    {
                        int L = pmt::to_long(L_tag[0].value);
                        if (L != 0)
                        {
                            memcpy(out, &in[fft_size-cp_s], cp_s* sizeof(gr_complex));
                            memcpy(&out[cp_s],in,fft_size*sizeof(gr_complex));
                            produce(0, fft_size + cp_s);
                            consume_each (1);
                        }
                        else
                        {
                            memcpy(out, &in[fft_size-cp_l], cp_l* sizeof(gr_complex));
                            memcpy(&out[cp_l],in,fft_size*sizeof(gr_complex));
                            produce(0, fft_size + cp_l);
                            consume_each (1);
                        }

                        //                    cout << "\ncp adder" << endl;
                    }
                    else
                    {
                        throw runtime_error("No tags read");
                    }
                }
                else
                {
                    produce(0, 0);
                    consume_each (0);
//                    throw runtime_error("No enough output items");
                }
            }
            else
            {
                if (noutput_items >= fft_size + cp_e)
                {
                // extended cp
                memcpy(out, &in[fft_size-cp_e], cp_e* sizeof(gr_complex));
                memcpy(&out[cp_e],in,fft_size*sizeof(gr_complex));
                produce(0, fft_size + cp_e);
                consume_each (1);
                }
                else
                {
                    produce(0, 0);
                    consume_each (0);
//                    throw runtime_error("Not enough output items");
                }
            }
        }

        // Tell runtime system how many output items we produced.
        return WORK_CALLED_PRODUCE;
    }

  } /* namespace my_lte */
} /* namespace gr */

