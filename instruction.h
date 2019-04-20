#ifndef INSTRUC_H
#define INSTRUC_H
#include <string>
class instruction{
public:
    int* program_counter;
    int* read_reg1;
    int* read_reg2;
    int* write_reg;
    std::string output[15];
    virtual void evaluate();
};

class addInstruction: public instruction{
public:
    addInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        *write_reg = *read_reg1 + *read_reg2;
    }
};

class andInstruction: public instruction{
public:
    andInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        *write_reg = (!!(*read_reg1)) & (!!(*read_reg2));
    }
};

class orInstruction: public instruction{
public:
    orInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        *write_reg = (!!(*read_reg1)) | (!!(*read_reg2));
    }
};

class sltInstruction: public instruction{
public:
    sltInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        *write_reg = (*read_reg1) < (*read_reg2);
    }
};

class beqInstruction: public instruction{
public:
    beqInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        if (*read_reg1 == *read_reg2){
            *program_counter = *write_reg;
        }
    }
};

class bneInstruction: public instruction{
public:
    bneInstruction(int* pc, int* rr1, int* rr2, int* wr){
        for (int i = 0; i < 15; i++){
            output[i] = ".";
        }
        program_counter = pc;
        read_reg1 = rr1;
        read_reg2 = rr2;
        write_reg = wr;
    }
    void evaluate(){
        if (*read_reg1 != *read_reg2){
            *program_counter = *write_reg;
        }
    }
};

#endif
