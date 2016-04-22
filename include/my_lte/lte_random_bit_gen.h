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


#ifndef INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_H
#define INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief Generates random bits and tag the stream for stream size of "packet_len" with
     *      the "length" key. This is used just for simulations.
     * \section parameters
     *      \param packet_len
     *          Determines the output length which is tagged to the stream with key "length".
     *      \param usleep_val
     *          The duration of sleep for this block in microseconds.
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API lte_random_bit_gen : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<lte_random_bit_gen> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::lte_random_bit_gen.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::lte_random_bit_gen's
       * constructor is in a private implementation
       * class. lte_bch_encode::lte_random_bit_gen::make is the public interface for
       * creating new instances.
       */
      static sptr make(int packet_len, uint32 usleep_val);
    };
  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_LTE_RANDOM_BIT_GEN_H */

