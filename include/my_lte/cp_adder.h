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


#ifndef INCLUDED_MY_LTE_CP_ADDER_H
#define INCLUDED_MY_LTE_CP_ADDER_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief Add CP based on the number of downlink PRBs and whether CP is normal or extended.
     *
     * \attention Bugs fixed:
     *              - In determining the fft size and cp lengths in conjunction with the number of downlink PRBs.
     * '21-11-2014' - Fixe bug: A check for enough output items is added.
     *
     * \ingroup my_lte
     *
     */
    class MY_LTE_API cp_adder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<cp_adder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::cp_adder.
       *
       * To avoid accidental use of raw pointers, my_lte::cp_adder's
       * constructor is in a private implementation
       * class. my_lte::cp_adder::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_dl_prb, bool normal_cp);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_CP_ADDER_H */

