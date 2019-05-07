#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
	// init
  int running = 1; // True until we get a HLT instruction
  int IR, bytes_to_next_inst, operandA, operandB;

  // main loop
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
	IR = cpu->ram[cpu->PC];

    // 2. Figure out how many operands this next instruction requires. 
	bytes_to_next_inst = ((IR >> 6) & 0b11) + 1;

    // 3. Get the appropriate value(s) of the operands following this instruction -- LOL why?.
	operandA = cpu_ram_read(cpu, 1);
	operandB = cpu_ram_read(cpu, 2);

    // 4. switch() over it to decide on a course of action. -- hmm maybe
	switch (IR) {

    	// 5. Do whatever the instruction should do according to the spec.
		case 0b10000010: // LDI
			cpu->reg[operandA] = operandB;
			break;

		case 0b01000111: // PRN
            printf("%d\n", cpu->reg[operandA]);
            break;

        case 0b00000001: // HLT
            running = 0;
            break;

		default: 
			printf("lol, this is the worst week of your life. Get rekt.");
			exit(1);
	}
    // 6. Move the PC to the next instruction.
	cpu->PC = cpu->PC + bytes_to_next_inst;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(struct cpu));
  memset(cpu->reg, 0, 7);
  cpu->reg[7] = 0xF4;
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, 256);
}

int cpu_ram_read(struct cpu *cpu, int offset)
{
	return cpu->ram[cpu->PC + offset];
}

void cpu_ram_write() 
{
// i wrote these, no idea what on earth they are for or why they are needed
// to "that access the RAM inside the `struct cpu`."
}
