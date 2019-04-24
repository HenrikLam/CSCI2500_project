#ifndef SIM_H
#define SIM_H
#include <iostream>
#include <string>
#include "instruction.h"
#include "stage.h"
#include "register.h"
#include <map>
#include <vector>
// to do:
//  - integrate stages
//  - simulate method (actual simulation after everything is parsed)
const int SAVED_REG_SIZE = 8;
const int TEMP_REG_SIZE = 10;
class Simulation{
private:
    instruction* parseLine(std::string& instruction){
        Register* read1;
        Register* read2;
        Register* write;

        int index = instruction.find("$");
        if (instruction[index+1] == 's'){
            write = &(saved_reg[instruction[index+2] - '0']);
        } else {
            write = &(temp_reg[instruction[index+2] - '0']);
        }

        index = instruction.find("$",index+1);
        if (instruction.find("$zero",index) == index){
            read1 = &zero;
        } else {
            if (instruction[index+1] == 's'){
                read1 = &(saved_reg[instruction[index+2] - '0']);
            } else {
                read1 = &(temp_reg[instruction[index+2] - '0']);
            }
        }

        index = instruction.find("$",index+1);
        if (instruction.find("$zero",index) == index){
            read2 = &zero;
        } else {
            if (index != -1){
                if (instruction[index+1] == 's'){
                    read2 = &(saved_reg[instruction[index+2] - '0']);
                } else {
                    read2 = &(temp_reg[instruction[index+2] - '0']);
                }
            } else {
                index = instruction.find_last_of(",");
                int i_value = std::stoi(instruction.substr(index + 1));
                read2 = new Register();
                read2->value = i_value;
            }
        }
        
        if (instruction.find("add") != std::string::npos){
            return new addInstruction(read1, read2, write, instruction);
        } else if (instruction.find("and") != std::string::npos){
            return new andInstruction(read1, read2, write, instruction);
        } else if (instruction.find("or") != std::string::npos){
            return new orInstruction(read1, read2, write, instruction);
        } else if (instruction.find("slt") != std::string::npos){
            return new sltInstruction(read1, read2, write, instruction);
        } else if (instruction.find("beq") != std::string::npos){
            return new beqInstruction(read1, read2, &jump_flag, instruction);
        } else if (instruction.find("bne") != std::string::npos){
            return new bneInstruction(read1, read2, &jump_flag, instruction);
        } else{
            throw std::runtime_error("Instruction type not supported");
        }
    }
public:
    bool forward;
    int cycle_count = 0;
    int statement_index = 0;
    Register saved_reg[SAVED_REG_SIZE];
    Register temp_reg[TEMP_REG_SIZE];
    // register for storing 1 or 0 for beq and bne
    Register jump_flag;
    Register zero;
    std::vector<instruction*> usedInstruction;
    instruction* instructions[10];
    int instruction_count;
    IFStage* stage1;
    // maps the label to the instruction line it is leading to
    std::map<std::string, int> label_map;
    Simulation(bool f){
        for (int i = 0; i < SAVED_REG_SIZE; i++){
            saved_reg[i] = Register();
        }
        for (int i = 0; i < TEMP_REG_SIZE; i++){
            temp_reg[i] = Register();
        }
        zero.value = 0;
        forward = f;
        instruction_count = 0;
        WBStage* stage5 = new WBStage(usedInstruction, f, NULL);
        MEMStage* stage4 = new MEMStage(usedInstruction, f, stage5);
        EXEStage* stage3 = new EXEStage(usedInstruction, f, stage4);
        IDStage* stage2 = new IDStage(usedInstruction, f, stage3);
        stage1 = new IFStage(usedInstruction, f, stage2);
    }
    void putInUsed(int index){
        usedInstruction.push_back(instructions[index]->copyInstruction());
    }
    int mapLabelTo(const std::string& label){
        std::map<std::string, int>::iterator itr = label_map.find(label);
        if (itr != label_map.end()){
            return itr->second;
        } else {
            return -1;
        }
    }
    void parseInstructions(std::string* instruction_strings, int inst_count){
        instruction_count = inst_count;
        int instruction_index = 0;
        for (int i = 0; i < inst_count; i++){
            instruction** current_inst = instructions + i;
            if (instruction_strings[i].find(":") != std::string::npos){
                // label instruction
                label_map[instruction_strings[i].substr(0,instruction_strings[i].length() -1)] = instruction_index;
                instruction_index--;
                instruction_count--;
            } else {
                // r-format instruction
                instructions[instruction_index] = parseLine(instruction_strings[i]);
            }
            instruction_index++;
        }
    }
    void printReg(){
        int counter = 1;
        for (int i = 0; i < SAVED_REG_SIZE; i++){
            std::cout << "$s" << i << " = " << std::setw(20) << std::left << saved_reg[i].value;
            if (counter % 4 == 0) std::cout << "\n";
            counter++;
        }
        for (int i = 0; i < TEMP_REG_SIZE; i++){
            std::cout << "$t" << i << " = " << std::setw(20) << std::left << temp_reg[i].value;
            if (counter % 4 == 0) std::cout << "\n";
            counter++;
        }
        std::cout << "\n";
    }
    void printActive(){
        std::cout << "----------------------------------------------------------------------------------\n"
                  << "CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16\n";
        for (int i = 0; i < usedInstruction.size(); i++){
            usedInstruction[i]->print_instruction();
        }
        printReg();
    }
    void simulate(){
        putInUsed(statement_index);
        if (stage1.fetchInstruction(statement_index)) {
            statement_index++;
        }
        stage1.execute();
        if(stage1.shouldJump()) {
            std::string label = stage1.getJumpLabel();
            statement_index = label_map[label];
        }
        if (cycle_count == 16) return;
        cycle_count++;
        simulate();
    }
};

#endif
