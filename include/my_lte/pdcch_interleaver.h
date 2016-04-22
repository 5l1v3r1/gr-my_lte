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


#ifndef INCLUDED_MY_LTE_PDCCH_INTERLEAVER_H
#define INCLUDED_MY_LTE_PDCCH_INTERLEAVER_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief This block interleaves PDDCH according to ‫‪3GPP‬‬ ‫‪TS‬‬ ‫‪36.211‬‬ ‫‪version‬‬ ‫‪10.1.0‬‬ ‫‪Release‬‬ ‫‪10‬‬ ‫‪section‬‬ ‫‪6.8.5.
     *      This interleaver is exactly the same as the subblock interleaver used in convolution encoder but
     *      here the interleaver is applied on quadruplets of modulaiton symbols instead of bits.
     *
     * \attention Fixed:
     *                  - Some unkown-length arrays changed to vectors.
     *                  - In the statement 'w_p[idx] = perm_mat[i][j];' the w_p index was increasing twice!
     *                  - All indexes' increments are manipulated carefully.
     *
     * \ingroup my_lte
     *
     */
    class MY_LTE_API pdcch_interleaver : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pdcch_interleaver> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::pdcch_interleaver.
       *
       * To avoid accidental use of raw pointers, my_lte::pdcch_interleaver's
       * constructor is in a private implementation
       * class. my_lte::pdcch_interleaver::make is the public interface for
       * creating new instances.
       */
      static sptr make(int N_ant, int N_cell_id);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_PDCCH_INTERLEAVER_H */

