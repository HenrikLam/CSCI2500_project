#include "instruction.h"
#include "assert.h"
int main (){
    Register write;
    Register read1;
    Register read2;
    Register immediate;
    std::string line;
    read1.value = 100;
    read2.value = 10;
    line = "add test";
    addInstruction inst1(&read1, &read2, &write, line);
    inst1.evaluate();
    assert(write.value == 110);

    read1.value = 100;
    read2.value = -10;
    inst1.evaluate();
    assert(write.value == 90);

    read1.value = 0;
    read2.value = 0;
    line = "or test";
    orInstruction inst2(&read1, &read2, &write, line);
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
    line = "slt test";
    sltInstruction inst3(&read1, &read2, &write, line);
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
    
    read1.value = 10;
    read2.value = 20;
    line = "beq test";
    beqInstruction inst4(&read1, &read2, &write, line);
    inst4.evaluate();
    assert(write.value == 0);

    read1.value = 20;
    read2.value = 10;
    inst4.evaluate();
    assert(write.value == 0);

    read1.value = 10;
    read2.value = 10;
    inst4.evaluate();
    assert(write.value == 1);

    read1.value = 10;
    read2.value = 20;
    line = "bne test";
    beqInstruction inst5(&read1, &read2, &write, line);
    inst5.evaluate();
    assert(write.value != 1);

    read1.value = 20;
    read2.value = 10;
    inst5.evaluate();
    assert(write.value != 1);

    read1.value = 10;
    read2.value = 10;
    inst5.evaluate();
    assert(write.value != 0);

    line = "print test";
    addInstruction inst6(&read1, &read2, &write, line);
    
    inst6.mark_cycle(1, "ID");
    inst6.mark_cycle(2, "IF");
    inst6.insert_stalls(2);
    inst6.mark_cycle(3, "EXE");
    inst6.advance_stalls(3);
    inst6.mark_cycle(4, "MEM");
    inst6.advance_stalls(4);
    inst6.terminate();
    inst6.mark_cycle(5, "WB");
    inst6.advance_stalls(5);
    inst6.print_instruction();
}