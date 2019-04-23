#ifndef INSTRUC_H
#define INSTRUC_H
#include <string>
#include "register.h"
#include <exception>
#include <iostream>
// to do:
//  - how should jump instructions be implemented?
//  - allow copying of instructions for loops

// contains the two read registers
// contains suspended flag for when the instruction should no longer be executed
// contains stall counter indicating how many stalls there are that are associated with this instruction

// program_counter should only be used by jump instructions
class instruction{
public:
    Register* read_reg1;
    Register* read_reg2;
    Register* write_reg;
    std::string line;
    int stalls;
    bool suspended = false;
    int evaluatedValue;
    // 0-th index is the actual instruction
    // subsequent index are for stalls
    std::string* output[3];
    virtual void evaluate(){
        throw std::runtime_error("evaluate an undefined instruction");
    }
    void suspend(){
        suspended = true;
    }
    void writeBack(){
        write_reg->value = evaluatedValue;
        write_reg->forwarded = false;
    }
    void forward(){
        write_reg->value = evaluatedValue;
    }
    void initializeOutput(){
        output[0] = new std::string[15];
        output[1] = new std::string[15];
        output[2] = new std::string[15];
        for (int i = 0; i < 15; i++){
            output[0][i] = ".";
            output[1][i] = ".";
            output[2][i] = ".";
        }
    }
    void insert_stalls(int stall_amount){
        if (stall_amount > 2 || stall_amount < 0) throw std::runtime_error("invalid number of stalls");
        if (stalls == 0){
            stalls += stall_amount;
            for (int i = 0; i < 15; i++){
                for (int j = 1; j <= stall_amount; j++){
                    output[j][i] = output[0][i];
                }
            }
        }
    }
    void mark_cycle(int cycle, std::string stage_id){
        if (!suspended)
            output[0][cycle-1] = stage_id;
        else
            output[0][cycle-1] = '*';
    }
    void advance_stalls(int cycle){
        for (int i = 1; i <= stalls; i++){
            output[i][cycle-1] = "*";
        }
    }
    void print_instruction(){
        for (int i = 1; i <= stalls; i++){
            std::cout << "nop\t\t";
            for (int j = 0; j < 15; j++){
                std::cout << output[i][j] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << line << "\t";
        for (int i = 0; i < 15; i++){
            std::cout << output[0][i] << "\t";
        }
    }
    void terminate(){
        suspended = true;
    }
};

class addInstruction: public instruction{
public:
    addInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        evaluatedValue = read_reg1->value + read_reg2->value;
    }
};

class andInstruction: public instruction{
public:
    andInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        evaluatedValue = (!!(read_reg1->value)) & (!!(read_reg2->value));
    }
};

class orInstruction: public instruction{
public:
    orInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        evaluatedValue = (!!(read_reg1->value)) | (!!(read_reg2->value));
    }
};

class sltInstruction: public instruction{
public:
    sltInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        evaluatedValue = read_reg1->value < read_reg2->value;
    }
};

// special case in that the write registers stores the index of the label to be stored at
class beqInstruction: public instruction{
public:
    beqInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        if (read_reg1->value == read_reg2->value){
            evaluatedValue = 1;
        } else {
            evaluatedValue = 0;
        }
    }
};

class bneInstruction: public instruction{
public:
    bneInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        initializeOutput();
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        if (read_reg1->value != read_reg2->value){
            evaluatedValue = 1;
        } else {
            evaluatedValue = 0;
        }
    }
};

class labelInstruction: public instruction{
public:
    labelInstruction(Register* rr1, Register* rr2, Register* wr, std::string& instruction_str){
        line = instruction_str;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        throw std::runtime_error("Attempting to evaluate a label");
    }
};
#endif
