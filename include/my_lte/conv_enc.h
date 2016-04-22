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


#ifndef INCLUDED_MY_LTE_CONV_ENC_H
#define INCLUDED_MY_LTE_CONV_ENC_H

#include <my_lte/api.h>
#include <gnuradio/sync_interpolator.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts 36.212 version 10.1.0 section 5.1.3.1 this block
     *      convolutionally encode tagged data stream with tag "length".
     * \section parameters
     *      \param constraint_length
     *          The constraint length of encoder which is 7 for 3GPP standard.
     *      \param rate
     *          Encoder rate (here inverse of it) i.e. n/k for example 3.
     *      \param g
     *          Matrix generator in decimal representation rather than in octal or binary.
     *      \param tail_bit
     *          Doing tail bitting or not.
     *
     * \attention Updates:
     *              - Fixed bug: A new variable for the output of encoder was added.
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API conv_enc : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<conv_enc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::conv_enc.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::conv_enc's
       * constructor is in a private implementation
       * class. lte_bch_encode::conv_enc::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 constraint_length,
                       uint32 rate,
                       vector<uint32> g,
                       bool tail_bit);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CONV_ENC_H */

