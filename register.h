// int wrapper to contain the values in each registers defaulted at 0
// contains a flag for when the register is being written into
#ifndef REG_H
#define REG_H
class Register{
public:
    int value;
    bool usedFlag;
    bool forwarded = false;
    int forwardedValue = 0;
    Register(){
        value = 0;
        usedFlag = false;
    }
    // put forwardedValue into value
    void writeForwarded(){
        if (!forwarded) return;
        value = forwardedValue;
        forwarded = true;
    }
};
#endif