#!/bin/bash

# script to run IncSP_Nprotons_General_0n_v1.C with the indicated input arguments

#SIGNAL
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIGNAL_0n_TEST", 
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_Signal_Data.root",
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_Signal.root",
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root",
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_Signal.root",
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_Signal.root",
                                  "SIGNAL/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_Signal.root",
                                 false)'

#SIGNAL_BTB
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIGNAL_BTB_0n_TEST",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_SignalBTB_Data_BacktrkBlips.root",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                 false)'




#SIGNAL_CRT
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIGNAL_CRT_0n_TEST",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_Signal_Data.root",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_Signal.root",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Signal.root",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_Signal.root",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_Signal.root",
                                  "SIGNAL_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_Signal.root",
                                 false)'





#SIGNAL_CRT_BTB
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIGNAL_CRT_BTB_0n_TEST",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_SignalBTB_Data_BacktrkBlips.root",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                  "SIGNAL_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_SignalBTB_BacktrkBlips.root",
                                 false)'

#========
#SIDEBAND
#========

#SIDEBAND
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIDEBAND_0n_TEST",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_Sideband_Data.root",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_Sideband.root",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_Sideband.root",
                                 false)'


#SIDEBAND_BTB
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIDEBAND_BTB_0n_TEST",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_SidebandBTB_Data_BacktrkBlips.root",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                 false)'


#SIDEBAND_CRT
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIDEBAND_CRT_0n_TEST",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_Sideband_Data.root",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_Sideband.root",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_Sideband.root",
                                  "SIDEBAND_CRT/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_Sideband.root",
                                 false)'


#SIDEBAND_CRT_BTB
root -l -b -q 'IncSP_Nprotons_General_0n_v1.C("SIDEBAND_CRT_BTB_0n_TEST",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_11_BNB_beam_on_surprise_reco2_hist_SidebandBTB_Data_BacktrkBlips.root",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_Run4b_BNB_beam_off_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nu_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_dirt_surpise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_NC_pi0_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                  "SIDEBAND_CRT_BTB/MCC9.10_Run4b_v10_04_07_09_BNB_nue_overlay_surprise_reco2_hist_SidebandBTB_BacktrkBlips.root",
                                 false)' 
