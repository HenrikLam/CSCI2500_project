// int wrapper to contain the values in each registers defaulted at 0
// contains a flag for when the register is being written into
#ifndef REG_H
#define REG_H
class Register{
public:
    int value;
    bool usedFlag;
    Register(){
        value = 0;
        usedFlag = false;
    }
};
#endif