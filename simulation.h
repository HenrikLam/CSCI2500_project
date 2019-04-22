#ifndef SIM_H
#define SIM_H
#include <iostream>
#include <string>
#include "instruction.h"
#include "stage.h"
#include "register.h"
#include <map>
// to do:
//  - parseLFormat
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

        index = instruction.find("$",index+1);
        if (instruction.find("$zero",index) == index){
            read1 = &zero;
        } else {
            if (instruction[index+1] == 's'){
                read1 = saved_reg + (instruction[index+2] - '0');
            } else {
                read1 = temp_reg + (instruction[index+2] - '0');
            }
        }

        index = instruction.find("$",index+1);
        if (instruction.find("$zero",index) == index){
            read1 = &zero;
        } else {
            if (index != -1){
                if (instruction[index+1] == 's'){
                    read2 = saved_reg + (instruction[index+2] - '0');
                } else {
                    read2 = temp_reg + (instruction[index+2] - '0');
                }
            } else {
                index = instruction.find_last_of(",");
                int i_value = std::stoi(instruction.substr(index + 1));
                read2 = new Register();
                read2->value = i_value;
            }
        }
        
        if (instruction.find("add") != std::string::npos){
            return addInstruction(read1, read2, write);
        } else if (instruction.find("and") != std::string::npos){
            return andInstruction(read1, read2, write);
        } else if (instruction.find("or") != std::string::npos){
            return orInstruction(read1, read2, write);
        } else if (instruction.find("slt") != std::string::npos){
            return sltInstruction(read1, read2, write);
        } else {
            throw std::runtime_error("Instruction type not supported");
        }
    }
public:
    bool forward;
    Register saved_reg[7];
    Register temp_reg[10];
    Register zero;
    instruction instructions[10];
    int instruction_count;
    // maps the label to the instruction line it is leading to
    std::map<std::string, int> label_map;
    Simulation(bool f){
        zero.value = 0;
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
                instructions[instruction_index] = parseRFormat(instruction_strings[i]);
            }
            instruction_index++;
        }
    }
};

#endif
