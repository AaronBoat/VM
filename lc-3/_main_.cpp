#define Memory_MAX (1 >> 16)
#define u16_t uint16_t

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
/* unix only */
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

u16_t memory[Memory_MAX];

enum REGISTER {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC, /* program counter */
  R_COND,
  R_COUNT
};
u16_t reg[R_COUNT];
enum OPCODE {
  OP_BR = 0, /* branch */
  OP_ADD,    /* add  */
  OP_LD,     /* load */
  OP_ST,     /* store */
  OP_JSR,    /* jump register */
  OP_AND,    /* bitwise and */
  OP_LDR,    /* load register */
  OP_STR,    /* store register */
  OP_RTI,    /* unused */
  OP_NOT,    /* bitwise not */
  OP_LDI,    /* load indirect */
  OP_STI,    /* store indirect */
  OP_JMP,    /* jump */
  OP_RES,    /* reserved (unused) */
  OP_LEA,    /* load effective address */
  OP_TRAP    /* execute trap */
};
enum FLAG {
  FL_POS = 1 << 0, /* P */
  FL_ZRO = 1 << 1, /* Z */
  FL_NEG = 1 << 2, /* N */
};

int read_image(const char* this_arg);
int mem_read(const u16_t addr);
void read_image_file(FILE* file);

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    /* show usage string */
    printf("lc3 [image-file1] ...\n");
    exit(2);
  }

  for (int j = 1; j < argc; ++j) {
    if (!read_image(argv[j])) {
      printf("failed to load image: %s\n", argv[j]);
      exit(1);
    }
  }

  //TODO: Setup

  reg[R_COND] = FL_ZRO;

  enum { PC_START = 0x3000 };
  reg[R_PC] = PC_START;

  int running = 1;
  while(running)
  {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    // switch (op)
    // {
    //     case OP_ADD:
    //         @{ADD}
    //         break;
    //     case OP_AND:
    //         @{AND}
    //         break;
    //     case OP_NOT:
    //         @{NOT}
    //         break;
    //     case OP_BR:
    //         @{BR}
    //         break;
    //     case OP_JMP:
    //         @{JMP}
    //         break;
    //     case OP_JSR:
    //         @{JSR}
    //         break;
    //     case OP_LD:
    //         @{LD}
    //         break;
    //     case OP_LDI:
    //         @{LDI}
    //         break;
    //     case OP_LDR:
    //         @{LDR}
    //         break;
    //     case OP_LEA:
    //         @{LEA}
    //         break;
    //     case OP_ST:
    //         @{ST}
    //         break;
    //     case OP_STI:
    //         @{STI}
    //         break;
    //     case OP_STR:
    //         @{STR}
    //         break;
    //     case OP_TRAP:
    //         @{TRAP}
    //         break;
    //     case OP_RES:
    //     case OP_RTI:
    //     default:
    //         @{BAD OPCODE}
    //         break;
  }
  //TODO:shutdown
}

void read_image_file(FILE* file)
{
    /* the origin tells us where in memory to place the image */
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);

    /* we know the maximum file size so we only need one fread */
    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);

    /* swap to little endian */
    while (read-- > 0)
    {
        *p = swap16(*p);
        ++p;
    }
}