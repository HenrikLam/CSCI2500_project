#include "simulation.h"
#include <iostream>
#include <fstream>
#include <assert.h>
void testParse();

int main(){
    testParse();
}

void testParse(){
    std::string instructions[10];
    instructions[0] = "ori $s1,$zero,451";
    instructions[1] = "or $s1,$zero,451";
    instructions[2] = "addi $t2,$s0,73";
    instructions[3] = "add $t2,$s0,73";
    instructions[4] = "label:";
    instructions[5] = "and $t3, $s0, $s1";
    instructions[6] = "andi $t3, $s0, $s1";
    instructions[7] = "beq $t3, $t3, label";
    instructions[8] = "bne $t3, $zero, label";
    int instruction_count = 9;
    Simulation sim1(false);
    sim1.parseInstructions(instructions, instruction_count);
    assert(sim1.instruction_count == 8);
    assert(sim1.instructions[0].instruction_type.compare("or") == 0);
    assert(sim1.instructions[1].instruction_type.compare("or") == 0);
    assert(sim1.instructions[2].instruction_type.compare("add") == 0);
    assert(sim1.instructions[3].instruction_type.compare("add") == 0);
    assert(sim1.instructions[4].instruction_type.compare("and") == 0);
    assert(sim1.instructions[5].instruction_type.compare("and") == 0);
    assert(sim1.instructions[6].instruction_type.compare("beq") == 0);
    assert(sim1.instructions[7].instruction_type.compare("bne") == 0);
    assert(sim1.instructions[0].read_reg1 == &sim1.zero);
    assert(sim1.instructions[2].read_reg1 == sim1.saved_reg);
    assert(sim1.instructions[6].read_reg1 == sim1.temp_reg+3);
}