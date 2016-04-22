#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Thu Apr 21 20:20:51 2016
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from PCFICH import PCFICH  # grc-generated hier_block
from PDSCH_5M import PDSCH_5M  # grc-generated hier_block
from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
from pbch import pbch  # grc-generated hier_block
import my_lte
import sip


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self, MCS=9, N_CCE=13, N_dl_prb=25, RB_start_UE_1=0, RB_start_UE_2=70, aggregation1=288, aggregation2=288*2, allocation_len_UE_1=60, allocation_len_UE_2=10, cfi_val=2, n_pdcch=2):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Parameters
        ##################################################
        self.MCS = MCS
        self.N_CCE = N_CCE
        self.N_dl_prb = N_dl_prb
        self.RB_start_UE_1 = RB_start_UE_1
        self.RB_start_UE_2 = RB_start_UE_2
        self.aggregation1 = aggregation1
        self.aggregation2 = aggregation2
        self.allocation_len_UE_1 = allocation_len_UE_1
        self.allocation_len_UE_2 = allocation_len_UE_2
        self.cfi_val = cfi_val
        self.n_pdcch = n_pdcch

        ##################################################
        # Variables
        ##################################################
        self.N_id_2 = N_id_2 = 2
        self.N_id_1 = N_id_1 = 100
        self.tx_mode = tx_mode = 1
        self.samp_rate = samp_rate = 30720000/40
        self.rv_idx = rv_idx = 0
        self.rnti = rnti = 1
        self.normal_cp = normal_cp = True
        self.cell_id = cell_id = N_id_1 * 3 + N_id_2
        self.TBS = TBS = 2112
        self.N_soft = N_soft = 3654144
        self.N_e_bits = N_e_bits = 6300
        self.N_code_block = N_code_block = 1
        self.N_ant = N_ant = 1
        self.M_dl_harq = M_dl_harq = 6

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
        	1024, #size
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0.set_update_time(0.10)
        self.qtgui_const_sink_x_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(True)
        self.qtgui_const_sink_x_0.enable_grid(True)
        
        if not True:
          self.qtgui_const_sink_x_0.disable_legend()
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_const_sink_x_0_win)
        self.pbch_0 = pbch(
            N_ant=N_ant,
            RNTI=rnti,
            cell_id=cell_id,
            normal_cp=normal_cp,
        )
        self.my_lte_resource_mapper_0 = my_lte.resource_mapper(N_id_1, N_id_2, N_dl_prb)
        self.my_lte_lte_random_bit_gen_0_0 = my_lte.lte_random_bit_gen(2112-24, 2000)
        self.my_lte_lte_random_bit_gen_0 = my_lte.lte_random_bit_gen(24, 20000)
        self.my_lte_cp_adder_0 = my_lte.cp_adder(N_dl_prb, True)
        self.my_lte_cfi_gen_0 = my_lte.cfi_gen(N_dl_prb, cfi_val)
        self.blocks_throttle_0_1_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_gr_complex, 1, N_CCE*9*8, "length")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_gr_complex, 1, 12*3, "length")
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/home/mostafa/Desktop/LTE_Signal/matlabfiles/ffo/grc_data_5M.dat", False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 1000, 1, 0)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 1000, 1, 0)
        self.PDSCH_5M_0 = PDSCH_5M(
            M_dl_harq=M_dl_harq,
            N_code_block=N_code_block,
            N_e_bits=N_e_bits,
            N_soft=N_soft,
            RNTI=rnti,
            TBS=TBS,
            rv_idx=rv_idx,
            tx_mode=tx_mode,
        )
        self.PCFICH_0 = PCFICH(
            RNTI=rnti,
            cell_id=cell_id,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.PCFICH_0, 0), (self.my_lte_resource_mapper_0, 4))    
        self.connect((self.PDSCH_5M_0, 0), (self.my_lte_resource_mapper_0, 1))    
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.my_lte_resource_mapper_0, 3))    
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.my_lte_resource_mapper_0, 2))    
        self.connect((self.blocks_throttle_0, 0), (self.pbch_0, 0))    
        self.connect((self.blocks_throttle_0_0, 0), (self.PDSCH_5M_0, 0))    
        self.connect((self.blocks_throttle_0_1_0, 0), (self.PCFICH_0, 0))    
        self.connect((self.my_lte_cfi_gen_0, 0), (self.blocks_throttle_0_1_0, 0))    
        self.connect((self.my_lte_cp_adder_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.my_lte_lte_random_bit_gen_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.my_lte_lte_random_bit_gen_0_0, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.my_lte_resource_mapper_0, 0), (self.my_lte_cp_adder_0, 0))    
        self.connect((self.pbch_0, 0), (self.my_lte_resource_mapper_0, 0))    
        self.connect((self.pbch_0, 0), (self.qtgui_const_sink_x_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_MCS(self):
        return self.MCS

    def set_MCS(self, MCS):
        self.MCS = MCS

    def get_N_CCE(self):
        return self.N_CCE

    def set_N_CCE(self, N_CCE):
        self.N_CCE = N_CCE
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(self.N_CCE*9*8)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(self.N_CCE*9*8)

    def get_N_dl_prb(self):
        return self.N_dl_prb

    def set_N_dl_prb(self, N_dl_prb):
        self.N_dl_prb = N_dl_prb

    def get_RB_start_UE_1(self):
        return self.RB_start_UE_1

    def set_RB_start_UE_1(self, RB_start_UE_1):
        self.RB_start_UE_1 = RB_start_UE_1

    def get_RB_start_UE_2(self):
        return self.RB_start_UE_2

    def set_RB_start_UE_2(self, RB_start_UE_2):
        self.RB_start_UE_2 = RB_start_UE_2

    def get_aggregation1(self):
        return self.aggregation1

    def set_aggregation1(self, aggregation1):
        self.aggregation1 = aggregation1

    def get_aggregation2(self):
        return self.aggregation2

    def set_aggregation2(self, aggregation2):
        self.aggregation2 = aggregation2

    def get_allocation_len_UE_1(self):
        return self.allocation_len_UE_1

    def set_allocation_len_UE_1(self, allocation_len_UE_1):
        self.allocation_len_UE_1 = allocation_len_UE_1

    def get_allocation_len_UE_2(self):
        return self.allocation_len_UE_2

    def set_allocation_len_UE_2(self, allocation_len_UE_2):
        self.allocation_len_UE_2 = allocation_len_UE_2

    def get_cfi_val(self):
        return self.cfi_val

    def set_cfi_val(self, cfi_val):
        self.cfi_val = cfi_val

    def get_n_pdcch(self):
        return self.n_pdcch

    def set_n_pdcch(self, n_pdcch):
        self.n_pdcch = n_pdcch

    def get_N_id_2(self):
        return self.N_id_2

    def set_N_id_2(self, N_id_2):
        self.N_id_2 = N_id_2
        self.set_cell_id(self.N_id_1 * 3 + self.N_id_2)

    def get_N_id_1(self):
        return self.N_id_1

    def set_N_id_1(self, N_id_1):
        self.N_id_1 = N_id_1
        self.set_cell_id(self.N_id_1 * 3 + self.N_id_2)

    def get_tx_mode(self):
        return self.tx_mode

    def set_tx_mode(self, tx_mode):
        self.tx_mode = tx_mode
        self.PDSCH_5M_0.set_tx_mode(self.tx_mode)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0_1_0.set_sample_rate(self.samp_rate)

    def get_rv_idx(self):
        return self.rv_idx

    def set_rv_idx(self, rv_idx):
        self.rv_idx = rv_idx
        self.PDSCH_5M_0.set_rv_idx(self.rv_idx)

    def get_rnti(self):
        return self.rnti

    def set_rnti(self, rnti):
        self.rnti = rnti
        self.PCFICH_0.set_RNTI(self.rnti)
        self.PDSCH_5M_0.set_RNTI(self.rnti)
        self.pbch_0.set_RNTI(self.rnti)

    def get_normal_cp(self):
        return self.normal_cp

    def set_normal_cp(self, normal_cp):
        self.normal_cp = normal_cp
        self.pbch_0.set_normal_cp(self.normal_cp)

    def get_cell_id(self):
        return self.cell_id

    def set_cell_id(self, cell_id):
        self.cell_id = cell_id
        self.PCFICH_0.set_cell_id(self.cell_id)
        self.pbch_0.set_cell_id(self.cell_id)

    def get_TBS(self):
        return self.TBS

    def set_TBS(self, TBS):
        self.TBS = TBS
        self.PDSCH_5M_0.set_TBS(self.TBS)

    def get_N_soft(self):
        return self.N_soft

    def set_N_soft(self, N_soft):
        self.N_soft = N_soft
        self.PDSCH_5M_0.set_N_soft(self.N_soft)

    def get_N_e_bits(self):
        return self.N_e_bits

    def set_N_e_bits(self, N_e_bits):
        self.N_e_bits = N_e_bits
        self.PDSCH_5M_0.set_N_e_bits(self.N_e_bits)

    def get_N_code_block(self):
        return self.N_code_block

    def set_N_code_block(self, N_code_block):
        self.N_code_block = N_code_block
        self.PDSCH_5M_0.set_N_code_block(self.N_code_block)

    def get_N_ant(self):
        return self.N_ant

    def set_N_ant(self, N_ant):
        self.N_ant = N_ant
        self.pbch_0.set_N_ant(self.N_ant)

    def get_M_dl_harq(self):
        return self.M_dl_harq

    def set_M_dl_harq(self, M_dl_harq):
        self.M_dl_harq = M_dl_harq
        self.PDSCH_5M_0.set_M_dl_harq(self.M_dl_harq)


def argument_parser():
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option(
        "", "--MCS", dest="MCS", type="intx", default=9,
        help="Set MCS [default=%default]")
    parser.add_option(
        "", "--N-CCE", dest="N_CCE", type="intx", default=13,
        help="Set N_CCE [default=%default]")
    parser.add_option(
        "", "--N-dl-prb", dest="N_dl_prb", type="intx", default=25,
        help="Set N_dl_prb [default=%default]")
    parser.add_option(
        "", "--RB-start-UE-1", dest="RB_start_UE_1", type="intx", default=0,
        help="Set RB_start_UE_1 [default=%default]")
    parser.add_option(
        "", "--RB-start-UE-2", dest="RB_start_UE_2", type="intx", default=70,
        help="Set RB_start_UE_2 [default=%default]")
    parser.add_option(
        "", "--aggregation1", dest="aggregation1", type="intx", default=288,
        help="Set aggregation1 [default=%default]")
    parser.add_option(
        "", "--aggregation2", dest="aggregation2", type="intx", default=288*2,
        help="Set aggregation2 [default=%default]")
    parser.add_option(
        "", "--allocation-len-UE-1", dest="allocation_len_UE_1", type="intx", default=60,
        help="Set allocation_len_UE_1 [default=%default]")
    parser.add_option(
        "", "--allocation-len-UE-2", dest="allocation_len_UE_2", type="intx", default=10,
        help="Set allocation_len_UE_2 [default=%default]")
    parser.add_option(
        "", "--cfi-val", dest="cfi_val", type="intx", default=2,
        help="Set cfi_val [default=%default]")
    parser.add_option(
        "", "--n-pdcch", dest="n_pdcch", type="intx", default=2,
        help="Set n_pdcch [default=%default]")
    return parser


def main(top_block_cls=top_block, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()
    if gr.enable_realtime_scheduling() != gr.RT_OK:
        print "Error: failed to enable real-time scheduling."

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(MCS=options.MCS, N_CCE=options.N_CCE, N_dl_prb=options.N_dl_prb, RB_start_UE_1=options.RB_start_UE_1, RB_start_UE_2=options.RB_start_UE_2, aggregation1=options.aggregation1, aggregation2=options.aggregation2, allocation_len_UE_1=options.allocation_len_UE_1, allocation_len_UE_2=options.allocation_len_UE_2, cfi_val=options.cfi_val, n_pdcch=options.n_pdcch)
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
