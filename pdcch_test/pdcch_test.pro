TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../lib/conv_enc_impl.cc \
    ../lib/crc_attachment_impl.cc \
    ../lib/layer_mapper_impl.cc \
    ../lib/modulation_mapper_impl.cc \
    ../lib/pdcch_interleaver_impl.cc \
    ../lib/pdcch_mux_impl.cc \
    ../lib/precoder_impl.cc \
    ../lib/rate_match_conv_impl.cc \
    ../lib/scrambler_impl.cc \
    ../lib/dci_gen_impl.cc

LIBS += \
/usr/lib/x86_64-linux-gnu/libboost_signals.so\
/usr/lib/x86_64-linux-gnu/libboost_signals.so.1.54.0\
#/usr/lib/x86_64-linux-gnu/libboost_system-mt.so\
/usr/lib/x86_64-linux-gnu/libboost_system.so\
/usr/lib/x86_64-linux-gnu/libboost_system.so.1.54.0\
#/usr/lib/x86_64-linux-gnu/libboost_thread-mt.so\
/usr/lib/x86_64-linux-gnu/libboost_thread.so\
/usr/lib/x86_64-linux-gnu/libboost_thread.so.1.54.0\
#/usr/lib/x86_64-linux-gnu/libboost_timer-mt.so\
/usr/lib/x86_64-linux-gnu/libboost_timer.so\
/usr/local/lib/libvolk.so \
/usr/local/lib/libvolk.so.0.0.0 \
-L/usr/local/lib -lgnuradio-audio -lgnuradio-runtime -lgnuradio-pmt -lgnuradio-blocks -lgnuradio-filter

LIBS+= -L/usr/local/lib -lgnuradio-analog -lgnuradio-runtime -lgnuradio-pmt -lgnuradio-audio -lgnuradio-blocks -lgnuradio-filter
LIBS+= -lgnuradio-qtgui -lgnuradio-fft \
#-lboost_system -lboost_thread-mt -lboost_thread -lboost_filesystem-mt -lboost_filesystem

INCLUDEPATH += /home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/include
LIBS += -lfftw3f -lfftw3

HEADERS += \
    ../lib/conv_enc_impl.h \
    ../lib/crc_attachment_impl.h \
    ../lib/layer_mapper_impl.h \
    ../lib/modulation_mapper_impl.h \
    ../lib/pdcch_interleaver_impl.h \
    ../lib/pdcch_mux_impl.h \
    ../lib/precoder_impl.h \
    ../lib/rate_match_conv_impl.h \
    ../lib/scrambler_impl.h \
    ../lib/dci_gen_impl.h \
    ../include/my_lte/conv_enc.h \
    ../include/my_lte/crc_attachment.h \
    ../include/my_lte/dci_gen.h \
    ../include/my_lte/layer_mapper.h \
    ../include/my_lte/modulation_mapper.h \
    ../include/my_lte/pdcch_interleaver.h \
    ../include/my_lte/pdcch_mux.h \
    ../include/my_lte/precoder.h \
    ../include/my_lte/rate_match_conv.h \
    ../include/my_lte/scrambler.h
