#ifndef STAGE_H
#define STAGE_H
#include <string>
#include <vector>
#include "instruction.h"
class stage{
public:
    int current_cycle = 0;
    int current_inst_index = 0;
    std::vector<nopInstruction>* nop_vector;
    instruction* inst = NULL;
};

class WBStage: public stage{
public:
    WBStage(){
        nop_vector = new std::vector<nopInstruction>();
    }

    void markCycle(){
        if (inst == NULL) return;
        if (!inst->suspended)
            inst->mark_cycle(current_cycle, "WB");
        else
            inst->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool fetchInstruction(instruction* i){
        inst = i;
        return true;
    }
    void writeBack(){
        if (inst == NULL) return;
        inst->writeBack();
        inst->write_reg->usedFlag = false;
        inst = NULL;
    }
    bool passInstruction(){
        current_inst_index = -1;
        inst = NULL;
        return true;
    }
    std::string getBranchLabel(){
        if (inst == NULL) return "";
        if (inst->instruction_type.compare("bne") == 0 ||
            inst->instruction_type.compare("beq") == 0){
            std::string line = inst->line;
            //std::cout << line << line.find_last_of(",") << "\n";
            return line.substr(line.find_last_of(",")+1);
        }
        return "";
    }
};

class MEMStage: public stage{
public:
    WBStage* next;
    MEMStage(){
        nop_vector = new std::vector<nopInstruction>();
    }

    void markCycle(){
        if (inst == NULL) return;
        if (!inst->suspended)
            inst->mark_cycle(current_cycle, "MEM");
        else
            inst->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool fetchInstruction(instruction* i){
        inst = i;
        return true;
    }
    
    bool passInstruction(){
        if (next->fetchInstruction(inst)){
            inst = NULL;
            next->current_inst_index = current_inst_index;
            current_inst_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
};

class EXStage: public stage{
public:
    MEMStage* next;
    void markCycle(){
        if (inst == NULL) return;
        if (!inst->suspended)
            inst->mark_cycle(current_cycle, "EX");
        else
            inst->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }

    EXStage(){
        nop_vector = new std::vector<nopInstruction>();
    }

    void executeInstruction(){
        if (inst == NULL) return;
        inst->evaluate();
        inst->write_reg->usedFlag = true;
    }

    void forward(){
        if (inst == NULL) return;
        inst->forward();
    }

    bool passInstruction(){
        if (next->fetchInstruction(inst)){
            inst = NULL;
            next->current_inst_index = current_inst_index;
            current_inst_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
    bool fetchInstruction(instruction* i){
        inst = i;
        return true;
    }
};

class IDStage: public stage{
public:
    EXStage* next;
    bool isStalled(){
        return nop_vector->size() != 0;
    }
    int checkStalls(){
        if (inst == NULL) return 0;
        int stall_count = 0;
        if (inst->read_reg1->usedFlag && !inst->read_reg1->forwarded){
            stall_count++;
        }
        if (&inst->read_reg1 != &inst->read_reg2 && inst->read_reg2->usedFlag && !inst->read_reg2->forwarded){
            stall_count++;
        }
        return stall_count;
    }
    bool insert_stalls(int stalls_num){
        if (nop_vector->size() != 0) return false;
        for (int i = 0; i < stalls_num; i++){
            nop_vector->push_back(nopInstruction());
            for (int j = 0; j <= current_cycle; j++){
                (*nop_vector)[i].output[j] = inst->output[j];
            }
            (*nop_vector)[i].edits = inst->edits;
        }
        return true;
    }
    IDStage(){
        nop_vector = new std::vector<nopInstruction>();
    }
    bool fetchInstruction(instruction* i){
        if (inst == NULL){
            inst = i;
            return true;
        }
        if (checkStalls() == 0){
            inst = i;
            return true;
        } else{
            return false;
        }
    }
    void markCycle(){
        if (inst == NULL) return;
        if (!inst->suspended)
            inst->mark_cycle(current_cycle, "ID");
        else
            inst->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool passInstruction(){
        if (checkStalls() == 0 && next->fetchInstruction(inst)){
            inst = NULL;
            next->current_inst_index = current_inst_index;
            current_inst_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
    void markAsUsed(){
        if (inst == NULL) return;
        inst->write_reg->usedFlag = true;
    }
};

class IFStage: public stage{
public:
    IDStage* next;
    bool fetchInstruction(instruction* i, int index){
        if (inst != NULL) return false;
        current_inst_index = index;
        inst = i;
        return true;
    }
    IFStage(){
        nop_vector = new std::vector<nopInstruction>();
    }
    void markCycle(){
        if (inst == NULL) return;
        if (!inst->suspended)
            inst->mark_cycle(current_cycle, "IF");
        else
            inst->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "IF");
        }
    }
    std::string getBranchLabel(){
        if (inst == NULL) return "";
        if (inst->instruction_type.compare("bne") == 0 ||
            inst->instruction_type.compare("beq") == 0){
            std::string line = inst->line;
            //std::cout << line << line.find_last_of(",") << "\n";
            return line.substr(line.find_last_of(",")+1);
        }
        return "";
    }
    bool passInstruction(){
        if (next->fetchInstruction(inst)){
            inst = NULL;
            next->current_inst_index = current_inst_index;
            current_inst_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
};

#endif
