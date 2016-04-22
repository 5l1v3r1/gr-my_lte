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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "resource_mapper_impl.h"
#include <vector>

namespace gr {
  namespace my_lte {


    resource_mapper::sptr
    resource_mapper::make(uint32 N_id_1, uint32 N_id_2, uint32 N_dl_prb)
    {
      return gnuradio::get_initial_sptr
        (new resource_mapper_impl(N_id_1, N_id_2, N_dl_prb));
    }

    /*
     * The private constructor
     */
    resource_mapper_impl::resource_mapper_impl(uint32 N_id_1, uint32 N_id_2, uint32 N_dl_prb)
      : gr::block("resource_mapper",
              gr::io_signature::make(1, 5, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, output_vec_size(N_dl_prb)*sizeof(gr_complex))),   // output is a vector of fixed length = N_dl_prb
        d_N_id_1(N_id_1),
        d_N_id_2(N_id_2),
        d_N_dl_prb(N_dl_prb),
        d_N_id_cell(3*N_id_1 + N_id_2),
        nf(0),          // frame number
        n_time_slot(0), // time slot number
        L(0),            // OFDM symbol number
        pbch_len(0),    // for keeping the size of PBCH
        pdsch_len(0),   // for keeping the size of PDSCH
        pdsch_len_remained(0),   // for counting how many PDSCH is remained
        nf_past(1)          // for checking if the next frame is started
    {
        set_tag_propagation_policy(TPP_DONT);

        // generating PSS
        float pss_re[62], pss_im[62];

        generate_pss(d_N_id_2, pss_re, pss_im);
            // to gr_complex
        symb_to_gr_complex(pss_re, pss_im, 62, PSS);

        // generating SSS
        float sss_re_0[62], sss_im_0[62], sss_re_5[62], sss_im_5[62];

        generate_sss(d_N_id_1, d_N_id_2, sss_re_0, sss_im_0, sss_re_5, sss_im_5);
            // to gr_complex
        symb_to_gr_complex(sss_re_0, sss_im_0, 62, SSS_0);
        symb_to_gr_complex(sss_re_5, sss_im_5, 62, SSS_5);

        // message ports
        message_port_register_out(pmt::mp("pbch"));
        message_port_register_out(pmt::mp("pdsch"));

//        vector<int> a;
//        a.push_back(1);
//        a.push_back(2);
//        set_processor_affinity(a);

//        set_thread_priority(10000);
        // fft object
        d_fft = new gr::fft::fft_complex(fft_size, false, 1);

//        my_file1 = fopen("fft_lte_tx.dat", "wb");
//        my_file2 = fopen("lte_tx.dat", "wb");

    }

    int resource_mapper_impl::output_vec_size(uint32 d_N_dl_prb)
    {
        if (d_N_dl_prb < 12)
        {
            fft_size = 128;
            return fft_size;
        }
        else if (d_N_dl_prb < 25)
        {
            fft_size = 256;
            return fft_size;
        }
        else if (d_N_dl_prb < 50)
        {
            fft_size = 512;
            return fft_size;
        }
        else if (d_N_dl_prb < 75)
        {
            fft_size = 1024;
            return fft_size;
        }
        else if (d_N_dl_prb < 75)
        {
            fft_size = 1536;
            return fft_size;
        }
        else
        {
            fft_size = 2048;
            return fft_size;
        }
    }

    void resource_mapper_impl::generate_pss(uint32  N_id_2,
                                            float  *pss_re,
                                            float  *pss_im)
    {
        float  root_idx;
        uint32 i;

        if(N_id_2 == 0)
        {
            root_idx = 25;
        }else if(N_id_2 == 1){

            root_idx = 29;
        }else{
            root_idx = 34;
        }

        for(i=0; i<31; i++)
        {
            pss_re[i] = cosf(-M_PI*root_idx*i*(i+1)/63);
            pss_im[i] = sinf(-M_PI*root_idx*i*(i+1)/63);
        }
        for(i=31; i<62; i++)
        {
            pss_re[i] = cosf(-M_PI*root_idx*(i+1)*(i+2)/63);
            pss_im[i] = sinf(-M_PI*root_idx*(i+1)*(i+2)/63);
        }
    }

