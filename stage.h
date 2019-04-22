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
public:
    int cycle_count = 0;
    int* saved_reg;
    int* temp_reg;
    int bubble_count;
    instruction* inst;
    int instruction_index;
    bool forward;
    stage* next;
    virtual void passInstruction();
    virtual void execute();
    // updates the output of the simulation
    virtual void updateOutput();
};

class IFStage: public stage{
private:
    void passInstruction(){

    }
public:
    IFStage(int* s_reg, int* t_reg, instruction* i, bool f, stage* id){
        saved_reg = s_reg;
        temp_reg = t_reg;
        bubble_count = 0;
        inst = i;
        forward = f;
        next = id;
    }
    void fetchInstruction(int index){
        instruction_index = index;
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        if (inst != NULL){
            
        }
        updateOutput();
        cycle_count++;
    }
};

// IDStage should be responisble of identifying the need to stall
class IDStage: public stage{
private:
    void passInstruction(){

    }
public:
    IDStage(int* s_reg, int* t_reg, instruction* i, bool f, stage* id){
        saved_reg = s_reg;
        temp_reg = t_reg;
        bubble_count = 0;
        inst = i;
        forward = f;
        next = id;
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        if (inst != NULL){
            
        }
        updateOutput();
        cycle_count++;
    }

    bool checkForStall(){
        if (inst == NULL){
            return false;
        }
    }
};
#endif
