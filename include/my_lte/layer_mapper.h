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


#ifndef INCLUDED_MY_LTE_LAYER_MAPPER_H
#define INCLUDED_MY_LTE_LAYER_MAPPER_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to 3GPP Ts 36.211 version 10.1.0 section 6.3.3.1 & 6.3.3.3,
     *      mapps codeword to layers. It just supports single antenna and Tx
     *      diversity modes. It can be used for physical all channels.
     * \section parameters
     *      \param N_ant
     *      Number of antennas
     *      \param set_align
     *          This is an important parameter which sets the value for "set_alignment" method of
     *          topblock which controls how many items may needed to output to the port. Because of
     *          a possibility of a large number of output items, it's required to set this value as
     *          large as need. For example it could be 60000 items.
     *
     *\attention NULL symbols insertion is added for the case N ant = 4.
     *
     *\attention You must care about number of outputs
     *
     * \bug Fixed bugs:
     *          - In adding tags of outputs for number of antennas greater than 1, the position of tagging
     *              is corrected which was "nitems_written(0)," for all outputs!
     *
     * \attention Updates:
     * '11-14-2014' - Exceptions added.
     *
     * \ingroup lte_bch_encode
     *
     */
    class MY_LTE_API layer_mapper : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<layer_mapper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::layer_mapper.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::layer_mapper's
       * constructor is in a private implementation
       * class. lte_bch_encode::layer_mapper::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_ant, uint32 set_align);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_LAYER_MAPPER_H */