    void resource_mapper_impl::symb_to_gr_complex(float *re, float *im, uint32 len, gr_complex *out)
    {
        for (uint32 i=0; i<len; i++)
        {
            out->real(*re);
            out->imag(*im);

            //            cout << *out << endl;
            out++;
            re++;
            im++;
        }
    }

    void resource_mapper_impl::generate_sss(uint32             N_id_1,
                                            uint32             N_id_2,
                                            float             *sss_re_0,
                                            float             *sss_im_0,
                                            float             *sss_re_5,
                                            float             *sss_im_5)
    {
        uint32 i;
        uint32 q_prime;
        uint32 q;
        uint32 m_prime;
        uint32 m0;
        uint32 m1;

        // SSS
//        float sss_mod_re_0[168][d_N_dl_prb* 12];
//        float sss_mod_im_0[168][d_N_dl_prb* 12];
//        float sss_mod_re_5[168][d_N_dl_prb* 12];
//        float sss_mod_im_5[168][d_N_dl_prb* 12];
        //            float sss_re_0[63];
        //            float sss_im_0[63];
        //            float sss_re_5[63];
        //            float sss_im_5[63];
        uint8 sss_x_s_tilda[31];
        uint8 sss_x_c_tilda[31];
        uint8 sss_x_z_tilda[31];
        int8  sss_s_tilda[31];
        int8  sss_c_tilda[31];
        int8  sss_z_tilda[31];
        int8  sss_s0_m0[31];
        int8  sss_s1_m1[31];
        int8  sss_c0[31];
        int8  sss_c1[31];
        int8  sss_z1_m0[31];
        int8  sss_z1_m1[31];

        // Generate m0 and m1
        q_prime = N_id_1/30;
        q       = (N_id_1 + (q_prime*(q_prime+1)/2))/30;
        m_prime = N_id_1 + (q*(q+1)/2);
        m0      = m_prime % 31;
        m1      = (m0 + (m_prime/31) + 1) % 31;

        // Generate s_tilda
        memset( sss_x_s_tilda, 0, sizeof(uint8)*31);
        sss_x_s_tilda[4] = 1;
        for(i=0; i<26; i++)
        {
            sss_x_s_tilda[i+5] = ( sss_x_s_tilda[i+2] +
                                   sss_x_s_tilda[i]) % 2;
        }
        for(i=0; i<31; i++)
        {
            sss_s_tilda[i] = 1 - 2* sss_x_s_tilda[i];
        }

        // Generate c_tilda
        memset( sss_x_c_tilda, 0, sizeof(uint8)*31);
        sss_x_c_tilda[4] = 1;
        for(i=0; i<26; i++)
        {
            sss_x_c_tilda[i+5] = ( sss_x_c_tilda[i+3] +
                                   sss_x_c_tilda[i]) % 2;
        }
        for(i=0; i<31; i++)
        {
            sss_c_tilda[i] = 1 - 2* sss_x_c_tilda[i];
        }

        // Generate z_tilda
        memset( sss_x_z_tilda, 0, sizeof(uint8)*31);
        sss_x_z_tilda[4] = 1;
        for(i=0; i<26; i++)
        {
            sss_x_z_tilda[i+5] = ( sss_x_z_tilda[i+4] +
                                   sss_x_z_tilda[i+2] +
                                   sss_x_z_tilda[i+1] +
                                   sss_x_z_tilda[i]) % 2;
        }
        for(i=0; i<31; i++)
        {
            sss_z_tilda[i] = 1 - 2* sss_x_z_tilda[i];
        }

        // Generate s0_m0 and s1_m1
        for(i=0; i<31; i++)
        {
            sss_s0_m0[i] =  sss_s_tilda[(i + m0) % 31];
            sss_s1_m1[i] =  sss_s_tilda[(i + m1) % 31];
        }

        // Generate c0 and c1
        for(i=0; i<31; i++)
        {
            sss_c0[i] =  sss_c_tilda[(i + N_id_2) % 31];
            sss_c1[i] =  sss_c_tilda[(i + N_id_2 + 3) % 31];
        }

        // Generate z1_m0 and z1_m1
        for(i=0; i<31; i++)
        {
            sss_z1_m0[i] =  sss_z_tilda[(i + (m0 % 8)) % 31];
            sss_z1_m1[i] =  sss_z_tilda[(i + (m1 % 8)) % 31];
        }

        // Generate SSS
        for(i=0; i<31; i++)
        {
            sss_re_0[2*i]   =  sss_s0_m0[i]* sss_c0[i];
            sss_im_0[2*i]   = 0;
            sss_re_0[2*i+1] =  sss_s1_m1[i]* sss_c1[i]* sss_z1_m0[i];
            sss_im_0[2*i+1] = 0;

            sss_re_5[2*i]   =  sss_s1_m1[i]* sss_c0[i];
            sss_im_5[2*i]   = 0;
            sss_re_5[2*i+1] =  sss_s0_m0[i]* sss_c1[i]* sss_z1_m1[i];
            sss_im_5[2*i+1] = 0;
        }
    }

