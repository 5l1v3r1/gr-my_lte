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


#ifndef INCLUDED_MY_LTE_PHICH_GROUPING_H
#define INCLUDED_MY_LTE_PHICH_GROUPING_H

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

using namespace  std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief ‫‪3GPP‬‬ ‫‪TS‬‬ ‫‪36.211‬‬ ‫‪version‬‬ ‫‪10.1.0‬‬ ‫‪Release‬‬ ‫‪10‬‬ ‫‪section‬‬ ‫‪6.9.3‬‬, PHICH channels must
     *      be gathered in groups to be transmitted.
     *
     * \attention
     * '19-11-2014' - Fixed bugs: In tagging output length.
     *
     *
     * \ingroup my_lte
     *
     */
    class MY_LTE_API phich_grouping : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<phich_grouping> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::phich_grouping.
       *
       * To avoid accidental use of raw pointers, my_lte::phich_grouping's
       * constructor is in a private implementation
       * class. my_lte::phich_grouping::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool normal_cp, int n_phich_group);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PHICH_GROUPING_H */

