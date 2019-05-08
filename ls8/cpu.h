#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;
  int FL;
  unsigned char reg[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_SUB,
	ALU_ADD,
	ALU_DIV,
	ALU_MOD
};

// Instructions
#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010
#define MOD  0b10100100
#define DIV  0b10100011
#define SUB  0b10100001
#define ADD  0b10100000

// Function declarations
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
int cpu_ram_read(struct cpu *cpu, int offset);
void cpu_ram_write(struct cpu *cpu, int address, unsigned int value);

#endif
