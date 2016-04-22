#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/lib
export PATH=/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/lib:$PATH
export LD_LIBRARY_PATH=/home/mostafa/Dropbox/LTE/OOT/gr-my_lte_000/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-my_lte 
