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


#ifndef INCLUDED_MY_LTE_PDCCH_MUX_H
#define INCLUDED_MY_LTE_PDCCH_MUX_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief This block according to 3GPP TS 36.211 version 10.1.0 Release 10 section 6.8.2,
     *      multiplexes different DCIs of different users with different aggeregation levels
     *      defined in 3GPP TS 36.211 version 10.1.0 Release 10 Table 6.8.1-1.
     * \attention This block insert "zeros" instead of <NIL> elements.
     * \section
     *      \param n_pdcch
     *          Number of PDCCH available for transmission in this subframe.
     *      \param n_cce
     *          Number of CCE available for tranmission of these PDCCH defined in 3GPP TS 36.211
     *          version 10.1.0 Release 10 section 6.8.1.
     * \ingroup my_lte
     *
     */
    class MY_LTE_API pdcch_mux : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pdcch_mux> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::pdcch_mux.
       *
       * To avoid accidental use of raw pointers, my_lte::pdcch_mux's
       * constructor is in a private implementation
       * class. my_lte::pdcch_mux::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n_pdcch, int n_cce);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PDCCH_MUX_H */

