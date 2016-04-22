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


#ifndef INCLUDED_MY_LTE_SCRAMBLER_H
#define INCLUDED_MY_LTE_SCRAMBLER_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>
#include "typedefs.h"

//typedef char               int8;
typedef unsigned char      uint8;
//typedef short              int16;
typedef unsigned short     uint16;
//typedef int                int32;
typedef unsigned int       uint32;
//typedef long long          int64;
//typedef unsigned long long uint64;

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to 3GPP Ts 36.211 version 10.1.0 section 6.3.1, this block scrambles
     *      tagged stream with tag "length". It can be used for all channel types inc. PBCH,
     *      PDSCH, DCI, PCFICH, PHICH.
     * \section parameters
     *      \param N_id_cell
     *      Physical layer cell identity which is used for initialization the scrambler for
     *      all channels at the start of each frame: 3GPP Ts 36.211 version 10.1.0 sections
     *      6.6.1, 6.3.1, 6.7.1, 6.8.2, 6.9.1 .
     *      \param name
     *      Identifying the name of the channel with possible balues: "PBCH", "PDSCH", "DCI",
     *      "PCFICH".
     *      \param rnti
     *      Identifying the initialization of the scrambler for PDSCH channel: 3GPP Ts 36.211
     *      version 10.1.0 section 6.3.1 . For other channels it's don't care.
     *      \param set_align
     *          This is an important parameter which sets the value for "set_alignment" method of
     *          topblock which controls how many items may needed to output to the port. Because of
     *          a possibility of a large number of output items, it's required to set this value as
     *          large as need. For example it could be 60000 items.
     *
     *  \attention Updates:
     *          - PHICH added. See 3GPP TS 36.211 version 10.1.0 Release 10 section 6.9.1. The input
     *      and output item sizes would be changed in this channel case.
     *          - The "PMCH" channel is added.
     * '11-14-2014' - Added exceptions.
     *
     *  \attention Fixed problems:
     *              - Initialization of the scrambling sequence is corrected. ( (2*count)%20 instead of count%20).
     *              Consider that for every channel other than PBCH, we must initialize the sequence at start of
     *              each subframe but the value of "cinit" depedons on time slot number which is equals to twice of
     *              the "count".
     * '11-14-2014' - Determining the input packet size by checking the availability of tags.
     *
     *
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API scrambler : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<scrambler> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::scrambler.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::scrambler's
       * constructor is in a private implementation
       * class. lte_bch_encode::scrambler::make is the public interface for
       * creating new instances.
       */

        /*!
         * \brief   The name determining the channel name
         *          N_id_cell is used for DCI and CFI
         *          rnti is just used for PDSCH's scrambler initialization
         */
      static sptr make(uint32 N_id_cell, string name, short rnti, uint32 set_align);

    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_SCRAMBLER_H */

