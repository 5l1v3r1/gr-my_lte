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


#ifndef INCLUDED_MY_LTE_RESOURCE_MAPPER_H
#define INCLUDED_MY_LTE_RESOURCE_MAPPER_H

#define LIBLTE_PHY_N_RB_DL_MAX 110

#include <my_lte/api.h>
#include <gnuradio/block.h>
#include "typedefs.h"

namespace gr {
  namespace my_lte {

    /*!
     * \brief Accoridng to 3GPP TS 36.211 version 10.1.0 Release 10, this block maps received
     *      symbols to resource elements with the following assumption:
     *      - Single antenna mapping
     *      - Needs entire PBCH for 4 frames to map.
     *      - Only cell-specific reference signals are mapped.
     *      - Normal cyclic prefix
     *      - Control channel region is always 3 OFDM symbol duration.
     *
     *
     * \attention Fixed problems:
     *          - Reference signal resource mapping is corrected.
     *          - Skipping cell-specific RS of port 0 and 1 for mapping control channels
     *          - Reading tags of pdsch (ninput_items[1] instead of ninput_items[0])
     *          - Fixed the size of OFDM_sym1 for control channels
     *
     *
     * \attention Updates:
     *          - Two new functions added: "fft_shift" & "padding_zero"
     *          - Control channel mappings are added
     *          - CRS genereation is fixed at L=0 & L=4
     *          - Determining fft size for 50 and 75 PRBs was wrong.
     *          - The max size must be allocated to CRS for its generation.
     *          - Fixed bug: fft object is created right before and after fft operation
     *                  to release memory.
     *          - Fixed bug: "L" input to the crs generator must be in mode 7!!
     *          - Fixed bug: Control resource mapping is updated.
     *          - Fixed bug: Padding zero of the second half of the OFDM symbol wasn't zeros.
     *          - Fixed bug: Resource mapper and control mapper was updated.
     *
     * \ingroup my_lte
     *
     * \attention
     *
     */
    class MY_LTE_API resource_mapper : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<resource_mapper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::resource_mapper.
       *
       * To avoid accidental use of raw pointers, my_lte::resource_mapper's
       * constructor is in a private implementation
       * class. my_lte::resource_mapper::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32 N_id_1, uint32 N_id_2, uint32 N_dl_prb);

//      int output_vec_size(uint32 d_N_dl_prb);

    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RESOURCE_MAPPER_H */

