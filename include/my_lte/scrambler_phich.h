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


#ifndef INCLUDED_MY_LTE_SCRAMBLER_PHICH_H
#define INCLUDED_MY_LTE_SCRAMBLER_PHICH_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief According to 3GPP TS 36.211 version 10.1.0 Release 10 section 6.9.1, after
     *      orthogonalization of PHICH channels, generated sequences must be scrambled
     *      similar to scrambling of other channels, however, here modulation symbols are
     *      scrambled instead of bit sequences.
     *
     * \ingroup my_lte
     *
     */
    class MY_LTE_API scrambler_phich : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<scrambler_phich> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::scrambler_phich.
       *
       * To avoid accidental use of raw pointers, my_lte::scrambler_phich's
       * constructor is in a private implementation
       * class. my_lte::scrambler_phich::make is the public interface for
       * creating new instances.
       */
       static sptr make(uint32 N_id_cell, short rnti, uint32 set_align);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_SCRAMBLER_PHICH_H */