    void resource_mapper_impl::generate_crs(uint32  N_s,
                                            uint32  L,
                                            uint32  N_id_cell,
                                            uint32  N_sc_rb_dl,
                                            float  *crs_re,
                                            float  *crs_im)
    {
        float  one_over_sqrt_2 = 1/sqrt(2);
        uint32 N_cp;
        uint32 c_init;
        uint32 len = 2*LIBLTE_PHY_N_RB_DL_MAX;
        uint32 c[2*len];
        uint32 i;

        if(12 == N_sc_rb_dl)
        {
            N_cp = 1;
        }else{
            N_cp = 0;
        }

        // Calculate c_init
        c_init = 1024 * (7 * (N_s+1) + L + 1) * (2 * N_id_cell + 1) + 2*N_id_cell + N_cp;

        // Generate the psuedo random sequence c
        generate_prs_c(c_init, 2*len, c);

        // Construct the reference signals
        for(i=0; i<len; i++)
        {
            crs_re[i] = one_over_sqrt_2*(1 - 2*(float)c[2*i]);
            crs_im[i] = one_over_sqrt_2*(1 - 2*(float)c[2*i+1]);
        }
    }

    void resource_mapper_impl::generate_prs_c(uint32  c_init,
                                              uint32  len,
                                              uint32 *c)
    {
        uint32 i;
        uint32 x1;
        uint32 x2;
        uint8  new_bit1;
        uint8  new_bit2;

        // Initialize the 2nd m-sequence
        x2 = c_init;

        // Advance the 2nd m-sequence
        for(i=0; i<(1600-31); i++)
        {
            new_bit2 = ((x2 >> 3) ^ (x2 >> 2) ^ (x2 >> 1) ^ x2) & 0x1;

            x2 = (x2 >> 1) | (new_bit2 << 30);
        }

        // Initialize the 1st m-sequence
        x1 = 0x54D21B24; // This is the result of advancing the initial value of 0x00000001

        // Generate c
        for(i=0; i<len; i++)
        {
            new_bit1 = ((x1 >> 3) ^ x1) & 0x1;
            new_bit2 = ((x2 >> 3) ^ (x2 >> 2) ^ (x2 >> 1) ^ x2) & 0x1;

            x1 = (x1 >> 1) | (new_bit1 << 30);
            x2 = (x2 >> 1) | (new_bit2 << 30);

            c[i] = new_bit1 ^ new_bit2;
        }
    }

    resource_mapper_impl::~resource_mapper_impl()
    {
        delete d_fft;
    }

    void
    resource_mapper_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
//        for (int i=0; i<2; i++)
//        {
            ninput_items_required[0] = 960;     // for PBCH
            ninput_items_required[1] = 50000;   // for PDSCH
            ninput_items_required[2] = 3000;     // for PDCCH
            ninput_items_required[3] = 1000;   // for PHICH
            ninput_items_required[4] = 16;     // for PCFICH
//        }
    }

