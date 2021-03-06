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


#ifndef INCLUDED_MY_LTE_TURBO_ENCODER_H
#define INCLUDED_MY_LTE_TURBO_ENCODER_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to the section 3GPP Ts.36212 version 10.1.0 section 5.1.3.2, turbo encodes the
     *      input tagged data stream with key "length" and add tag to the output the length of generated
     *      bits with key "length".
     * \attention This block carry out filler bits dealings as described in 3GPP Ts.36212 version 10.1.0
     *          section 5.1.3.2.1 . So at the receiver we must take care of them!
     * \section parameters
     * \ingroup lte_dlsch_encode
     *
     */
    class MY_LTE_API turbo_encoder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<turbo_encoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte_dlsch_encode::turbo_encoder.
       *
       * To avoid accidental use of raw pointers, lte_dlsch_encode::turbo_encoder's
       * constructor is in a private implementation
       * class. lte_dlsch_encode::turbo_encoder::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };


  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_TURBO_ENCODER_H */

#define TX_NULL_BIT 100
