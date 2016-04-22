#include <iostream>
#include <gnuradio/top_block.h>
#include <my_lte/pdcch_mux.h>
#include <my_lte/dci_gen.h>
#include <my_lte/pdcch_interleaver.h>
#include <my_lte/crc_attachment.h>
#include <my_lte/rate_match_conv.h>
#include <my_lte/scrambler.h>
#include <my_lte/conv_enc.h>
#include <my_lte/modulation_mapper.h>
#include <my_lte/layer_mapper.h>
#include <my_lte/precoder.h>
#include <gnuradio/blocks/file_sink.h>

using namespace std;
using namespace gr;

// RNTIs 36.321 v10.2.0 Section 7.1
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

int main()
{
    uint32 N_id_1 = 100, N_id_2 = 0, N_dl_prb = 110;
    uint16 RNTI = LIBLTE_MAC_RA_RNTI_START;
    int N_group = 3;     //N_dl_prb/48 + 1;
    int cell_id = 3*N_id_1 + N_id_2;
    vector<uint32> g;
    g.push_back(0133);
    g.push_back(0171);
    g.push_back(0165);

    top_block_sptr tb = make_top_block("top block");
    //***********************************************************************************************//
    //*************************************** PDCCH **************************************************//
    string ch_n("DCI");
    int num_pdcch_bits = 288;   // aggregation level 4
    string dci_type("1A");
    bool carrier_indicator = false, new_data_ind = false;
    int mcs = 9;        //3GPP TS 36.213 version 10.1.0 Release 10 table 7.1.7.1-1
    int alloc_len = 20, RB_start = 0;
    int N_REG = N_dl_prb * (2+3+3) - N_group * 8 * 12/4 - 4;
    int N_CCE = N_REG/9;
    uint32 tx_mode = 1;
    uint32 M_dl_harq = 6;   // can be 4, 7, 10, 9, 12, 15, 6 for TDD mode!!!
    uint32 rv_idx = 0;
    string mod_type("QPSK");
    int n_ant = 1;

    // source 1
    my_lte::dci_gen::sptr dci_gen = my_lte::dci_gen::make(dci_type,
                                                          RNTI,
                                                          carrier_indicator,
                                                          rv_idx,
                                                          N_dl_prb,
                                                          n_ant,
                                                          tx_mode,
                                                          M_dl_harq,
                                                          mcs,
                                                          new_data_ind,
                                                          n_ant,
                                                          alloc_len,
                                                          RB_start);

    my_lte::crc_attachment::sptr crc_dci = my_lte::crc_attachment::make(CRC16, 1, ch_n, 100, RNTI);

    my_lte::conv_enc::sptr conv_enc_dci = my_lte::conv_enc::make(7, 3, g, true);

    my_lte::rate_match_conv::sptr rm_dci = my_lte::rate_match_conv::make(num_pdcch_bits);

    // source 2
    my_lte::dci_gen::sptr dci_gen2 = my_lte::dci_gen::make(dci_type,
                                                          RNTI+10,
                                                          carrier_indicator,
                                                          rv_idx,
                                                          N_dl_prb,
                                                          n_ant,
                                                          tx_mode,
                                                          M_dl_harq,
                                                          mcs,
                                                          new_data_ind,
                                                          n_ant,
                                                          alloc_len,
                                                          RB_start);

    my_lte::crc_attachment::sptr crc_dci2 = my_lte::crc_attachment::make(CRC16, 1, ch_n, 100, RNTI+10);

    my_lte::conv_enc::sptr conv_enc_dci2 = my_lte::conv_enc::make(7, 3, g, true);

    my_lte::rate_match_conv::sptr rm_dci2 = my_lte::rate_match_conv::make(num_pdcch_bits*2);


    /////////////////// pdcch ...

    my_lte::pdcch_mux::sptr pdcc_multiplex = my_lte::pdcch_mux::make(2, N_CCE);

//    blocks::file_sink::sptr pdcch_file_snk = blocks::file_sink::make(sizeof(gr_complex),
//                                                                     "pdcch.txt",
//                                                                     false);

    my_lte::scrambler::sptr scrmblr_pdcch = my_lte::scrambler::make(cell_id,
                                                                    ch_n,
                                                                    RNTI,
                                                                    10000);

    my_lte::modulation_mapper::sptr mod_pdcch = my_lte::modulation_mapper::make(mod_type, 10000);

    my_lte::layer_mapper::sptr layer_map_pdcch = my_lte::layer_mapper::make(n_ant, 10000);

    my_lte::precoder::sptr precoder_pdcch = my_lte::precoder::make(n_ant, 10000, false, 0, true);

    my_lte::pdcch_interleaver::sptr pdcch_int = my_lte::pdcch_interleaver::make(n_ant, cell_id);

    blocks::file_sink::sptr pdcch_file_snk = blocks::file_sink::make(sizeof(gr_complex),
                                                                     "pdcch.txt",
                                                                     false);

    /////////////////////////////// PDCCH connections /////////////////////////////////////////////
    tb->connect(dci_gen, 0, crc_dci, 0);
    tb->connect(crc_dci, 0, conv_enc_dci, 0);
    tb->connect(conv_enc_dci, 0, rm_dci, 0);
    tb->connect(rm_dci, 0, pdcc_multiplex, 0);

    tb->connect(dci_gen2, 0, crc_dci2, 0);
    tb->connect(crc_dci2, 0, conv_enc_dci2, 0);
    tb->connect(conv_enc_dci2, 0, rm_dci2, 0);
    tb->connect(rm_dci2, 0, pdcc_multiplex, 1);

    tb->connect(pdcc_multiplex, 0, scrmblr_pdcch, 0);
    tb->connect(scrmblr_pdcch,0, mod_pdcch ,0);
    tb->connect(mod_pdcch, 0, layer_map_pdcch, 0);
    tb->connect(layer_map_pdcch, 0, precoder_pdcch, 0);
    tb->connect(precoder_pdcch, 0, pdcch_int,0 );
    tb->connect(pdcch_int,0 , pdcch_file_snk, 0);

    tb->run();

    cout << "THE END!" << endl;
    return 0;
}

