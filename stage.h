#ifndef STAGE_H
#define STAGE_H
#include <string>
#include <vector>
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
        if (inst->size() == 0 || instruction_index == -1) return;
        (*inst)[instruction_index]->mark_cycle(cycle_count, stage_name);
    }
    void flush(){
        if(instruction_index!=-1) {
            (*inst)[instruction_index]->terminate();
            instruction_index = -1;
        }
        //(*inst)[instruction_index]->terminate();
    }
    
public:
    void flushAll(){
        flush();
        if (next != NULL){
            next->flushAll();
        }
    }
    bool stalled = false;
    virtual bool shouldJump() = 0;
    virtual std::string getJumpLabel() = 0;
    std::string stage_name;
    int cycle_count = 0;
    std::vector<instruction*>* inst;
    // -1 is special value for no instruction currently in the stage
    int instruction_index = -1;
    bool forward;
    stage* next;
    virtual void execute() = 0;
    virtual bool isStalled(){
        return stalled;
    }
};

class IFStage: public stage{
public:
    IFStage(std::vector<instruction*>& i, bool f, stage* id){
        inst = &i;
        forward = f;
        next = id;
        stage_name = "IF";
    }
    bool shouldJump(){
        return next->shouldJump();
    }
    std::string getJumpLabel(){
        return next->getJumpLabel();
    }
    bool fetchInstruction(int index){
        unsigned int x = index;
        if (!(next->isStalled()) && x < inst->size()) {
            instruction_index = index;
            return true;//(inst[index] != NULL);
        }
        return false;
    }
    // only execute if no stalls, checked in simulation class
    void execute(){
        updateOutput();
        next->execute();
        if(!(next->isStalled())) {
            if (shouldJump()){
                flushAll();
            }
            passInstruction();
        }
        cycle_count++;
    }
    
};

// IDStage should be responisble of identifying the need to stall
class IDStage: public stage{
private:
    int checkForStall(){
        if (inst->size() == 0){
            return 0;
        }
        int stall_count = 0;
        if (instruction_index == -1) return stall_count;
        instruction* current_instruction = (*inst)[instruction_index];
        if (current_instruction->read_reg1->usedFlag == true && current_instruction->read_reg1->forwarded == false){
            stall_count++;
        } 
        if (current_instruction->read_reg2->usedFlag == true && current_instruction->read_reg2->forwarded == false){
            stall_count++;
        }
        return stall_count;
    }
public:
    bool stalled;
    bool isStalled(){
        return stalled;
    }
    IDStage(std::vector<instruction*>& i, bool f, stage* id){
        stalled = false;
        inst = &i;
        forward = f;
        next = id;
        stage_name = "ID";
    }
    bool shouldJump(){
        return next->shouldJump();
    }
    std::string getJumpLabel(){
        return next->getJumpLabel();
    }
    // check for stall, if needs to be stalled, don't pass the instruction to the next stage
    void execute(){
        next->execute();
        updateOutput();
        int stall_count = checkForStall();
        if (stall_count == 0 && instruction_index != -1){
            (*inst)[instruction_index]->write_reg->usedFlag = true;
            passInstruction();
            stalled = false;
        } else if (instruction_index != -1){
            (*inst)[instruction_index]->insert_stalls(stall_count);
            stalled = true;
        } else {
            stalled = false;
        }
        cycle_count++;
    }
};

class EXEStage: public stage{
public:
    EXEStage(std::vector<instruction*>& i, bool f, stage* id){
        inst = &i;
        forward = f;
        next = id;
        stage_name = "EX";
    }
    bool shouldJump(){
        return next->shouldJump();
    }
    std::string getJumpLabel(){
        return next->getJumpLabel();
    }
    void execute(){
        next->execute();
        if (instruction_index != -1){
            (*inst)[instruction_index]->evaluate();
        }
        updateOutput();
        passInstruction();
        cycle_count++;
    }
};

class MEMStage: public stage{
public:
    MEMStage(std::vector<instruction*>& i, bool f, stage* id){
        inst = &i;
        forward = f;
        next = id;
        stage_name = "MEM";
    }
    bool shouldJump(){
        return next->shouldJump();
    }
    std::string getJumpLabel(){
        return next->getJumpLabel();
    }
    // if forward flag is set the instruction will forward the evaluated result
    void execute(){
        next->execute();
        updateOutput();
        if (forward && instruction_index != -1){
            (*inst)[instruction_index]->forward();
        }
        passInstruction();
        cycle_count++;
    }
};

class WBStage: public stage{
public:
    bool jump = false;
    std::string jumpLabel = "";
    WBStage(std::vector<instruction*>& i, bool f, stage* id){
        inst = &i;
        forward = f;
        next = id;
        stage_name = "WB";
    }
    bool shouldJump(){
        return jump;
    }
    std::string getJumpLabel(){
        return jumpLabel;
    }
    // if forward flag is set the instruction will forward the evaluated result
    void execute(){
        if (instruction_index != -1){
            (*inst)[instruction_index]->write_reg->usedFlag = false;
            (*inst)[instruction_index]->write_reg->forwarded = false;
            (*inst)[instruction_index]->writeBack();
            if ((*inst)[instruction_index]->line.find(":") != std::string::npos){
                jump = true;
                jumpLabel = (*inst)[instruction_index]->line;
            }
        }
        updateOutput();
        cycle_count++;
    }
};
#endif