# Mips-Instruction-Simulator

Instructions
For reference, please use the MIPS Instruction Set Architecture PDF (available from class project2 webpage) to see
the format for each instruction and pay attention to the following changes. Your disassembler and simulator need
to support the three categories of instructions.

Category-1 

NOP, J, BEQ, BNE, BGTZ, SW, LW, BREAK

Category-2

XOR, MUL, ADD, SUB, AND, OR, ADDU, SUBU

Category-3

ORI, XORI, ADDI, SUBI, ANDI, SRL, SRA, SLL

If the instruction belongs to Category-1, the first three bits (leftmost bits) are always “001” followed by 3 bits Opcode. 
Please note that instead of using 6 bits opcode in MIPS, we use 3 bits opcode. The remaining part of the instruction binary 
is exactly the same as the original MIPS instruction set for that specific instruction.

Please pay attention to the exact instruction formats and its interpretation in MIPS instruction set manual. For
example, in case of J instruction, the format is “J target”. Here target is equal to {Most significant four bits of
(PC+4), instr_index<<2}. For BEQ, BNE and BGTZ instructions, format is “BEQ rs, rt, offset”, “BNE rs, rt,
offset” and “BGTZ rs, offset” respectively. Please note that we do not consider delay slot for this project. In other
words, an instruction following the branch instruction should be treated as a regular instruction (see simulation.txt).

Instruction Opcode

NOP 000

J 001

BEQ 010

BNE 011

BGTZ 100

SW 101

LW 110

BREAK 111

If the instruction belongs to Category-2 which has the form “dest ← src1 op src2”, the first three bits (leftmost
three bits) are always “010”. The next three bits denotes opcode. Then the following 5 bits
serve as dest. The next 5 bits for src1, followed by 5 bits for src2. Dest, src1 as well as src2 are registers. The
remaining bits are all 0’s.

Instruction Opcode

XOR 000

MUL 001

ADD 010

SUB 011

AND 100

OR 101

ADDU 110

SUBU 111

If the instruction belongs to Category-3 which has the form “dest ← src1 op immediate_value”, the first three bits
(leftmost three bits) are always “100”. The subsequent 5 bits serve as dest followed by 5 bits for src1. The second 
source operand is immediate 16-bit value.

Instruction Opcode

ORI 000

XORI 001

ADDI 010

SUBI 011

ANDI 100

SRL 101

SRA 110

SLL 111

Assume that for SRL, SRA and SLL instructions, shift amount (sa) is the least significant 5 bits of the
immediate_value.

#Input/Output Files

Your program will be given a text input file (see sample.txt). This file will contain a sequence of 32-bit instruction
words which begin at address "64". The final instruction in the sequence of instructions is always BREAK, and
there will be only one BREAK instruction. Following the BREAK instruction (immediately after BREAK), there is
a sequence of 32-bit 2's complement signed integers for the program data up to the end of the file.

The disassembler output file should contain 3 columns of data with each column separated by one tab character (‘\t’
or char(9)).

1. The text (e.g., 0’s and 1’s) string representing the 32-bit instruction word.

2. The address (in decimal) of that instruction

3. The disassembled instruction.

Note, if you are displaying an instruction, the third column should contain every part of the instruction, with each
argument separated by a comma and then a space (“, ”). See disassembly.txt for examples.

The simulation output file should have the following format:

* 20 hyphens and a new line

* Cycle < cycleNumber >:< tab >< instr_Address >< tab >< instr_string >

* < blank_line >

* Registers

* R00: < tab >< int(R0) >< tab >< int(R1) >...< tab >< int(R7) >

* R08: < tab >< int(R8) >< tab >< int(R9) >...< tab >< int(R15) >

* R16: < tab >< int(R16) >< tab >< int(R17) >...< tab >< int(R23) >

* R24: < tab >< int(R24) >< tab >< int(R25) >...< tab >< int(R31) >

* < blank_line >

* Data

* < firstDataAddress >: < tab >< display 8 data words as integers with tabs in between >

* ..... < continue until the last data word > 






















