    int
    resource_mapper_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *pbch = (gr_complex *) input_items[0];
        gr_complex *pdsch = (gr_complex *) input_items[1];
        gr_complex *pdcch = (gr_complex *) input_items[2];
        gr_complex *phich = (gr_complex *) input_items[3];
        gr_complex *pcfich = (gr_complex *) input_items[4];

        gr_complex *out = (gr_complex *) output_items[0];

        // generating reference signals
        gr_complex crs[d_N_dl_prb*12/6];
        if(L%6 == 0 || L%6 == 4)
        {
            float *crs_re_f = new float [d_N_dl_prb*12/6];
            float *crs_im_f = new float [d_N_dl_prb*12/6];
            generate_crs(n_time_slot,
                         L,
                         d_N_id_cell,
                         12,
                         crs_re_f,
                         crs_im_f);

            // changing to gr_complex
            symb_to_gr_complex(crs_re_f,
                               crs_im_f,
                               d_N_dl_prb*12/6,
                               crs);
        }

        // reading tags
        vector<tag_t> all_tag_pbch;
        vector<tag_t> all_tag_pdsch;
        vector<tag_t> all_tag_pdcch;
        vector<tag_t> all_tag_phich;
        vector<tag_t> all_tag_pcfich;

        get_tags_in_window(all_tag_pbch,
                           0,
                           0,
                           ninput_items[0],
                           pmt::string_to_symbol("length"));

        get_tags_in_window(all_tag_pdsch,
                           1,
                           0,
                           ninput_items[1],
                           pmt::string_to_symbol("length"));

        get_tags_in_window(all_tag_pdcch,
                           2,
                           0,
                           ninput_items[2],
                           pmt::string_to_symbol("length"));

        get_tags_in_window(all_tag_phich,
                           3,
                           0,
                           ninput_items[3],
                           pmt::string_to_symbol("length"));

        get_tags_in_window(all_tag_pcfich,
                           4,
                           0,
                           ninput_items[4],
                           pmt::string_to_symbol("length"));


        if (all_tag_pbch.size()>0 & nf%4 == 0 & nf_past != nf)  // I will read PBCH at the 4-multiple of frames
        {
            pbch_len = pmt::to_long(all_tag_pbch[0].value);
        }

        if (all_tag_pdsch.size()>0 & pdsch_len_remained == 0)
        {
            pdsch_len = pmt::to_long(all_tag_pdsch[0].value);
            pdsch_len_remained = pdsch_len;
        }

        gr_complex ofdm_symb[d_N_dl_prb*12];
        uint32 n_pdsch_mapped = 0, n_pbch_mapped = 0;

