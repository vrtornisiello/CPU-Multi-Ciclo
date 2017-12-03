/* cpu.c - The CPU.

   Copyright 2017  Monaco F. J.   <monaco@icmc.usp.br>

   This file is part of Muticlycle Mips CPU (MMCPU)

   MMCPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>


#include "mask.h"		/* Useful masks; customize at you please. */
#include "cpu.h"		/* CPU functions' headers. */


/* Global vars. */

int memory[MAX];		/* Main memory. */

int reg[NUMREG];		/* Register bank */

int loop = 1;			/* Main loop control. */

int main (int argc, char *argv[])
{

  /* Auxiliary registers used for writing during a single cycle. They store the
     cycle result so that it is available for reading in at the beginning of the
     next cycle. */

  int PCnew = 0, IRnew, MDRnew, Anew, Bnew, ALUOUTnew;

  /* Special registers used for reading during a single cycle. They store
     results that may be utilized during the current cycle, since they have been
     written during the previous cycle. Eg.: the ALU result is initially stored
     in ALUOUTnew; only at the end of the cycle the value is stored in ALUOUT so
     that it can be read during the next cycle. */

  int PC = 0, IR = -1, MDR, A, B, ALUOUT;

  /* Control signals (16 signals). Each bit determines one of the control signals
     leaving the control unit. The position of each signal in the short int is
     specified at mask.h. */

  short int sc = 0;

  /* ALU input control signal. Only 4 less-significant bits are used. */

  char ula_op = 0;

  int nr_cycles = 0; 		/* Counts the number of executed cycles. */

    /* Variables zero and overflow are return arguments and therefore don't
       need to be initialized. */

    /* Memory. */
    memory[0] = 0x8c480000;  // 1000 1100 0100 1000 0000 0000 0000 0000  lw $t0, 0($v0) *5*
    memory[1] = 0x010c182a;  // 0000 0001 0000 1100 0001 1000 0010 1010  slt $v1, $t0, $t4 *4*
    memory[2] = 0x106d0004;  // 0001 0000 0110 1101 0000 0000 0000 0100  beq $v1, $t5, fim(4 palavras abaixo de PC+4) *3*
    memory[3] = 0x01084020;  // 0000 0001 0000 1000 0100 0000 0010 0000  add $t0, $t0, $t0 *4*
    memory[4] = 0xac480000;  // 1010 1100 0100 1000 0000 0000 0000 0000  sw $t0, 0($v0) *4*
    memory[5] = 0x004b1020;  // 0000 0000 0100 1011 0001 0000 0010 0000  add $v0, $t3, $v0 *4*
    memory[6] = 0x08000000;  // 0000 1000 0000 0000 0000 0000 0000 0000  j inicio (palavra 0) *3*
    memory[7] = 0;           // fim (criterio de parada do programa)    (27*6)+(5+4+3)+1
    memory[8] = 0;
    memory[9] = 0;

    /* Data. */
    memory[20] = 10;
    memory[21] = 12;
    memory[22] = 14;
    memory[23] = 16;
    memory[24] = 18;
    memory[25] = 20;
    memory[26] = -1;

    /* Registers. */
    reg[2]  = 20; // $v0
    reg[11] = 1;  // $t3
    reg[12] = 0;  // $t4
    reg[13] = 1;  // $t5

    while(loop)
    {

	/* Starting a new cycle. */

	/* The following functional units shall be executed at every cycle.
	   The control signals in sc will enable/disable the effective execution. */
        control_unit(IR, &sc);
        instruction_fetch(sc, PC, ALUOUT, IR, &PCnew, &IRnew, &MDRnew);
        decode_register(sc, IR, PC, A, B, &Anew, &Bnew, &ALUOUTnew);
        exec_calc_end_branch(sc, A, B, IR, PC, ALUOUT, &ALUOUTnew, &PCnew);
        write_r_access_memory(sc, B, IR, ALUOUT, PC, &MDRnew, &IRnew);
        write_ref_mem(sc, IR, MDR, ALUOUT);

	/* Count cycle. */
        nr_cycles++;

	/* Update temporary registers for the next cycle. */
        PC = PCnew;
        IR = IRnew;
        MDR = MDRnew;
        A = Anew;
        B = Bnew;
        ALUOUT = ALUOUTnew;

	/* End of cycle. */

    }

    /* Dump memory for verifying the result. */

    {
        int ii;
        for (ii = 20; ii < 27; ii++) {
            printf("memory[%d]=%d\n", ii, memory[ii]);
    }

        printf("N. executed cycles =%d\n", nr_cycles);
    }

    return(EXIT_SUCCESS);
}
