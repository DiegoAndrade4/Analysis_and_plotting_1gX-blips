#!/bin/bash

# ./anamacro_1gX_blips_sideband <input_file> <Signal/Sideband> <IsData> <AddBacktrkBlips> <OutDir>

#BNB Nu overlay
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist.root Sideband false false SIDEBAND



#BNB-ON
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist.root Sideband true false SIDEBAND


#BNB-OFF
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist.root Sideband false false SIDEBAND


#BNB Nue overlay
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist.root Sideband false false SIDEBAND


#Pi0
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist.root Sideband false false SIDEBAND


#DIRT
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist.root Sideband false false SIDEBAND



#Adding proton blips candidates 

#BNB Nu overlay
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist.root SidebandBTB false true SIDEBAND_BTB



#BNB-ON
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist.root SidebandBTB true true SIDEBAND_BTB


#BNB-OFF
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist.root SidebandBTB false true SIDEBAND_BTB


#BNB Nue overlay
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist.root SidebandBTB false true SIDEBAND_BTB


#Pi0
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist.root SidebandBTB false true SIDEBAND_BTB


#DIRT
./anamacro_1gX_blips_sideband /path/to/file/BNB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist.root SidebandBTB false true SIDEBAND_BTB


