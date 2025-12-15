# Analysis_and_plotting_1gX-blips
macros for signal/sideband event selection and plotting for inclusive single shower events.


## Description and usage

Parsing macros: 
*  `anamacro_1gX_blips_signal.cpp`
*  `anamacro_1gX_blips_sideband.cpp` 


To compile the files above, do: 

* `g++ anamacro_1gX_blips_signal.cpp -o anamacro_1gX_blips_signal $(root-config --cflags --libs)`

* `g++ anamacro_1gX_blips_sideband.cpp -o anamacro_1gX_blips_sideband $(root-config --cflags --libs)`



To parse the input files run the analysis macros by sourcing:  `run_anamacro_1gX_blips_signal.sh` and `run_anamacro_1gX_blips_sideband.sh`.
 Output files will be located in the corresponding output directories and these files will be the input for the plotting macros. 


Plotting macros: 
*  `IncSP_Nprotons_General_v1.C` 
*  `IncSP_Nprotons_General_0n_v1.C`
*  `IncSP_Nprotons_General_Nn_v1.C`


To plot such outcomes, source the `run_IncSP_Nprotons_General.sh`, `run_IncSP_Nprotons_General_0n.sh` or `run_IncSP_Nprotons_General_Nn.sh` accordingly. Output plots will be saved in a specific output directory. 
