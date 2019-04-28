#ifndef STAGE_H
#define STAGE_H
#include <string>
#include <vector>
#include "instruction.h"
class stage{
public:
    int current_cycle = 0;
    std::vector<nopInstruction>* nop_vector;
    std::vector<instruction*>* inst;
    int instruction_index = -1;
};

class WBStage: public stage{
public:
    WBStage(std::vector<instruction*>& i){
        inst = &i;
        nop_vector = new std::vector<nopInstruction>();
    }

    void markCycle(){
        if (instruction_index == -1) return;
        if (!(*inst)[instruction_index]->suspended)
            (*inst)[instruction_index]->mark_cycle(current_cycle, "WB");
        else
            (*inst)[instruction_index]->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool fetchInstruction(int index){
        instruction_index = index;
        return true;
    }
    void writeBack(){
        if (instruction_index == -1) return;
        (*inst)[instruction_index]->writeBack();
        (*inst)[instruction_index]->write_reg->usedFlag = false;
    }
    bool passInstruction(){
        instruction_index = -1;
        return true;
    }
    std::string getBranchLabel(){
        if (instruction_index == -1) return "";
        if ((*inst)[instruction_index]->instruction_type.compare("bne") == 0 ||
            (*inst)[instruction_index]->instruction_type.compare("beq") == 0){
            std::string line = (*inst)[instruction_index]->line;
            //std::cout << line << line.find_last_of(",") << "\n";
            return line.substr(line.find_last_of(",")+1);
        }
        return "";
    }
};

class MEMStage: public stage{
public:
    WBStage* next;
    MEMStage(std::vector<instruction*>& i){
        inst = &i;
        nop_vector = new std::vector<nopInstruction>();
    }

    void markCycle(){
        if (instruction_index == -1) return;
        if (!(*inst)[instruction_index]->suspended)
            (*inst)[instruction_index]->mark_cycle(current_cycle, "MEM");
        else
            (*inst)[instruction_index]->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool fetchInstruction(int index){
        instruction_index = index;
        return true;
    }
    
    bool passInstruction(){
        if (next->fetchInstruction(instruction_index)){
            instruction_index = -1;
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
        if (instruction_index == -1) return;
        if (!(*inst)[instruction_index]->suspended)
            (*inst)[instruction_index]->mark_cycle(current_cycle, "EX");
        else
            (*inst)[instruction_index]->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }

    EXStage(std::vector<instruction*>& i){
        inst = &i;
        nop_vector = new std::vector<nopInstruction>();
    }

    void executeInstruction(){
        if (instruction_index == -1) return;
        (*inst)[instruction_index]->evaluate();
        (*inst)[instruction_index]->write_reg->usedFlag = true;
    }

    void forward(){
        if (instruction_index == -1) return;
        (*inst)[instruction_index]->forward();
    }

    bool passInstruction(){
        if (next->fetchInstruction(instruction_index)){
            instruction_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
    bool fetchInstruction(int index){
        instruction_index = index;
        return true;
    }
};

class IDStage: public stage{
public:
    EXStage* next;
    int checkStalls(){
        if (instruction_index == -1) return 0;
        int stall_count = 0;
        if ((*inst)[instruction_index]->read_reg1->usedFlag && !(*inst)[instruction_index]->read_reg1->forwarded){
            stall_count++;
        }
        if (&(*inst)[instruction_index]->read_reg1 != &(*inst)[instruction_index]->read_reg2 && (*inst)[instruction_index]->read_reg2->usedFlag && !(*inst)[instruction_index]->read_reg2->forwarded){
            stall_count++;
        }
        return stall_count;
    }
    bool insert_stalls(int stalls_num){
        if (nop_vector->size() != 0) return false;
        for (int i = 0; i < stalls_num; i++){
            nop_vector->push_back(nopInstruction());
            for (int j = 0; j <= current_cycle; j++){
                (*nop_vector)[i].output[j] = (*inst)[instruction_index]->output[j];
            }
            (*nop_vector)[i].edits = (*inst)[instruction_index]->edits;
        }
        return true;
    }
    IDStage(std::vector<instruction*>& i){
        inst = &i;
        nop_vector = new std::vector<nopInstruction>();
    }
    bool fetchInstruction(int index){
        if (instruction_index == -1){
            instruction_index = index;
            return true;
        }
        if (checkStalls() == 0){
            instruction_index = index;
            return true;
        } else{
            return false;
        }
    }
    void markCycle(){
        if (instruction_index == -1) return;
        if (!(*inst)[instruction_index]->suspended)
            (*inst)[instruction_index]->mark_cycle(current_cycle, "ID");
        else
            (*inst)[instruction_index]->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "*");
        }
    }
    bool passInstruction(){
        if (next->fetchInstruction(instruction_index)){
            instruction_index = -1;
            std::vector<nopInstruction>* next_nop_vec = next->nop_vector;
            next_nop_vec->clear();
            next->nop_vector = nop_vector;
            nop_vector = next_nop_vec;
            return true;
        }
        return false;
    }
    void markAsUsed(){
        if (instruction_index == -1) return;
        (*inst)[instruction_index]->write_reg->usedFlag = true;
    }
};

class IFStage: public stage{
public:
    IDStage* next;
    bool fetchInstruction(int index){
        instruction_index = index;
        return next->fetchInstruction(next->instruction_index);
    }
    IFStage(std::vector<instruction*>& i){
        inst = &i;
        nop_vector = new std::vector<nopInstruction>();
    }
    void markCycle(){
        if (instruction_index == -1) return;
        if (!(*inst)[instruction_index]->suspended)
            (*inst)[instruction_index]->mark_cycle(current_cycle, "IF");
        else
            (*inst)[instruction_index]->mark_cycle(current_cycle, "*");
        for (unsigned int i = 0; i < nop_vector->size(); i++){
            if ((*nop_vector)[i].edits < 5)
                (*nop_vector)[i].mark_cycle(current_cycle, "IF");
        }
    }
    std::string getBranchLabel(){
        if (instruction_index == -1) return "";
        if ((*inst)[instruction_index]->instruction_type.compare("bne") == 0 ||
            (*inst)[instruction_index]->instruction_type.compare("beq") == 0){
            std::string line = (*inst)[instruction_index]->line;
            //std::cout << line << line.find_last_of(",") << "\n";
            return line.substr(line.find_last_of(",")+1);
        }
        return "";
    }
    bool passInstruction(){
        if (next->fetchInstruction(instruction_index)){
            instruction_index = -1;
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