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


#ifndef INCLUDED_MY_LTE_CFI_GEN_H
#define INCLUDED_MY_LTE_CFI_GEN_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to ‫‪3GPP‬‬ ‫‪TS‬‬ ‫‪36.212‬‬ ‫‪version‬‬ ‫‪10.1.0‬‬ ‫‪Release‬‬ ‫‪10‬‬ ‫‪section‬‬ 5.3.4‬‬, CFI value determines the number of
     *      OFDM symbols which are allocated to control channels at the start of each subframe. Here we've assumed that
     *      the control channels are mapped within the first 3 OFDM symbols in all subframes.
     * \ingroup my_lte
     *
     */
    class MY_LTE_API cfi_gen : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<cfi_gen> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::cfi_gen.
       *
       * To avoid accidental use of raw pointers, my_lte::cfi_gen's
       * constructor is in a private implementation
       * class. my_lte::cfi_gen::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_dl_prb, int cfi_val);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CFI_GEN_H */

