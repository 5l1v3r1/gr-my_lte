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


#ifndef INCLUDED_MY_LTE_RATE_MATCH_CONV_H
#define INCLUDED_MY_LTE_RATE_MATCH_CONV_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts 36.212 version 10.1.0 section 5.1.4.2 this block rate matchs for
     *      convolutionally encoded tagged data stream with tag "length".
     *        It can be used for BCH and DCI channels: 3GPP Ts 36.212 version 10.1.0 table 5.1.3-1
     * \section parameters
     *      \param N_e_bits
     *          The number of output bits of rate match.
     *
     * \attention Updates:
     * '11-14-2014' - Exceptions added.
     * '16-14-2014' - Fixed bug: Packet length must be checked to be non zero before rate matching.
     * '20-14-2014' - Fixed bug: out bits are first returned to a pre-defined variable to avoid the
     *              condition of insufficient output items.
     *
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API rate_match_conv : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<rate_match_conv> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::rate_match_conv.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::rate_match_conv's
       * constructor is in a private implementation
       * class. lte_bch_encode::rate_match_conv::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_e_bits);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RATE_MATCH_CONV_H */

