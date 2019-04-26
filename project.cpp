#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "simulation.h"

int main(int argc, char* argv[]){
    bool test = false;
	if (argc < 3) return EXIT_FAILURE;
    // use -t to toggle testing
    bool forwarding = !(strcmp(argv[1], "N") == 0);
    std::ifstream input_file(argv[2]);
    std::string instructions[10];
    int instruction_count = 0;
    if (input_file.is_open()){
        while (getline(input_file, instructions[instruction_count])){
            if (test){
                std::cout << instructions[instruction_count] + "\n";
            }
            ++instruction_count;
        }
        input_file.close();
    }
    Simulation s(forwarding);
    s.parseInstructions(instructions, instruction_count);
    s.simulate();
}