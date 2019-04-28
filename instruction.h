#ifndef INSTRUC_H
#define INSTRUC_H
#include <string>
#include "register.h"
#include <exception>
#include <iostream>
#include <iomanip>
// to do:
//  - how should jump instructions be implemented?
//  - allow copying of instructions for loops

// contains the two read registers
// contains suspended flag for when the instruction should no longer be executed
// contains stall counter indicating how many stalls there are that are associated with this instruction

// program_counter should only be used by jump instructions
const int MAX_CYCLE = 16;
class instruction{
public:
    std::string instruction_type;
    Register* read_reg1;
    Register* read_reg2;
    Register* write_reg;
    std::string line;
    bool suspended = false;
    int evaluatedValue;
    // 0-th index is the actual instruction
    // subsequent index are for stalls
    std::string output[MAX_CYCLE];
    virtual instruction* copyInstruction(){
        return this;
    }
    virtual void evaluate() = 0;
    void suspend(){
        suspended = true;
    }
    void writeBack(){
        write_reg->setValue(evaluatedValue);
    }
    void forward(){
        write_reg->setForwardValue(evaluatedValue);
    }
    void initializeOutput(){
        for (int i = 0; i < MAX_CYCLE; i++){
            output[i] = ".";
        }
    }
    void insert_stalls(int stall_amount){
        if (stall_amount > 2 || stall_amount < 0) throw std::runtime_error("invalid number of stalls");
    }
    void mark_cycle(int cycle, std::string stage_id){
        if (!suspended){
            output[cycle] = stage_id;
        }
        else
            output[cycle] = '*'; 
    }
    void print_instruction(){
        std::cout << std::left << std::setw(20) << line;
        for (int i = 0; i < MAX_CYCLE-1; i++){
            std::cout << std::setw(4) << output[i];
        }
        std::cout << output[MAX_CYCLE-1] << "\n";
    }
    void terminate(){
        suspended = true;
    }
};

class addInstruction: public instruction{
public:
    addInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "add";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    addInstruction(const addInstruction& other){
        instruction_type = "add";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    virtual void evaluate(){
        evaluatedValue = read_reg1->getValue() + read_reg2->getValue();
    }
    virtual addInstruction* copyInstruction(){
        return new addInstruction(*this);
    }
};

class andInstruction: public instruction{
public:
    andInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "and";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    andInstruction(const andInstruction& other){
        instruction_type = "and";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        evaluatedValue = ((read_reg1->getValue())) & ((read_reg2->getValue()));
    }
    virtual andInstruction* copyInstruction(){
        return new andInstruction(*this);
    }
};

class orInstruction: public instruction{
public:
    orInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "or";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    orInstruction(const orInstruction& other){
        instruction_type = "or";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        evaluatedValue = ((read_reg1->getValue())) | ((read_reg2->getValue()));
    }
    virtual orInstruction* copyInstruction(){
        return new orInstruction(*this);
    }
};

class sltInstruction: public instruction{
public:
    sltInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "slt";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    sltInstruction(const sltInstruction& other){
        instruction_type = "slt";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        evaluatedValue = read_reg1->getValue() < read_reg2->getValue();
    }
    virtual sltInstruction* copyInstruction(){
        return new sltInstruction(*this);
    }
};

// special case in that the write registers stores the index of the label to be stored at
class beqInstruction: public instruction{
public:
    beqInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "beq";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    beqInstruction(const beqInstruction& other){
        instruction_type = "beq";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        if (read_reg1->getValue() == read_reg2->getValue()){
            evaluatedValue = 1;
        } else {
            evaluatedValue = 0;
        }
    }
    virtual beqInstruction* copyInstruction(){
        return new beqInstruction(*this);
    }
};

class bneInstruction: public instruction{
public:
    bneInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = "bne";
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    bneInstruction(const bneInstruction& other){
        instruction_type = "bne";
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        if (read_reg1->getValue() != read_reg2->getValue()){
            evaluatedValue = 1;
        } else {
            evaluatedValue = 0;
        }
    }
    virtual bneInstruction* copyInstruction(){
        return new bneInstruction(*this);
    }
};

class labelInstruction: public instruction{
public:
    labelInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        instruction_type = instruction_str;
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    labelInstruction(const labelInstruction& other){
        instruction_type = other.instruction_type;
        initializeOutput();
        line = other.line;
        read_reg1 = other.read_reg1;
        read_reg2 = other.read_reg2;
        write_reg = other.write_reg;
    }
    void evaluate(){
        throw std::runtime_error("Attempting to evaluate a label");
    }
    virtual labelInstruction* copyInstruction(){
        return new labelInstruction(*this);
    }
};

class nopInstruction: public instruction{
public:
    nopInstruction(){
        instruction_type = "nop";
        line = "nop";
        initializeOutput();
    }
    void evaluate(){
        throw std::runtime_error("Attempting to evaluate a nop");
    }
};
#endif
