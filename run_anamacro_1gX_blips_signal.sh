#!/bin/bash

# ./anamacro_1gX_blips_signal <input_file> <Signal/Sideband> <IsData> <AddBacktrkBlips> <OutDir>


#run4b EXT unbiased 

<<<<<<< HEAD
./anamacro_1gX_blips_signal_EXTUnb /path/to/file/surprise/run4b_full_samples/wc_processed/BNB/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist.root Signal true false SIGNAL_EXTUnb
=======
./anamacro_1gX_blips_signal_EXTUnb /pnfs/uboone/persistent/users/uboonepro/surprise/run4b_full_samples/wc_processed/BNB/checkout_MCC9.10_Run4b_BNB_extunbiased_data_surprise_reco2_hist.root Signal true false SIGNAL_EXTUnb
>>>>>>> 40cccb8144cf9b941335a90dc770c558df1539aa


#BNB Nu overlay
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist.root Signal false false SIGNAL
./anamacro_1gX_blips_signal_enhanced /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist.root Signal false false SIGNAL_ENHANCED


#BNB-ON
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist.root Signal true false SIGNAL
./anamacro_1gX_blips_signal_enhanced /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist.root Signal true false SIGNAL_ENHANCED_BNBoN

#BNB-OFF
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist.root Signal false false SIGNAL


#BNB Nue overlay
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist.root Signal false false SIGNAL


#Pi0
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist.root Signal false false SIGNAL


#DIRT
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist.root Signal false false SIGNAL



#Adding proton blips candidates 

#BNB Nu overlay
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist.root SignalBTB false true SIGNAL_BTB



#BNB-ON
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist.root SignalBTB true true SIGNAL_BTB


#BNB-OFF
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist.root SignalBTB false true SIGNAL_BTB


#BNB Nue overlay
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist.root SignalBTB false true SIGNAL_BTB


#Pi0
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist.root SignalBTB false true SIGNAL_BTB


#DIRT
./anamacro_1gX_blips_signal /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist.root SignalBTB false true SIGNAL_BTB


