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


#ifndef INCLUDED_MY_LTE_CODE_BLOCK_CONCATENATION_H
#define INCLUDED_MY_LTE_CODE_BLOCK_CONCATENATION_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts.36212 version 10.1.0 section 5.1.5, this block concatenates
     *      the rate-matched bits of PDSCH. This block reads tag of "length" from each input port
     *      and aggregate them to the output port.
     * \section parameters
     *      \param N_code_block
     *          The number of codeblocks for PDSCH which is equevalent to the number of input ports.
     *      \param N_e_bit_max
     *          The maximum possible size of output of a rate match in bits.
     *      \param set_align
     *          This is an important parameter which sets the value for "set_alignment" method of
     *          topblock which controls how many items may needed to output to the port. Because of
     *          a possibility of a large number of output items, it's required to set this value as
     *          large as need. For example it could be 60000 items.
     *
     * \attention Updates:
     * '11-14-2014' - Exceptions added.
     * '16-14-2014' - Added "length" key for reading tags.
     *
     * \ingroup lte_dlsch_encode
     *
     */
    class MY_LTE_API code_block_concatenation : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<code_block_concatenation> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_dlsch_encode::code_block_concatenation.
       *
       * To avoid accidental use of raw pointers, lte_dlsch_encode::code_block_concatenation's
       * constructor is in a private implementation
       * class. lte_dlsch_encode::code_block_concatenation::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_code_block, uint32 N_e_bit_max, uint32 set_align);

//      static buffer_sptr out_buff;
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CODE_BLOCK_CONCATENATION_H */

