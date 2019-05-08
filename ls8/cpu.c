#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
    FILE *fp;
    char line[1024];
    int address = 0;
    char *endptr;
	unsigned int value;

    fp = fopen(file, "r");

    if (fp == NULL) 
	{
        fprintf(stderr,"comp: error opening file\n");
        exit(2);
    }

    while (fgets(line, 1024, fp) != NULL) 
	{
        value = strtoul(line, &endptr, 2);
        if (endptr == line) 
		{ 
            continue; // printf("Found no digits on this line\n");
        }
		cpu_ram_write(cpu, address++, value); // printf("%u\n", value);
    }

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
	unsigned char *A = &cpu->reg[regA];
	unsigned char *B = &cpu->reg[regB];
	
  	switch (op) {
		case ALU_MUL:	*A *= *B;	break;
		case ALU_SUB:	*A -= *B;	break;
		case ALU_ADD:	*A += *B;	break;
		case ALU_DIV:	*A /= *B;	break;
		case ALU_MOD:	*A %= *B;	break;
		
		default: printf("Get rekt with love - ALU\n"); exit(1);
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
		IR = cpu_ram_read(cpu, 0);
		adv_by = ((IR >> DATA_LEN) & 0b11) + 1;
		opA = cpu_ram_read(cpu, 1);
		opB = cpu_ram_read(cpu, 2);
		memcpy(r, cpu->reg, 8*sizeof(unsigned char)); // v b/c pretty v

		// main switch
		switch (IR) 
		{
			// ALU operations -- Basic Math
			case ADD: alu(cpu, ALU_ADD, opA, opB); 			break;
			case SUB: alu(cpu, ALU_SUB, opA, opB); 			break;
			case MUL: alu(cpu, ALU_MUL, opA, opB); 			break;
			case DIV: if(r[opB]){alu(cpu,ALU_DIV,opA,opB);}
				else  {run = 0;printf("ERR: div by 0.\n");}	break;
			case MOD: if(r[opB]){alu(cpu,ALU_MOD,opA,opB);}
				else  {run = 0;printf("ERR: div by 0.\n");}	break;
			// Bitwise Crap... if we need it
			
			// Everything else
			case LDI: cpu->reg[opA] = opB; 					break;
			case PRN: printf("%d\n", r[opA]);				break;
			
			// Admin
			case HLT: run = 0; 								break;
			default:  printf("lol, Get rekt.\n");			exit(1);
		}
		cpu->PC += adv_by;
  	}
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
	cpu = malloc(sizeof(struct cpu));
	memset(cpu->reg, 0, 7);
	cpu->reg[7] = 0xF4;
	cpu->PC = 0;
	cpu->FL = 0;
	memset(cpu->ram, 0, 256);
}

/**
 * Read/write helpers
 */
int cpu_ram_read(struct cpu *cpu, int offset)
{
	return cpu->ram[cpu->PC + offset];
}

void cpu_ram_write(struct cpu *cpu, int address, unsigned int value) 
{
	cpu->ram[address] = value;
}
