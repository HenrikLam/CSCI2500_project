#include "instruction.h"
#include "assert.h"
int main (){
    Register write;
    Register read1;
    Register read2;
    Register immediate;

    read1.value = 100;
    read2.value = 10;
    addInstruction inst1(&read1, &read2, &write);
    inst1.evaluate();
    assert(write.value == 110);

    read1.value = 100;
    read2.value = -10;
    inst1.evaluate();
    assert(write.value == 90);

    read1.value = 0;
    read2.value = 0;
    orInstruction inst2(&read1, &read2, &write);
    inst2.evaluate();
    assert(write.value == 0);

    read1.value = 0;
    read2.value = 1;
    inst2.evaluate();
    assert(write.value == 1);

    read1.value = 1;
    read2.value = 1;
    inst2.evaluate();
    assert(write.value == 1);

    read1.value = 10;
    read2.value = 20;
    sltInstruction inst3(&read1, &read2, &write);
    inst3.evaluate();
    assert(write.value == 1);

    read1.value = 20;
    read2.value = 10;
    inst3.evaluate();
    assert(write.value == 0);

    read1.value = 10;
    read2.value = 10;
    inst3.evaluate();
    assert(write.value == 0);
    
}