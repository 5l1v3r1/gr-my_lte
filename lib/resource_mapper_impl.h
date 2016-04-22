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

#ifndef INCLUDED_MY_LTE_RESOURCE_MAPPER_IMPL_H
#define INCLUDED_MY_LTE_RESOURCE_MAPPER_IMPL_H

#include <my_lte/resource_mapper.h>
#include <gnuradio/fft/fft.h>
//#include <stdio.h>

using namespace std;

namespace gr {
  namespace my_lte {

    class resource_mapper_impl : public resource_mapper
    {
     private:
        uint32 d_N_id_1, d_N_id_2, d_N_dl_prb, d_N_id_cell;
        uint32 nf, n_time_slot, L, fft_size, nf_past;
        uint32 pbch_len, pdsch_len ,pdsch_len_remained;

        gr_complex ofdm_symb[12*LIBLTE_PHY_N_RB_DL_MAX];
        gr_complex OFDM_sym1[12*LIBLTE_PHY_N_RB_DL_MAX];
        gr_complex OFDM_sym2[12*LIBLTE_PHY_N_RB_DL_MAX];

        // Mapping channels & RS and SS
        gr_complex CRS[4*LIBLTE_PHY_N_RB_DL_MAX];
        gr_complex PSS[62], SSS_0[62], SSS_5[62];
        gr_complex PDSCH[50000];        // as long as possible array of PDSCH symbols
        gr_complex PBCH[960];

        // fft
        gr::fft::fft_complex *d_fft;
        gr_complex fft_ofdm_symb[2048];

     public:
      resource_mapper_impl(uint32 N_id_1, uint32 N_id_2, uint32 N_dl_prb);
      ~resource_mapper_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      void generate_pss(uint32  N_id_2,
                        float  *pss_re,
                        float  *pss_im);
      void symb_to_gr_complex(float *re, float *im, uint32 len, gr_complex *out);

      void generate_sss(uint32 N_id_1,
                        uint32             N_id_2,
                        float *sss_re_0,
                        float             *sss_im_0,
                        float             *sss_re_5,
                        float             *sss_im_5);

      void generate_crs(uint32  N_s,
                        uint32  L,
                        uint32  N_id_cell,
                        uint32  N_sc_rb_dl,
                        float  *crs_re,
                        float  *crs_im);

      void generate_prs_c(uint32  c_init,
                          uint32  len,
                          uint32 *c);

      int output_vec_size(uint32 d_N_dl_prb);

      void resource_mapper(gr_complex *pss,
                           gr_complex *sss,
                           gr_complex *crs,
                           gr_complex *PBCH,
                           gr_complex *PDSCH,
                           uint32 n_pdsch_symb,
                           uint32 N_dl_prb,
                           uint32 n_time_slot,
                           int L,
                           uint32 nf,
                           uint32 N_id_cell,
                           gr_complex *ofdm_symb,
                           uint32 *n_pdsch_mapped,
                           uint32 *n_pbch_mapped,
                           uint32 *n_crs_symb);

      void padding_zero(gr_complex *in, uint32 d_fft_size, uint32 N_dl_rb, gr_complex *out);

      void fft_shift(gr_complex *in, uint32 d_fft_size,gr::fft::fft_complex *fft ,gr_complex *out);

      void control_mapper(uint32 N_dl_prb,
                          uint32 N_id_cell,
                          gr_complex *pdcch,
                          uint32 n_pdcch,
                          gr_complex *phich,
                          uint32 n_phich,
                          gr_complex *pcfich,
                          gr_complex *OFDM_sym0,
                          gr_complex *OFDM_sym1,
                          gr_complex *OFDM_sym2,
                          uint32 *n_pdcch_mapped,
                          uint32 *n_phich_mapped,
                          uint32 *n_pcfich_mapped);

    };


  } // namespace my_lte
} // namespace gr

#endif /* INCLUDED_MY_LTE_RESOURCE_MAPPER_IMPL_H */