        if(pdsch_len>0)     // always data must be present
        {
            if (n_time_slot == 0)
            {
                resource_mapper(PSS,
                                SSS_0,  // for time slot 0
                                crs,
                                pbch,
                                pdsch,
                                pdsch_len_remained,
                                d_N_dl_prb,
                                n_time_slot,
                                L%7,
                                nf,
                                d_N_id_cell,
                                ofdm_symb,
                                &n_pdsch_mapped,
                                &n_pbch_mapped);
                pdsch_len_remained -= n_pdsch_mapped;
            }
            else
            {
                resource_mapper(PSS,
                                SSS_5,  // for time slot 10
                                crs,
                                pbch,
                                pdsch,
                                pdsch_len_remained,
                                d_N_dl_prb,
                                n_time_slot,
                                L%7,
                                nf,
                                d_N_id_cell,
                                ofdm_symb,
                                &n_pdsch_mapped,
                                &n_pbch_mapped);
                pdsch_len_remained -= n_pdsch_mapped;
            }

            /////////////////////////// mapping control channels ////////////////////////////////
            // we just mapping control channels at the start of each subframe to the 3 OFDM symbols
            uint32 n_pdcch_mapped = 0, n_phich_mapped = 0, n_pcfich_mapped = 0;
            if ( L%7 == 0 & (n_time_slot%2 == 0))
            {
                if(all_tag_pdcch.size()>0 &
                        all_tag_phich.size()>0 &
                        all_tag_pcfich.size()>0)
                {
                    uint32 n_pdcch = pmt::to_long(all_tag_pdcch[0].value);
//                    uint32 n_pcfich = pmt::to_long(all_tag_pdcfich[0].value);
                    uint32 n_phich = pmt::to_long(all_tag_phich[0].value);
                    OFDM_sym1 = new gr_complex [fft_size];
                    OFDM_sym2 = new gr_complex [fft_size];


                    control_mapper(d_N_dl_prb,
                                   d_N_id_cell,
                                   pdcch,
                                   n_pdcch,
                                   phich,
                                   n_phich,
                                   pcfich,
                                   ofdm_symb,   // RS are mapped previousely by resource mapper
                                   OFDM_sym1,
                                   OFDM_sym2,
                                   &n_pdcch_mapped,
                                   &n_phich_mapped,
                                   &n_pcfich_mapped);
                }
                else
                {
                    // waiting for control channels to come!!
                    consume_each(0);
                    return 0;
                }
            }
            gr_complex fft_ofdm_symb[output_vec_size(d_N_dl_prb)];

            if (L%7 == 1 & (n_time_slot%2 == 0))
            {
                padding_zero(OFDM_sym1, fft_size, d_N_dl_prb, fft_ofdm_symb);
            }
            else if (L%7 == 2 & (n_time_slot%2 == 0))
            {
                padding_zero(OFDM_sym2, fft_size, d_N_dl_prb, fft_ofdm_symb);
            }
            else
            {
                padding_zero(ofdm_symb, fft_size, d_N_dl_prb, fft_ofdm_symb);
                cout << "OFDM SYMB 0 ===================================================" << endl;
                for (int i=0; i<d_N_dl_prb * 12; i++)
                    cout << ofdm_symb[i] << endl;
            }

//            // perfoming fft shift
            fft_shift(fft_ofdm_symb, fft_size, d_fft, out);

            // adding L as a tag (using in cp adder for checking start of a time slot)
            add_item_tag(0,
                         nitems_written(0),
                         pmt::string_to_symbol("L"),
                         pmt::from_long(L%7));

            //            cout << "\t\toutput of resource mapper: " << out[0] << endl;
            // printing status
            //            cout << "\t\tL: " << L << "\tn_time_slot: " << n_time_slot << "\tnf: " << nf << endl;
            // handling the frame
//            if (L <=6)
//            {
//                cout << "\t\t\t\t\t\toutput of L= " << L << endl ;
//                int count = fwrite(out, sizeof(gr_complex), fft_size, my_file1);
//                int c = fwrite(shifted_ofdm_symb, sizeof(gr_complex), fft_size, my_file2);
//            }
//            else
//                return -1;
            L++;
            n_time_slot = int(floor((L-L%7)/7))%20;  // normal cyclic prefix

            nf_past = nf;
            nf = floor((L - L%(20*7))/(20*7));

//            if (nf != nf_past)
//                cout << "========================================" << nf << endl;


            // posting messages ...
                // pbch
            if (nf%4 == 0 & nf != nf_past)  // when the next frame is started
            {
                message_port_pub(pmt::string_to_symbol("pbch"), pmt::from_bool(true));
                cout << "\t\t sending message to bch source\n";
            }
                // pdsch
            if (pdsch_len_remained < 2 * d_N_dl_prb * 12)   // as a role of thumb we suppose that the length of 2 ofdm symbols is at least available
            {
                message_port_pub(pmt::mp("pdsch"), pmt::from_bool(true));
                cout << "\t\t sending message to dsch source\n";
            }

            cout << "resource mapper " << endl;

            consume (0, n_pbch_mapped);
            consume(1, n_pdsch_mapped);
            consume (2, n_pdcch_mapped);
            consume(3, n_phich_mapped);
            consume (4, n_pcfich_mapped);

            return 1;
        }
        else
        {
            consume_each(0);
            return 0;
        }


