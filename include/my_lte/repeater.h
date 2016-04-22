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


#ifndef INCLUDED_MY_LTE_REPEATER_H
#define INCLUDED_MY_LTE_REPEATER_H

#include <my_lte/api.h>
#include <gnuradio/sync_interpolator.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief Used for channel coding of PHICH according to 3GPP TS 36.212 version 10.1.0 Release 10
     *      section 5.3.5.1 .
     * \ingroup my_lte
     *
     */
    class MY_LTE_API repeater : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<repeater> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::repeater.
       *
       * To avoid accidental use of raw pointers, my_lte::repeater's
       * constructor is in a private implementation
       * class. my_lte::repeater::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_REPEATER_H */

