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


#ifndef INCLUDED_MY_LTE_CRC_ATTACHMENT_H
#define INCLUDED_MY_LTE_CRC_ATTACHMENT_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"
#include <string.h>

using namespace std;


namespace gr {
  namespace my_lte {

    /*!
     * \brief According to 3GPP Ts 36.212 version 10.1.0 section 5.1.1 attaching crc to the
     *      tagged data stream with tag "length". This block propogates data tags and add
     *      its own "length" tag.
     * \section parameters
     *      \param CRC_type
     *          For determining crc type and the possible vslues: CRC16, CRC8, CRC24B, CRC24A.
     *      \param N_ant
     *          Used for BCH channel masking parity bits as stated in 3GPP TS 136 212 in table
     *          5.3.1.1-1.
     *      \param ch_name
     *          The type of the channel. There is just checking it's BCH or not.
     *      \param ninput_needed
     *          This value interpreted as an approximation value for the forecast method to
     *          determine how many input is needed.
     * \attention Fixed:
     *              - The variable 'tmp_array' must be a vector instead of an array.
     * '16-11-2014' - There is a possibility of fewer input/output items at runtime,
     *              so they are checked before performing crc attachment.
     *
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API crc_attachment : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<crc_attachment> sptr;
      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::crc_attatchment.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::crc_attatchment's
       * constructor is in a private implementation
       * class. lte_bch_encode::crc_attatchment::make is the public interface for
       * creating new instances.
       */
        /*! \brief Getting the name of the channel to determine whether is BCH or not
         *         to add parity bits' mask or not
         */

      static sptr make(uint32 CRC_type, int N_ant, string ch_name, int ninput_needed, uint32 rnti);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CRC_ATTACHMENT_H */

#define CRC24A 0x01864CFB
#define CRC24B 0x01800063
#define CRC16  0x00011021
#define CRC8   0x0000019B

