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
#include "cfi_gen_impl.h"
#include <unistd.h>

namespace gr {
namespace my_lte {

uint8 CFI_BITS_1[32] = {0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1};
uint8 CFI_BITS_2[32] = {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0};
uint8 CFI_BITS_3[32] = {1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1};
uint8 CFI_BITS_4[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

cfi_gen::sptr
cfi_gen::make(uint32 N_dl_prb, int cfi_val)
{
    return gnuradio::get_initial_sptr
            (new cfi_gen_impl(N_dl_prb, cfi_val));
}

/*
     * The private constructor
     */
cfi_gen_impl::cfi_gen_impl(uint32 N_dl_prb, int cfi_val)
    : gr::sync_block("cfi_gen",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(uint8))),
      d_N_dl_prb(N_dl_prb),
      d_cfi(cfi_val)
{
    set_output_multiple(32);
}

/*
     * Our virtual destructor.
     */
cfi_gen_impl::~cfi_gen_impl()
{
}

int
cfi_gen_impl::work(int noutput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
    uint8 *out = (uint8 *) output_items[0];

    usleep(2000);

    if (d_cfi == 1)
        memcpy(out, CFI_BITS_1, 32*sizeof(uint8));
    else if (d_cfi == 2)
        memcpy(out, CFI_BITS_2, 32*sizeof(uint8));
    else if (d_cfi == 3)
        memcpy(out, CFI_BITS_3, 32*sizeof(uint8));
    else
        throw runtime_error("CFI value isn't corret: must be 1,2,3");

    // adding length tag
    add_item_tag(0,
                 nitems_written(0),
                 pmt::string_to_symbol("length"),
                 pmt::from_long(32));

    //    cout << "CFI generator" << endl;
    // Tell runtime system how many output items we produced.
    return 32;
}

} /* namespace my_lte */
} /* namespace gr */

