# Analysis_and_plotting_1gX-blips
Macros for signal/sideband parse/event selection and plotting for inclusive single shower events.


## Description and usage

#### Event selection macros: 
*  `anamacro_1gX_blips_signal.cpp`
*  `anamacro_1gX_blips_sideband.cpp` 


To compile the files above, do: 

* `g++ anamacro_1gX_blips_signal.cpp -o anamacro_1gX_blips_signal $(root-config --cflags --libs)`

* `g++ anamacro_1gX_blips_sideband.cpp -o anamacro_1gX_blips_sideband $(root-config --cflags --libs)`


To execute, provide the following argument for a single processing: 
* `./anamacro_1gX_blips_signal <input_file> <Signal/Sideband> <IsData> <AddBacktrkBlips> <OutDir>`



#### Plotting macros: 
*  `IncSP_Nprotons_General_v1.C` 
*  `IncSP_Nprotons_General_0n_v1.C`
*  `IncSP_Nprotons_General_Nn_v1.C`


#### Bash scripts
* `run_anamacro_1gX_blips_signal.sh`
* `run_anamacro_1gX_blips_sideband.sh`
* `run_IncSP_Nprotons_General.sh`
* `run_IncSP_Nprotons_General_Nn.sh`
* `run_IncSP_Nprotons_General_0n.sh`

## Usage

To parse and get event selection for multiple variations, run the executable by sourcing:  `run_anamacro_1gX_blips_signal.sh` and `run_anamacro_1gX_blips_sideband.sh`.
Output files will be located in the corresponding output directories. These files will be the input for the plotting macros. 

 To plot such outcomes, source the `run_IncSP_Nprotons_General.sh`, `run_IncSP_Nprotons_General_0n.sh` or `run_IncSP_Nprotons_General_Nn.sh` accordingly. Inputs should match the outputs of the previous step. Output plots will be saved in a specific output directory self descriptive.  

