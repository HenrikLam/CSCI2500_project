#ifndef INSTRUC_H
#define INSTRUC_H
#include <string>
#include "register.h"
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
    virtual void evaluate();
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
    addInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    andInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    orInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    sltInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    beqInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    bneInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        initializeOutput();
        program_counter = pc;
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
    labelInstruction(int* pc, Register* rr1, Register* rr2, Register* wr){
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
        stalls = 0;
    }
    void evaluate(){
        throw std::runtime_error("Attempting to evaluate a label");
    }
}
#endif