        // Tell runtime system how many output items we produced.
    }

    void resource_mapper_impl::resource_mapper(gr_complex *pss,
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
                                               uint32 *n_pbch_mapped)
    {
        int k = 0;
        int idx = 0;
        int pbch_mapped = 0;
    //    int m = 0;

        int v_0 = 0, v_1 = 0, v_2 = 0, v_3 = 0;
        int v_shift = N_id_cell%6;

        // mapping pss
        if ((n_time_slot == 0 || n_time_slot == 10)& L == 6)
        {
            for (int n =0; n<62 ; n++)
            {
                k = n - 31 + 6 * N_dl_prb;
                ofdm_symb[k] = pss[n];
            }
        }

        // mapping sss
        if ((n_time_slot == 0 || n_time_slot == 10) & L == 5)
        {
            for(int n =0; n < 62; n++)
            {
                k = n - 31 + 6 * N_dl_prb;
                ofdm_symb[k] = sss[n];
            }
        }

        // mapping PBCH
        if (n_time_slot == 1 & (L == 0 | L == 1 | L == 2 | L == 3))
        {
            for (int n=0; n < 72; n++)
            {
                k = N_dl_prb * 6 - 36 + n;

                if (L==0)
                {
                    idx = 240*(nf%4)+0;
                    v_0 = 0;
                    v_1 = 3;

                    // skiping reference signals
                    if (!((k%6 == (v_0 + v_shift)%6)||
                          (k%6 == (v_1 + v_shift)%6) ))
                    {
                        ofdm_symb[k] = PBCH[idx+pbch_mapped];
                        pbch_mapped++;
                    }

                }
                else if (L==1)
                {
                    idx = 240*(nf%4)+48;
                    v_2 = 3 *(n_time_slot%2);
                    v_3 = 3 + 3 *(n_time_slot%2);

                    // skiping reference signals
                    if (!((k%6 == (v_2 + v_shift)%6)||
                          (k%6 == (v_3 + v_shift)%6) ) )
                    {
                        ofdm_symb[k] = PBCH[idx+pbch_mapped];
                        pbch_mapped++;
                    }

                }
                else if (L==2)
                {
                    idx = 240*(nf%4)+96;
                    ofdm_symb[k] = PBCH[idx+n];
                    pbch_mapped++;
                }
                else
                {
                    idx = 240*(nf%4)+168;
                    ofdm_symb[k] = PBCH[idx+pbch_mapped];
                    pbch_mapped++;
                }

            }

        }
        *n_pbch_mapped = pbch_mapped;

        k = 0;
        idx = 0;
        // mapping reference signals (corrected)
        if (L==0)
        {
            v_0 = 0;
            k = (v_0 + v_shift)%6;
            while ( k <N_dl_prb*12)
            {
                ofdm_symb[k] = crs[idx];
                idx++;
                k = 6 * idx + (v_0 + v_shift)%6;
            }
        }
        else if (L == 4)
        {
            v_0 = 3;
            k = (v_0 + v_shift)%6;
            while ( k <N_dl_prb*12)
            {
                ofdm_symb[k] = crs[idx];
                idx++;
                k = 6 * idx + (v_0 + v_shift)%6;
            }
        }

        k = 0;
        idx = 0;
        // mapping PDSCH
        if ((n_time_slot%2 == 0 & L>2) || (n_time_slot%2 == 1)) // skipping control channels
        {
            while(k < 12*N_dl_prb & k < n_pdsch_symb)
            {
                // skipping reference signals
                if (L == 0)
                {
                    v_0 = 0;
                    if (k%6 != (v_0 + v_shift)%6)   // skipping reference signals
                    {
                        if (n_time_slot == 1)       // taking care about PBCH
                        {
                            if(k < (N_dl_prb * 6 - 36) || k > (N_dl_prb * 6 - 36 + 71))
                            {
                                ofdm_symb[k] = PDSCH[idx];
                                idx++;
                            }
                        }
                        else
                        {
                            ofdm_symb[k] = PDSCH[idx];
                            idx++;
                        }
                    }
                }
                else if (L == 4)    // skipping reference signals
                {
                    v_0 = 3;
                    if (k%6 != (v_0 + v_shift)%6)   // skipping reference signals
                    {
                        ofdm_symb[k] = PDSCH[idx];
                        idx++;
                    }

                }
                else if (L == 1)
                {
                    if (n_time_slot == 1)    // skipping PBCH
                    {
                        if (k < (N_dl_prb * 6 - 36) || k > (N_dl_prb * 6 - 36 + 71))
                        {
                            ofdm_symb[k] = PDSCH[idx];
                            idx++;
                        }
                    }
                    else
                    {
                        ofdm_symb[k] = PDSCH[idx];
                        idx++;
                    }
                }
                else if (L == 2)
                {
                    if (n_time_slot == 1)    // skipping PBCH
                    {
                        if (k < (N_dl_prb * 6 - 36) || k > (N_dl_prb * 6 - 36 + 71))
                        {
                            ofdm_symb[k] = PDSCH[idx];
                            idx++;
                        }
                    }
                    else
                    {
                        ofdm_symb[k] = PDSCH[idx];
                        idx++;
                    }
                }
                else if (L == 3)
                {
                    if (n_time_slot == 1)    // skipping PBCH
                    {
                        if (k < (N_dl_prb * 6 - 36) || k > (N_dl_prb * 6 - 36 + 71))
                        {
                            ofdm_symb[k] = PDSCH[idx];
                            idx++;
                        }
                    }
                    else
                    {
                        ofdm_symb[k] = PDSCH[idx];
                        idx++;
                    }
                }
                else    // for L = 5 or 6
                {
                    // skipping pss & sss
                    if (n_time_slot == 0 || n_time_slot == 10)
                    {
                        if(k< (-31+N_dl_prb*6) ||
                                k > (61-31+N_dl_prb*6))
                        {
                            ofdm_symb[k] = PDSCH[idx];
                            idx++;
                        }
                    }
                    else
                    {
                        ofdm_symb[k] = PDSCH[idx];
                        idx++;
                    }

                }

                k++;
            }
            *n_pdsch_mapped = idx;
        }
        else    // adding zeros instead of control channels ...
        {
            *n_pdsch_mapped = 0;
            while (k<N_dl_prb)
            {
                if(L == 0)  // skipping RSs
                {
                    v_0 = 0;
                    v_1 = 3;
                    if ((k%6 != (v_0 + v_shift)%6) & (k%6 != (v_1 + v_shift)%6 ) )
                    {
                        ofdm_symb[k] = 0;
                    }

                }
                else
                {
                    ofdm_symb[k] = 0;
                }
                k++;
            }
        }
    }
    void resource_mapper_impl::padding_zero(gr_complex *in, uint32 d_fft_size, uint32 N_dl_rb, gr_complex *out)
    {
        for (int i=0; i< 12*N_dl_rb; i++)
        {
            out[(d_fft_size-12*N_dl_rb)/2 + i] = in[i];
        }

        // padding half of the zeros to the begining and to the end of ofdm symbol
        for (int i = 0; i< (d_fft_size-12*N_dl_rb)/2; i++)
        {
            out[i] = 0;
            out[d_fft_size-i];
        }

    }

    void resource_mapper_impl::fft_shift(gr_complex *in,
                                         uint32 d_fft_size,
                                         gr::fft::fft_complex *fft ,
                                         gr_complex *out)
    {
        // perfoming fft shift
        gr_complex shifted_ofdm_symb[d_fft_size];
        memcpy(shifted_ofdm_symb,                   &in[d_fft_size/2]   , d_fft_size*sizeof(gr_complex)/2);
        memcpy(&shifted_ofdm_symb[d_fft_size/2],    in                  , fft_size*sizeof(gr_complex)/2);
        // fft
        memcpy(fft->get_inbuf(), shifted_ofdm_symb, d_fft_size * sizeof(gr_complex));
        fft->execute();                           // exzecuting fft
        memcpy(out, fft->get_outbuf(), d_fft_size*sizeof(gr_complex));

    }

    void resource_mapper_impl::control_mapper(uint32 N_dl_prb,
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
                                              uint32 *n_pcfich_mapped)
    {

        int v_0 = 0;
        int v_1 = 3;
        int v_shift = N_id_cell%6;
        int k_bar = 6 * (N_id_cell%(2*N_dl_prb));
        int k = 0, l = 0;
        int idx = 0;
        int n = 0;
        bool unused_reg_0[N_dl_prb*2];

        for (int i=0; i<N_dl_prb * 2; i++)
            unused_reg_0[i] = true;

        // mapping PCFICH ( PCFICH is 16 symbol duration)
        for (int o=0; o<4; o++)
        {
            k = k_bar + floor(N_dl_prb * o/2) * 6;
            k = k%(12 * N_dl_prb);
            unused_reg_0[k/6] = false;
            idx = 0;
            while(idx < 4)
            {
                //skipping RS of both for p = 0,1
                if ((k%6 != (v_0 + v_shift)%6) & (k%6 != (v_1 + v_shift)%6))
                {
                    OFDM_sym0[k] = pcfich[n++];
                    idx++;
                }
                k++;
            }
        }
        *n_pcfich_mapped = n;

        // mapping PHICH (number of mapping units ... )
        int m_prim_max = n_phich/12, M_prim = 0;
        int n1 = N_dl_prb *3, n0 = N_dl_prb * 2 - 4;    // number of REGs in symbol 0 and 1 not assigned to PCFICH

        n = 0;
        bool unused_reg[N_dl_prb*2];
        for (int i=0; i<N_dl_prb * 2; i++)
            unused_reg[i] = true;

        while (M_prim < m_prim_max)
        {
            for (int i = 0; i<3; i++)
            {
                int Ni = int(floor(N_id_cell*n0/n1)+M_prim + i*floor(n0/3))%n0;

                int count = 0;
                for(int j=0; j<N_dl_prb*2; j++)
                {
                    // skipping PCFICH
                    if (unused_reg_0[j])
                    {
                        if (count == Ni)
                        {
                            k = j * 6;
                            unused_reg[j] = false;
                            break;
                        }
                        count ++;
                    }

                }


                idx = 0;
                while(idx<4)
                {
                    //skipping RS of both for p = 0,1
                    if ((k%6 != (v_0 + v_shift)%6) & (k%6 != (v_1 + v_shift)%6))
                    {
                        OFDM_sym0[k] = phich[n++];
                        idx++;
                    }
                    k++;
                }

            }

            M_prim++;
        }
        *n_phich_mapped = n;

        for (int i=0; i<N_dl_prb * 2; i++)
        {
            if(!unused_reg_0[i])
            {
                unused_reg[i] = false;
            }
        }

        n = 0;
        int K0 = 0, K1 = 0, K2 = 0;
        while( (K0 < N_dl_prb*12 || K1 < N_dl_prb*12 || K2 < N_dl_prb*12) & n<n_pdcch)
        {
            for (int l = 0; l<3; l++)
            {
                // we know that PHICH and PCFICH are mapped only to the first OFDM symbol
                if(l == 0 & K0 < N_dl_prb*12)
                {
                    idx = 0;
                    // skipping PHICH and PCFICH
                    if (unused_reg[K0/6])
                    {
                        while (idx < 4)
                        {
                            //skipping RS of both for p = 0,1
                            if ((K0%6 != (v_0 + v_shift)%6) & (K0%6 != (v_1 + v_shift)%6))
                            {
                                OFDM_sym0[K0] = pdcch[n++];
                                idx++;
                            }
                            K0++;
                        }
                    }
                    else
                    {
                        K0 += 6;
                    }

                }
                else if (l == 1 & K1 < N_dl_prb *12)
                {
                    idx = 0;
                    while (idx < 4)
                    {
                        OFDM_sym1[K1] = pdcch[n++];
                        idx++;
                        K1++;
                    }
                }
                else
                {
                    idx = 0;
                    while (idx < 4 & K2 < N_dl_prb *12)
                    {
                        OFDM_sym2[K2] = pdcch[n++];
                        idx++;
                        K2++;
                    }

                }
            }

        }
        *n_pdcch_mapped =  n;

    }


  } /* namespace my_lte */
} /* namespace gr */

