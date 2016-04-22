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


#ifndef INCLUDED_MY_LTE_RATE_MATCH_H
#define INCLUDED_MY_LTE_RATE_MATCH_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts.36212 version 10.1.0 section 5.1.4.1, rate matches the input bits.
     *      This blocks read the turbo encoded input bits which are tagged with key "length" and interleaves
     *      them and put them into the circular buffer and produce output. Also tags length of output bits
     *      with the key "length".
     * \section parameters
     *      \param N_e_bits
     *          The number of output bits which is computed by procedure introduced in 3GPP Ts.36212 version
     *          10.1.0 section 5.1.4.1.2.
     *      \param N_code_block
     *          Number of code blocks
     *      \param tx_mode
     *          Transmission mode of PDSCH which could be 0-9.
     *      \param N_soft
     *          Total number of soft bit channels defined in 3GPP Ts.136306 version 10.1.0 section 4.1 table 4.1.1
     *      \param M_dl_harq
     *          Max number of HARQ processes defined in 3GPP Ts.136213 version 10.1.0 section 7.
     *      \param rv_idx
     *          Redundancy version of rate matching. It can be 0, 1, 2 or 3 which is defined in 3GPP 136 212
     *          section 5.1.4.1.2 .
     * \attention The distinguish between "PDSCH" and "PMCH" channels was added.
     *      \param chan_name
     *          The channel name which is either "PDSCH"  or "PMCH"
     *
     * \attention Updates:
     * '11-14-2014' - Exceptions added.
     *
     * \ingroup lte_dlsch_encode
     *
     */
    class MY_LTE_API rate_match : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<rate_match> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_dlsch_encode::rate_match.
       *
       * To avoid accidental use of raw pointers, lte_dlsch_encode::rate_match's
       * constructor is in a private implementation
       * class. lte_dlsch_encode::rate_match::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_e_bits,
                       uint32 N_code_block,
                       int tx_mode,
                       uint32 N_soft,
                       int M_dl_harq,
                       int rv_idx,
                       string chan_name);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RATE_MATCH_H */

#define TX_NULL_BIT 100

