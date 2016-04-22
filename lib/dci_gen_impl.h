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

#ifndef INCLUDED_MY_LTE_DCI_GEN_IMPL_H
#define INCLUDED_MY_LTE_DCI_GEN_IMPL_H

#include <my_lte/dci_gen.h>

namespace gr {
  namespace my_lte {

    class dci_gen_impl : public dci_gen
    {
    private:
        string dci_type;
        typedef struct{
            uint32                          tbs;
            uint32                          rv_idx;
            uint32                          N_prb;
            uint32                          prb[2][110];    // 110 is the max. of BW
            uint32                          N_layers;
            uint32                          tx_mode;
            uint16                          rnti;
            int                             mcs;
            uint32                           tpc;
            bool                            ndi;        // new data indicator
            uint8                           N_dl_harq;
            bool                            ci_presence;
            int                             n_antenna;
            int                             N_dl_prb;
        }dci_alloc_struct;

        dci_alloc_struct *alloc;

    public:
        dci_gen_impl(string dci_t,
                   short rnti,
                   bool carrier_ind,
                   uint32 rv_idx,
                   uint32 N_dl_prb,
                   uint32 N_layers,
                   uint32 tx_mode,
                   uint32 N_dl_harq,
                   int mcs,
                   bool ndi, int N_ant, int alloc_len, int RB_start);

      ~dci_gen_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);

      void dci_1a_pack(dci_alloc_struct    *alloc,
                       bool  ci_presence,
                       uint32                           N_rb_dl,
                       uint8                            N_ant,
                       uint8                           *out_bits,
                       uint32                          *N_out_bits);

      void phy_value_2_bits(uint32   value,
                            uint8  **bits,
                            uint32   N_bits);


    };

  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_DCI_GEN_IMPL_H */

#define LIBLTE_MAC_INVALID_RNTI    0x0000
#define LIBLTE_MAC_RA_RNTI_START   0x0001
#define LIBLTE_MAC_RA_RNTI_END     0x003C
#define LIBLTE_MAC_C_RNTI_START    0x003D
#define LIBLTE_MAC_C_RNTI_END      0xFFF3
#define LIBLTE_MAC_RESV_RNTI_START 0xFFF4
#define LIBLTE_MAC_RESV_RNTI_END   0xFFFC
#define LIBLTE_MAC_M_RNTI          0xFFFD
#define LIBLTE_MAC_P_RNTI          0xFFFE
#define LIBLTE_MAC_SI_RNTI         0xFFFF

#define DCI_VRB_TYPE_LOCALIZED   0
#define DCI_VRB_TYPE_DISTRIBUTED 1


