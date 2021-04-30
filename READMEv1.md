# Oreo DEVS model
###  FILES ORGANIZATION
- The Oreo DEVS model simulate the process of making Oreos, the whole process and Cadium model implementation routine is clarified below

# Readme.md

- Oreo.doc
- makefile

****Table of Contents****

# Atomic models
- This folder contains atomic models implemented in Cadmium
    - Queue.hpp
    - Process.hpp
    - mCheck.hpp
    - Classification.hpp
    - Formation.hpp
    - Bake.hpp
    - Brokencheckshop.hpp
    - Workshop.hpp
    - Package.hpp

## Bin
- This folder will be created automatically the first time you compile the poject.
- It will contain all the executables.

## Build
- This folder will be created automatically the first time you compile the poject.
- It will contain all the build files and generated during compilation.

## data_structures
- This folder contains message data structure used in the model.
    - message.hpp
    - message.cpp

## input_data
- This folder contains all the input data to run the model and the tests
    - queue_input_test.txt
    - process_input_test.txt
    - mcheck_input_test.txt
    - classify _input_test.txt
    - formation_input_test.txt
    - bake_input_test.txt
    - borkencheck_input_test.txt
    - workshop_biscuit_input_test.txt
    - workshop_cream_input_test.txt
    - package_input_test.txt
    - rawmaterial_input_test.txt
    - inspector_input_test.txt
    - maintop_input_test.txt
    - assembly_biscuit_input_test.txt
    - assembly_cream_input_test.txt
    - manufact_input_test.txt

## simulation_results
- This folder will be created automatically the first time you compile the poject.
-  It will store the outputs from your simulations and tests.

## test
- This folder the unit test of the atomic and coulped models
    - main_Queue.cpp
    - main_Process.cpp
    - main_mCheck.cpp
    - main_Classification.cpp
    - main_Formation.cpp
    - main_bake_test.cpp
    - main_brokencheck_test.cpp
    - main_workshop_test.cpp
    - main_package_test.cpp
    - main_RawMaterial.cpp
    - main_Inspector.cpp
    - main_assembly.cpp
    - main_Manufacturing.cpp
    

## top_model
- This folder contains the Oreo top model
    - main_top.cpp

# Steps

-  OreoFac.doc contains the explanation of this model
-  Update include path in the makefile in this folder and subfolders. You need to update the following lines:
    -  INCLUDECADMIUM=-I ../../cadmium/include
    -  INCLUDEDESTIMES=-I ../../DESTimes/include
-  Compile the project and the tests
    - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the Oreo folder
    - To compile the project and the tests, type in the terminal:
			make clean; make all
- Run subnet test
     - Open the terminal in the bin folder
	 - To run the test, type in the terminal
            ./OreoFactory.exe
     - For this specific test you need to type:
			./QUEUE_TEST (or ./SUBNET_TEST.exe for Windows)
     - To check the output of the test, go to the folder and simulation_results and open  "main_test_output_messages.txt" and "main_test_output_state.txt"
- To run others tests you just need to select the proper executable name in step again
- Run the top model
     - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
	 - To run the model, type in the terminal 
	       ./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". 
	 - For this test you need to type:
		   ./OreoFactory ../input_data/maintop_input_test.txt (for Windows: ./OreoFactory.exe ../input_data/maintop_input_test.txt)
	   
     - To check the output of the model, go to the folder simulation_results and open main_test_output_messasges.txt" and "main_test_output_state.txt"
	 - To run the model with different inputs
	   - Create new .txt files with the same structure such as bake_input_test.txt or package_input_test.txt in the folder input_data
	   -  Run the model using the instructions in step before
	   - If you want to keep the output, rename
	   - Eg: "bake_test_output_messages.txt" and "bake_test_output_state.txt".

## The End

