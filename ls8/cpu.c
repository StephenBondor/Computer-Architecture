#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
	unsigned char *A = &cpu->reg[regA];
	unsigned char *B = &cpu->reg[regB];
	
  	switch (op) {
		case ALU_ADD:	*A += *B;				break; // basic math
		case ALU_SUB:	*A -= *B;				break;
		case ALU_MUL:	*A *= *B;				break;  
		case ALU_DIV:	*A /= *B;				break;
		case ALU_MOD:	*A %= *B;				break;
		case ALU_NOT: 	*A = !*A;				break; // bitwise ops
		case ALU_AND: 	*A &= *B; 				break; 
		case ALU_OR :	*A |= *B;				break; 
		case ALU_XOR:	*A ^= *B;				break;
		case ALU_DEC: 	*A = *A - 1;			break; // Other
		case ALU_INC: 	*A = *A + 1;			break;
		case ALU_SHL:	*A <<= *B;				break; 
		case ALU_SHR:	*A >>= *B;				break; 
		case ALU_CMP:	if(*A==*B){cpu->FL=1;}
				else 	if(*A >*B){cpu->FL=2;}
				else 			  {cpu->FL=4;}	break;
		default: printf("Get rekt - ALU\n"); 	exit(1);
  	}
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  	int run = 1; 
 	int IR, adv_by, opA, opB;
	unsigned char r[8];

  	// main loop
  	while (run) 
	{
		IR = ram_r(cpu, 0, -1, -1);
		adv_by = ((IR >> DATA_LEN) & 0b11) + 1;
		opA = ram_r(cpu, 1, -1, -1);
		opB = ram_r(cpu, 2, -1, -1);

		// helpers
		memcpy(r, cpu->reg, 8*sizeof(unsigned char)); // v b/c pretty v

		// main switch
		switch (IR) 
		{
			// ALU operations 
			case ADD :	alu(cpu, ALU_ADD, opA, opB); 			break; // Basic Math
			case SUB :	alu(cpu, ALU_SUB, opA, opB); 			break;
			case MUL :	alu(cpu, ALU_MUL, opA, opB); 			break;
			case DIV :	if(r[opB]){alu(cpu,ALU_DIV,opA,opB);}
				else	{run = 0;printf("ERR: div by 0.\n");}	break;
			case MOD :	if(r[opB]){alu(cpu,ALU_MOD,opA,opB);}
				else	{run = 0;printf("ERR: div by 0.\n");}	break;
			case NOT :	alu(cpu, ALU_NOT, opA, opB);			break; // Bitwise
			case AND : 	alu(cpu, ALU_AND, opA, opB); 			break;
			case OR  :	alu(cpu, ALU_OR,  opA, opB);			break;
			case XOR :	alu(cpu, ALU_XOR, opA, opB);			break;
			case CMP : 	alu(cpu, ALU_CMP, opA, opB);			break; // Other
			case DEC :	alu(cpu, ALU_DEC, opA, opB);			break;
			case INC :	alu(cpu, ALU_INC, opA, opB);			break;
			case SHL : 	alu(cpu, ALU_SHL, opA, opB);			break;
			case SHR : 	alu(cpu, ALU_SHR, opA, opB);			break;
			// CPU processes
			case PUSH:	s_push(cpu, r[opA]);					break; // Stack
			case POP :	cpu->reg[opA] = s_pop(cpu); 			break;
			case CALL: 											break; // Subroutines 
			case RET :  										break;
			case INT :											break; // Interupts
			case IRET:											break;
			case JEQ :											break; // Jumps
			case JGE :											break;
			case JGT :											break;
			case JLE : 											break;
			case JLT :											break;
			case JMP : 											break;
			case JNE :											break;
			case LD  : 	cpu->reg[opA] = ram_r(cpu,0,-1,opB);	break; // Load
			case LDI : 	cpu->reg[opA] = opB; 					break;
			case ST  :	ram_w(cpu, r[opA], r[opB]);				break;
			case PRN : 	printf("%d\n", r[opA]);					break; // Printing
			case PRA : 	printf("%c\n", r[opA]);					break;
			case NOP :											break; // Admin
			case HLT : 	run = 0; 								break;
			default  :	printf("lol, Get rekt.\n");				exit(1);
		}

		cpu->PC += adv_by;
  	}
} 

/**
 * helpers
 */
int ram_r(struct cpu *cpu, int offset, int reg_addr, int mem_addr)
{
	if (reg_addr == -1 && mem_addr == -1){ 	// reading PC directions from a program
		return cpu->ram[cpu->PC + offset];
	} else if (mem_addr == -1){			 	// reading memory by a register address
		return cpu->ram[cpu->reg[reg_addr]];
	} else {							 	// reading memory by a memory address
		return cpu->ram[mem_addr];}	
}

void ram_w(struct cpu *cpu, int address, unsigned int value)
{
	cpu->ram[address] = value;
}

void s_push(struct cpu *cpu, int value)
{
	cpu->reg[7]--;
	ram_w(cpu, cpu->reg[7], value);
}

int s_pop(struct cpu *cpu)
{
	int temp = ram_r(cpu, 0, 7, -1);
	cpu->reg[7]++; 
	return temp;
}
