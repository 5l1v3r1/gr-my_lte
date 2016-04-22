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


#ifndef INCLUDED_MY_LTE_MODULATION_MAPPER_H
#define INCLUDED_MY_LTE_MODULATION_MAPPER_H

#include <my_lte/api.h>
#include <gnuradio/sync_decimator.h>
#include "typedefs.h"
#include <string.h>

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the 3GPP Ts 36.211 version 10.1.0 section 7.1, this block modulates
     *      input bits. It supports BPSK, QPSK, 16QAM, 64QAM modultions.
     * \section parameters
     *      \param mod_type
     *          Type of modulation. Possible values: "BPSK", "QPSK", "16QAM", "64QAM" .
     *      \param set_align
     *          This is an important parameter which sets the value for "set_alignment" method of
     *          topblock which controls how many items may needed to output to the port. Because of
     *          a possibility of a large number of output items, it's required to set this value as
     *          large as need. For example it could be 60000 items.
     * \ingroup lte_bch_encode
     *
     * \attention Fixed bugs:
     *                      - float variables d_im, d_re was deleted after "new" operation.
     *                      - ninput_items_required[0] must be noutput_items/samps_per_symb.
     *                      - There is a possiblity of fewer number of input items than the
     *                      input packet length! Releasing memory of d_re and d_im must be
     *                      where they are allocated.
     */
    class MY_LTE_API modulation_mapper : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<modulation_mapper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_bch_encode::modulation_mapper.
       *
       * To avoid accidental use of raw pointers, lte_bch_encode::modulation_mapper's
       * constructor is in a private implementation
       * class. lte_bch_encode::modulation_mapper::make is the public interface for
       * creating new instances.
       */
      static sptr make(string mod_type, uint32 set_align);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_MODULATION_MAPPER_H */

