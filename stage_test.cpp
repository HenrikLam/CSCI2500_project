#include "stage.h"
#include "instruction.h"
#include <assert.h>
int main(){
    Register write1;
    Register write2;
    Register read1;
    Register read2;
    Register read3;
    Register read4;
    
    std::vector<instruction*> instructions;
    std::string line1 = "add1";
    std::string line2 = "add2";
    std::string line3 = "add3";
    std::string line4 = "beq $s1,$s2,branch";

    addInstruction inst1(&read1, &read2, &write1, line1);
    addInstruction inst2(&read1, &read2, &write1, line2);
    addInstruction inst3(&write1, &read2, &write1, line3);
    beqInstruction inst4(&read1, &read2, &write2, line4);

    instructions.push_back(&inst1);
    instructions.push_back(&inst2);
    instructions.push_back(&inst3);
    instructions.push_back(&inst4);

    IFStage stage_if(instructions);
    IDStage stage_id(instructions);
    stage_if.next = &stage_id;
    stage_if.fetchInstruction(0);
    assert(stage_if.instruction_index == 0);
    assert(stage_if.inst == &instructions);
    //stage_if.markCycle();
    //inst1.print_instruction();

    stage_if.fetchInstruction(3);
    assert(stage_if.getBranchLabel().compare("branch") == 0);

    stage_id.fetchInstruction(0);
    read1.usedFlag = true;
    assert(stage_id.checkStalls() == 1);
    stage_id.insert_stalls(1);
    instructions.insert(instructions.begin()+1,&stage_id.nop_vector->at(0));
    //stage_id.markCycle();
    assert(stage_id.inst->at(0) == &inst1);
    //instructions[0]->print_instruction();
    //instructions[1]->print_instruction();
    stage_id.current_cycle = 1;
    read1.usedFlag = false;

    EXStage stage_ex(instructions);
    read1.setValue(1);
    read2.setValue(2);
    stage_ex.fetchInstruction(0);
    stage_ex.executeInstruction();
    assert(write1.usedFlag);
    assert(inst1.evaluatedValue == 3);
    //read1.setValue(0);
    //read2.setValue(0);

    MEMStage stage_mem(instructions);
    stage_mem.fetchInstruction(0);
    stage_ex.forward();
    assert(write1.forwardedValue == 3);
    
    WBStage stage_wb(instructions);
    stage_wb.fetchInstruction(0);
    stage_wb.writeBack();
    assert(write1.value == 3);
    
    // integraton testing
    stage_if.next = &stage_id;
    stage_id.next = &stage_ex;
    stage_ex.next = &stage_mem;
    stage_mem.next = &stage_wb;

    stage_mem.instruction_index = -1;
    stage_if.instruction_index = -1;
    stage_id.instruction_index = -1;
    stage_ex.instruction_index = -1;
    stage_wb.instruction_index = -1;
    
    stage_if.current_cycle = 0;
    stage_id.current_cycle = 0;
    stage_ex.current_cycle = 0;
    stage_mem.current_cycle = 0;
    stage_wb.current_cycle = 0;

    std::vector<nopInstruction>* id_old_nop_vec = stage_id.nop_vector;
    stage_id.passInstruction();
    assert(stage_ex.nop_vector == id_old_nop_vec);
    assert(stage_id.nop_vector->size() == 0);

    instructions.erase(instructions.begin());
    instructions.erase(instructions.begin());

//--------------------------------------------------------------------------------------------------------------------------------
    stage_if.fetchInstruction(0);
    assert(stage_if.getBranchLabel().compare("") == 0);
    assert(instructions.at(0) == &inst2);

    stage_if.markCycle();
    stage_id.markCycle();
    stage_ex.markCycle();
    stage_mem.markCycle();
    stage_wb.markCycle();
    
    instructions[0]->print_instruction();
    instructions[1]->print_instruction();
    instructions[2]->print_instruction();
    std::cout << "\n";

    stage_mem.passInstruction();
    stage_ex.passInstruction();
    stage_id.passInstruction();
    stage_if.passInstruction();

    stage_if.current_cycle++;
    stage_id.current_cycle++;
    stage_ex.current_cycle++;
    stage_mem.current_cycle++;
    stage_wb.current_cycle++;

//--------------------------------------------------------------------------------------------------------------------------------
    assert(stage_id.instruction_index == 0);
    stage_if.fetchInstruction(1);
    assert(stage_if.getBranchLabel().compare("") == 0);

    stage_if.markCycle();
    stage_id.markCycle();
    stage_ex.markCycle();
    stage_mem.markCycle();
    stage_wb.markCycle();

    instructions[0]->print_instruction();
    instructions[1]->print_instruction();
    instructions[2]->print_instruction();
    std::cout << "\n";

    stage_mem.passInstruction();
    stage_ex.passInstruction();
    stage_id.passInstruction();
    stage_if.passInstruction();

    stage_if.current_cycle++;
    stage_id.current_cycle++;
    stage_ex.current_cycle++;
    stage_mem.current_cycle++;
    stage_wb.current_cycle++;

//--------------------------------------------------------------------------------------------------------------------------------
    assert(stage_id.instruction_index == 1);
    stage_if.fetchInstruction(2);
    assert(stage_if.getBranchLabel().compare("branch") == 0);

    stage_if.markCycle();
    stage_id.markCycle();
    stage_ex.markCycle();
    stage_mem.markCycle();
    stage_wb.markCycle();

    stage_id.markAsUsed();
    instructions[0]->print_instruction();
    instructions[1]->print_instruction();
    instructions[2]->print_instruction();
    std::cout << "\n";

    stage_mem.passInstruction();
    stage_ex.passInstruction();
    assert(stage_id.checkStalls() == 1);
    stage_id.insert_stalls(1);
    instructions.insert(instructions.begin()+1, &(stage_id.nop_vector->at(0)));
    stage_id.instruction_index += 1;
    stage_if.instruction_index += 2;
    //stage_id.passInstruction();
    stage_if.passInstruction();

    stage_if.current_cycle++;
    stage_id.current_cycle++;
    stage_ex.current_cycle++;
    stage_mem.current_cycle++;
    stage_wb.current_cycle++;

//--------------------------------------------------------------------------------------------------------------------------------

    stage_if.markCycle();
    stage_id.markCycle();
    stage_ex.markCycle();
    stage_mem.markCycle();
    stage_wb.markCycle();

    instructions[0]->print_instruction();
    instructions[1]->print_instruction();
    instructions[2]->print_instruction();
    instructions[3]->print_instruction();
    std::cout << "\n";

    stage_mem.passInstruction();
    stage_ex.passInstruction();
    stage_id.passInstruction();
    stage_if.passInstruction();

    stage_if.current_cycle++;
    stage_id.current_cycle++;
    stage_ex.current_cycle++;
    stage_mem.current_cycle++;
    stage_wb.current_cycle++;

//--------------------------------------------------------------------------------------------------------------------------------

    stage_if.markCycle();
    stage_id.markCycle();
    stage_ex.markCycle();
    stage_mem.markCycle();
    stage_wb.markCycle();

    instructions[0]->print_instruction();
    instructions[1]->print_instruction();
    instructions[2]->print_instruction();
    instructions[3]->print_instruction();
    std::cout << "\n";

    stage_mem.passInstruction();
    stage_ex.passInstruction();
    stage_id.passInstruction();
    stage_if.passInstruction();

    stage_wb.writeBack();

    stage_if.current_cycle++;
    stage_id.current_cycle++;
    stage_ex.current_cycle++;
    stage_mem.current_cycle++;
    stage_wb.current_cycle++;
}