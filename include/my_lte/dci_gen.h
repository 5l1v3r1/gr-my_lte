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


#ifndef INCLUDED_MY_LTE_DCI_GEN_H
#define INCLUDED_MY_LTE_DCI_GEN_H

#include <my_lte/api.h>
#include <gnuradio/sync_block.h>
#include "typedefs.h"

using namespace std;

namespace gr {
  namespace my_lte {

    /*!
     * \brief This block, according to the 3GPP TS 36.212 version 10.1.0 Release 10 section 5.3.3.1.3,
     *      construct only DCI format 1A with the following assumption:
     *          1- Carrier indicator isn't used.
     *          2- Only supporting localized and contiguous VRBs (3GPP TS 36.213 version 10.1.0 Release
     *          10, section 7.1.6.3 .
     *          3- Only FFD is supported.
     *          4- TCP command for PUCCH is replaced with 2 bits zeros.
     * \section parameters
     *      \param dci_t
     *          Type of DCI which is now only DCI format 1A is available.
     *      \param rnti
     *          RNTI number assigned to the UE.
     *      \param carrier_ind
     *          Determining carrier indicator presented or not, which is not supported yet. If it is true, the
     *          block interrupted with an exception.
     *      \param rv_idx
     *          Redundancy version which is used in DSCH rate matching. (3GPP TS 36.212 version 10.1.0 Release 10
     *          section 5.1.4.1.2)
     *      \param N_dl_prb
     *          Number of Downlink PRBs
     *      \param N_layers
     *          Number of layers which could be 1,2,4
     *      \param tx_mode
     *          Tranmission mode. (defined in 3GPP TS 36.213 version 10.1.0 Release 10 section 7.1.
     *      \param N_dl_harq
     *          Number of downlink HARQ processes which could be maximum 8 for FDD system.
     *      \param mcs
     *          MCS scheme value which is between 0-31.
     *      \param ndi
     *          New data indicator
     *      \param N_ant
     *          Number of antennas for tx.
     *      \param alloc_len
     *          The length of contiguous RBs allocated.
     *      \param RB_start
     *          Start RB for allocation
     *
     *  \attention For tcp command we put 2 bits 0f "01"! (it needs to think!)
     *
     * \ingroup my_lte
     *
     */
    class MY_LTE_API dci_gen : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<dci_gen> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of my_lte::dci_gen.
       *
       * To avoid accidental use of raw pointers, my_lte::dci_gen's
       * constructor is in a private implementation
       * class. my_lte::dci_gen::make is the public interface for
       * creating new instances.
       */
      static sptr make(string dci_t,
                       short rnti,
                       bool carrier_ind,
                       uint32 rv_idx,
                       uint32 N_dl_prb,
                       uint32 N_layers,
                       uint32 tx_mode,
                       uint32 N_dl_harq,
                       int mcs,
                       bool ndi,
                       int N_ant,
                       int alloc_len, int RB_start);
    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_DCI_GEN_H */

