CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEJSON=-I ../CadmiumModelJSONExporter/include
INCLUDEDESTIMES=-I ../DESTimes/include


#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL TESTS
message.o: data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/message.cpp	-o build/message.o

main_top.o: test/main_top.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) test/main_top.cpp -o build/main_top.o

main_Queue.o: test/main_Queue.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Queue.cpp -o build/main_Queue.o

main_Process.o: test/main_Process.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Process.cpp -o build/main_Process.o

main_mCheck.o: test/main_mCheck.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_mCheck.cpp -o build/main_mCheck.o

main_Classification.o: test/main_Classification.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Classification.cpp -o build/main_Classification.o

main_Formation.o: test/main_Formation.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Formation.cpp -o build/main_Formation.o

main_bake_test.o: test/main_bake_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_bake_test.cpp -o build/main_bake_test.o

main_brokencheck_test.o: test/main_brokencheck_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_brokencheck_test.cpp -o build/main_brokencheck_test.o

main_workshop_test.o: test/main_workshop_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_workshop_test.cpp -o build/main_workshop_test.o

main_package_test.o: test/main_package_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_package_test.cpp -o build/main_package_test.o

main_assembly.o: test/main_assembly.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_assembly.cpp -o build/main_assembly.o

main_Inspector.o: test/main_Inspector.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Inspector.cpp -o build/main_Inspector.o

main_RawMaterial.o: test/main_RawMaterial.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_RawMaterial.cpp -o build/main_RawMaterial.o

main_Manufacturing.o: test/main_Manufacturing.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM)	$(INCLUDEDESTIMES) test/main_Manufacturing.cpp -o build/main_Manufacturing.o


tests: main_Queue.o main_Process.o main_mCheck.o main_Classification.o main_Formation.o main_bake_test.o main_brokencheck_test.o main_workshop_test.o main_package_test.o main_assembly.o main_RawMaterial.o main_Inspector.o main_Manufacturing.o message.o
		$(CC) -g -o bin/QUEUE_TEST build/main_Queue.o build/message.o
		$(CC) -g -o bin/PROCESS_TEST build/main_Process.o build/message.o
		$(CC) -g -o bin/MCHECK_TEST build/main_mCheck.o build/message.o
		$(CC) -g -o bin/CLASSIFICATION_TEST build/main_Classification.o build/message.o
		$(CC) -g -o bin/FORMATION_TEST build/main_Formation.o build/message.o
		$(CC) -g -o bin/BAKE_TEST build/main_bake_test.o build/message.o
		$(CC) -g -o bin/BROKENCHECK_TEST build/main_brokencheck_test.o build/message.o
		$(CC) -g -o bin/WORKSHOP_TEST build/main_workshop_test.o build/message.o
		$(CC) -g -o bin/PACKAGE_TEST build/main_package_test.o build/message.o
		$(CC) -g -o bin/RAWMATERIAL_TEST build/main_RawMaterial.o build/message.o
		$(CC) -g -o bin/INSPECTOR_TEST build/main_Inspector.o build/message.o
		$(CC) -g -o bin/ASSEMBLY_TEST build/main_assembly.o build/message.o
		$(CC) -g -o bin/MANUFACT_TEST build/main_Manufacturing.o build/message.o


#TARGET TO COMPILE ONLY ABP SIMULATOR
simulator: main_top.o message.o
	$(CC) -g -o bin/OreoFactory build/main_top.o build/message.o
#TARGET TO COMPILE EVERYTHING (ABP SIMULATOR + TESTS TOGETHER)
all: simulator tests
#CLEAN COMMANDS
clean:
	rm -f bin/* build/*