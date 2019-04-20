#include <iostream>
#include <string>
#include "instruction.h"
#include "stage.h"

class Simulation{
    bool forward;
    int saved_reg[7];
    int temp_reg[10];
    instruction instructions[10];
    Simulation(bool f){
        forward = f;
    }
};