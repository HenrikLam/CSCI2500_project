#ifndef INSTRUC_H
#define INSTRUC_H
#include <string>
#include "register.h"
#include <exception>
#include <iostream>
// to do:
//  - how should jump instructions be implemented?

// contains the two read registers
// contains suspended flag for when the instruction should no longer be executed
// contains stall counter indicating how many stalls there are that are associated with this instruction

// program_counter should only be used by jump instructions
class instruction{
public:
    int* program_counter;
    Register* read_reg1;
    Register* read_reg2;
    Register* write_reg;
    int stalls;
    bool suspended = false;
    std::string output[15];
    virtual void evaluate(){
        throw std::runtime_error("evaluate an undefined instruction");
    }
    void suspend(){
        suspended = true;
    }
    void initializeOutput(){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
    }
};

class addInstruction: public instruction{
public:
    addInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        write_reg->value = read_reg1->value + read_reg2->value;
    }
};

class andInstruction: public instruction{
public:
    andInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        write_reg->value = (!!(read_reg1->value)) & (!!(read_reg2->value));
    }
};

class orInstruction: public instruction{
public:
    orInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        write_reg->value = (!!(read_reg1->value)) | (!!(read_reg2->value));
    }
};

class sltInstruction: public instruction{
public:
    sltInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        write_reg->value = read_reg1->value < read_reg2->value;
    }
};

// special case in that the write registers stores the index of the label to be stored at
class beqInstruction: public instruction{
public:
    beqInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        if (read_reg1->value == read_reg2->value){
            *program_counter = write_reg->value;
        }
    }
};

class bneInstruction: public instruction{
public:
    bneInstruction(Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        if (read_reg1->value != read_reg2->value){
            *program_counter = write_reg->value;
        }
    }
};

class labelInstruction: public instruction{
public:
    labelInstruction(Register* rr1, Register* rr2, Register* wr){
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
