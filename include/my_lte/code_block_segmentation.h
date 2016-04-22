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


#ifndef INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_H
#define INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts.36212 version 10.1.0 section 5.1.2, segments a crc-attached
     *      trasnport block and add crc of type CRC24B to the segments if the number of output
     *      segments are greater than 1. Currently it just support an output of 3 segment.
     *
     * \attention Updates:
     *              - Fixed: Return 'WORK_CALLED_PRODUCE'.
     *
     * \section parameters
     *      \param
     * \ingroup lte_dlsch_encode
     *
     * \attention Filler bits must be cared. Currently we assumed that there is no filler bits.
     * \attention All the generated codeblocks have the same length.
     *
     */
    class MY_LTE_API code_block_segmentation : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<code_block_segmentation> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_dlsch_encode::code_block_segmentation.
       *
       * To avoid accidental use of raw pointers, lte_dlsch_encode::code_block_segmentation's
       * constructor is in a private implementation
       * class. lte_dlsch_encode::code_block_segmentation::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };


  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CODE_BLOCK_SEGMENTATION_H */

#define CRC24A 0x01864CFB
#define CRC24B 0x01800063
#define CRC16  0x00011021
#define CRC8   0x0000019B

