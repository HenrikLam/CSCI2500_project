#include "simulation.h"
#include <iostream>
#include <fstream>
#include <assert.h>
int main(int argc, char* argv[]){
    if (argc < 3) return EXIT_FAILURE;
    std::ifstream input_file("p1-input01.txt");
    std::string instructions[10];
    int instruction_count = 0;
    if (input_file.is_open()){
        while (getline(input_file, instructions[instruction_count])){
            ++instruction_count;
        }
        input_file.close();
    }
    Simulation sim1(false);
    sim1.parseInstructions(instructions, instruction_count);
    assert(sim1.instruction_count == 3);
    for (int i = 0; i < instruction_count; i++){
        sim1.instructions[i].evaluate();
    }
    assert(sim1.saved_reg[1].value == 1);
    assert(sim1.temp_reg[2].value == 74);
    assert(sim1.temp_reg[4].value == 74);

    
}