#ifndef SIM_H
#define SIM_H
#include <iostream>
#include <string>
#include "instruction.h"
#include "stage.h"
#include <map>
class Simulation{
private:
    instruction parseRFormat(std::string& instruction){
        Register* read1;
        Register* read2;
        Register* write;

        int index = instruction.find("$");
        if (instruction[index+1] == 's'){
            write = saved_reg + (instruction[index+2] - '0');
        } else {
            write = temp_reg + (instruction[index+2] - '0');
        }

        int index = instruction.find("$",index+1);
        if (instruction[index+1] == 's'){
            read1 = saved_reg + (instruction[index+2] - '0');
        } else {
            read1 = temp_reg + (instruction[index+2] - '0');
        }

        int index = instruction.find("$",index+1);
        if (instruction[index+1] == 's'){
            read2 = saved_reg + (instruction[index+2] - '0');
        } else {
            read2 = temp_reg + (instruction[index+2] - '0');
        }
        if (instruction.find("addi") != std::string::npos){
            
        } else if (instruction.find("add") != std::string::npos){

        } else if (instruction.find("andi") != std::string::npos){

        } else if (instruction.find("and") != std::string::npos){

        } else if (instruction.find("ori") != std::string::npos){

        } else if (instruction.find("or") != std::string::npos){

        } else if (instruction.find("slti") != std::string::npos){

        } else if (instruction.find("slt") != std::string::npos){

        } else {
            throw std::runtime_error("Instruction type not supported");
        }
    }
public:
    bool forward;
    Register saved_reg[7];
    Register temp_reg[10];
    instruction instructions[10];
    int instruction_count;
    int instruction_count;
    // maps the label to the instruction line it is leading to
    std::map<std::string, int> label_map;
    Simulation(bool f){
        forward = f;
        instruction_count = 0;
    }
    void parseInstructions(std::string* instruction_strings, int inst_count){
        instruction_count = inst_count;
        int instruction_index = 0;
        for (int i = 0; i < inst_count; i++){
            bool is_label = false;
            instruction* current_inst = instructions + i;
            if (instruction_strings[i].find("beq") != std::string::npos){

            } else if (instruction_strings[i].find("bne") != std::string::npos){

            } else if (instruction_strings[i].find(":") != std::string::npos){
                // label instruction
                is_label = true;
                instruction_index--;
            } else {
                // r-format instruction
            }
        }
    }
};

#endif
