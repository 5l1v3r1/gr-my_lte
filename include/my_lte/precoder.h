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


#ifndef INCLUDED_MY_LTE_PRECODER_H
#define INCLUDED_MY_LTE_PRECODER_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to 3GPP Ts 36.211 version 10.1.0 section 6.3.4.1 & 6.3.4.3,
     *      precodes one codeword to layers. It just supports single antenna and Tx
     *      diversity modes.
     * \section parameters
     *      \param N_ant
     *          Number of antennas.
     *      \param set_align
     *          This is an important parameter which sets the value for "set_alignment" method of
     *          topblock which controls how many items may needed to output to the port. Because of
     *          a possibility of a large number of output items, it's required to set this value as
     *          large as need. For example it could be 60000 items.
     *
     *  \attention Fixed bugs:
     *          - Precoding for N ant = 4.
     *          - "return" of the work call is fixed by using produce.
     *
     *\attention Added PHICH precoder for 4 antennas as 3GPP TS 36.211 version 10.1.0 Release 10
     *      section 6.9.2. New parameters as follow:
     *      \param is_PHICH
     *          Determining whether the channel is PHICH or not.
     *      \param n_phich_group
     *          The PHICH group number.
     *      \param normal_cp
     *          Determining whether cp is normal or not.
     *
     * \attention Updates:
     * '11-14-2014' - Exceptions added.
     *
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API precoder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<precoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::precoder.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::precoder's
       * constructor is in a private implementation
       * class. lte_bch_encode::precoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_ant, uint32 set_align, bool is_phich, int n_phich_group, bool normal_cp);
    };


  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PRECODER_H */

