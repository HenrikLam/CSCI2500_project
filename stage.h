#ifndef STAGE_H
#define STAGE_H
#include <string>
#include "instruction.h"
// to do:
//  - everything xd

// inst is the point to the beginning of the array of instructions
// instruction_index keeps track of what instruction the stage is
//  executing

class stage{
protected:
    void passInstruction(){
        next->instruction_index = instruction_index;
        instruction_index = -1;
    }
    // updates the output of the simulation
    void updateOutput(){
        if (inst == NULL || instruction_index == -1) return;
        inst[instruction_index].mark_cycle(cycle_count, stage_name);
    }
public:
    std::string stage_name;
    int cycle_count = 0;
    int* saved_reg;
    int* temp_reg;
    instruction* inst;
    // -1 is special value for no instruction currently in the stage
    int instruction_index = -1;
    bool forward;
    stage* next;
    virtual void passInstruction();
    virtual void execute();
};

class IFStage: public stage{
public:
    IFStage(int* s_reg, int* t_reg, instruction* i, bool f, stage* id){
        saved_reg = s_reg;
        temp_reg = t_reg;
        inst = i;
        forward = f;
        next = id;
        stage_name = "IF";
    }
    void fetchInstruction(int index){
        instruction_index = index;
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        next->execute();
        if (inst != NULL){
            passInstruction();
        }
        updateOutput();
        cycle_count++;
    }
    
};

// IDStage should be responisble of identifying the need to stall
class IDStage: public stage{
private:
    int checkForStall(){
        if (inst == NULL){
            return false;
        }
        int stall_count = 0;
        if (instruction_index == -1) return stall_count;
        instruction current_instruction = inst[instruction_index];
        if (current_instruction.read_reg1->usedFlag == true && current_instruction.read_reg1->forwarded == false){
            stall_count++;
        } 
        if (current_instruction.read_reg2->usedFlag == true && current_instruction.read_reg1->forwarded == true){
            stall_count++;
        }
        return stall_count;
    }
public:
    IDStage(int* s_reg, int* t_reg, instruction* i, bool f, stage* id){
        saved_reg = s_reg;
        temp_reg = t_reg;
        inst = i;
        forward = f;
        next = id;
        stage_name = "ID";
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        next->execute();
        if (inst != NULL && instruction_index != -1){
            int stall_count = checkForStall();
            if (stall_count == 0){
                passInstruction();
            } else {
                inst[instruction_index].insert_stalls(stall_count);
            }
        }
        updateOutput();
        cycle_count++;
    }
};

class EXEStage: public stage{
public:
    EXEStage(int* s_reg, int* t_reg, instruction* i, bool f, stage* id){
        saved_reg = s_reg;
        temp_reg = t_reg;
        inst = i;
        forward = f;
        next = id;
        stage_name = "EXE";
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        next->execute();
        if (inst != NULL){
            inst[instruction_index].evaluate();
            passInstruction();
        }
        updateOutput();
        cycle_count++;
    }
};

class WBStage: public stage{

}
#endif
