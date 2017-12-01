/* cpu.h - Headers of cpu functions.

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


/*************************
   TO BE LOCALIZED

  ******************/

/* 
int ula (int a, int b, char ula_op, int *result_ula, char *zero, char *overflow)

input arguments:
int a, int b, char ula_op

return arguments:
int *result_ula, char *zero, char *overflow */

int alu( int, int , char, int *, char *, char *);

/* 
void ControlUnit(int IR, short int *sc);

input arguments:
int IR

return arguments:
short int *sc     */

void control_unit(int, short int *);

/* 
void instruction_fetch(short int sc, int PC, int ALUOUT, int IR, int *PCnew, int *IRnew, int *MDRnew);

input arguments:
short int sc, int PC, int ALUOUT, int IR

return arguments:
int *PCnew, int *IRnew, int *MDRnew     */

void instruction_fetch(short int, int, int, int, int*, int*, int*);

/* 
void Decodifica_BuscaRegistrador(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew);

input arguments:
short int sc, int IR, int PC, int A, int B

return arguments:
int *Anew, int *Bnew, int *ALUOUTnew
*/

void decode_register(short int, int, int, int, int, int *, int *, int *);

/*
void Execucao_CalcEnd_Desvio(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew);

input arguments:
short int sc, int A, int B, int IR, int PC, int ALUOUT

return arguments:
int *ALUOUTnew, int *PCnew */

void exec_calc_end_branch(short int, int, int, int, int, int, int *, int *);

/* 
void EscreveTipoR_AcessaMemoria(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew);

input arguments:
short int sc, int B, int IR, int ALUOUT, int PC

return arguments:
int *MDRnew, int *IRnew
*/

void write_r_access_memory(short int, int, int, int, int, int *, int *);

/* 
void EscreveRefMem(short int sc, int IR, int MDR, int ALUOUT);

input arguments:
short int sc, int IR, int MDR, int ALUOUT

return arguments:
none */

void write_ref_mem(short int, int, int, int);


extern int memory[];
extern int reg[];
extern int loop;
