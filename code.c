/* code-c - Student's code for mmcpu

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

/* After you have implemented your functions, you may safely remove these lines. */
#include <stdio.h>
#include <stdlib.h>
#include "mask.h"
#include "cpu.h"
#define not_implemented() fprintf(stderr, "Not implemented\n"); exit(EXIT_FAILURE)

#define separa_4bits_func 0xF  //000000 00000 00000 00000 00000 001111

/* Students, you are required to implemented the functions bellow.
   Please, refer to cpu.h for further information. */

char alu_control(int IR, short int sc)
{
    sc &= separa_ALUOp0;
    sc |= separa_ALUOp1;

    sc >>= 5;

    switch(sc){
        case (short int)0b00:
            return ativa_soma; //0b0010 - soma
            break;

        case (short int)0b01:
            return ativa_subtracao; //0b0110 - subtracao
            break;

        default:
            IR &= separa_4bits_func;
            break;
    }
    switch(IR){
        case 0b0000:
            return ativa_soma; //0b0010 - soma
            break;

        case 0b0010:
            return ativa_subtracao; //0b0110 - subtracao
            break;

        case 0b0100:
            return ativa_and; //0b0000 - and
            break;

        case 0b0101:
            return ativa_or; //0b0001 - or
            break;

        case 0b1010:
            return ativa_slt; //0b0111 - SLT
            break;

        default:
            break;
    }
}

int alu(int a, int b, char alu_op, int *result_alu, char *zero, char *overflow)
{
    switch(alu_op){
        case ativa_soma:
            *result_alu = a + b;
            break;
        case ativa_subtracao:
            *result_alu = a - b;
            break;
        case ativa_and:
            *result_alu = a & b;
            break;
        case ativa_or:
            *result_alu = a | b;
            break;
        case ativa_slt:
            if(a < b)
                *result_alu = 1;
            else
                *result_alu = 0;
			break;
        default:
            break;
    }
    if(*result_alu == 0)
        *zero |= ativa_bit_zero;
    else
        *zero &= desativa_bit_zero;

  return 0;
}

void control_unit(int IR, short int *sc)
{
    if(IR == -1){
        *sc = 0b1001010000001000; //estado 0
        return;
    }

    if(*sc == (short int)0b1001010000001000){ //caso esteja no estado 0
        *sc = 0b1001010000011000; //estado 1
        return;
    }

    char opcode = ((IR & separa_cop) >> 26);

    switch(opcode){
        case 0x0: //caso seja tipo R
            if(*sc == (short int)0b1001010000011000){ // se estiver no estado 1, vai pro 6
                *sc = 0b1001010001000100;
            }
            else if(*sc == (short int)0b1001010001000100){ // se estiver no estado 6, vai pro 7
               *sc = 0b1001010001000111;
            }
            else if(*sc == (short int)0b1001010001000111){ //se estiver no estado 7, vai pro 0
                *sc = 0b1001010000001000;
            }
            break;
        case 0x23: //caso seja lw
            if(*sc == (short int)0b1001010000011000){ //se estiver no estado 1, vai pro 2
                *sc = 0b1001010000010100;
            }
            else if(*sc == (short int)0b1001010000010100){ //se estiver no estado 2, vai pro 3
                *sc = 0b1001110000010100;
            }
            else if(*sc == (short int)0b1001110000010100){ //se estiver no estado 3, vai pro 4
                *sc = 0b1001110000010111;
            }
            else if(*sc == (short int)0b1001110000010111){ //se estiver no estado 4, vai pro 0
                *sc = 0b1001010000001000;
            }
            break;
        case 0x2b: //caso seja sw
            if(*sc == (short int)0b1001010000011000){ //se estiver no estado 1, vai pro 2
                *sc = 0b1001010000010100;
            }
            else if(*sc == (short int)0b1001010000010100){ //se estiver no estado 2, vai pro 5
                *sc = 0b1011110000011000;
            }
            else if(*sc == (short int)0b1011110000011000){ //se estiver no estado 5, vai pro 0
                *sc = 0b1001010000001000;
            }
            break;
        case 0x4: //caso seja BEQ
            if(*sc == (short int)0b1001010000011000){ //se estiver no estado 1, vai pro 8
                *sc = 0b1001011010100100;
            }
            else if(*sc == (short int)0b1001011010100100){ //se estiver no estado 8, vai pro 0
                *sc = 0b1001010000001000;
            }
            break;
        case 0x2: //caso seja Jump
            if(*sc == (short int)0b1001010000011000){ //se estiver no estado 1, vai pro 9
                *sc = 0b1001010100011000;
            }
            else if(*sc == (short int)0b1001010100011000){ //se estiver no estado 9, vai pro 0
                *sc = 0b1001010000001000;
            }
            break;
        default:
            break;
    }
    return;
}

void instruction_fetch(short int sc, int PC, int ALUOUT, int IR, int* PCnew, int* IRnew, int* MDRnew)
{
    char zero, overflow;

    if(sc == (short int)0b1001010000001000){ //estado 0
        *IRnew = memory[PC];
        alu(PC, 1, ativa_soma, PCnew, &zero, &overflow);
    }

    if(IR == 0){
        loop = 0;
    }
    return;
}

void decode_register(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew)
{
    char zero, overflow;

    if(sc == (short int)0b1001010000011000){

        int rs = ((IR & separa_rs) >> 21);
        int rt = ((IR & separa_rt) >> 16);

        *Anew = reg[rs];
        *Bnew = reg[rt];

        alu(PC, ((IR & separa_imediato) << 2), ativa_soma, ALUOUTnew, &zero, &overflow);
    }
    return;
}

void exec_calc_end_branch(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
    char zero = 0, overflow, alu_op;

    switch(sc){
        case (short int)0b1001010000010100:
            *ALUOUTnew = A + (IR & separa_imediato);//soma esses bits com o conteúdo de rs que foi armazenado em A no ciclo passado     }
            break;

        case (short int)0b1001010001000100: //se estiver no estado 6 (tipo R)
            alu_op = alu_control(IR, sc);
            alu(A, B, alu_op, ALUOUTnew, &zero, &overflow); //realiza a operacao na ula e armazena o resultado em ALUOUTnew
            break;

        case (short int)0b1001011010100100: //se estiver no estado 8 (BEQ)
            alu(A, B, ativa_subtracao, ALUOUTnew, &zero, &overflow); //verifica se A = B
            if(zero == 1)
                *PCnew = ALUOUT; //se for igual, PC recebe o endereço do branch calculado no ciclo anterior
            break;

        case (short int)0b1001010100011000: //se estivar no estado 9 (Jump)
            PC &= separa_4bits_PC;          //pega os 4 bits mais significativos de PC - PC[31-28]
            IR &= separa_endereco_jump;     //pega os 26 bits menos significativos de IR - IR[25-0]
            *PCnew = PC | IR;               //concatena os bits de PC com IR.
            break;

        default:
            break;
     }
     return;
}

void write_r_access_memory(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew)
{
    int rd = ((IR & separa_rd) >> 11); //pega o registrador de destino rd

    switch(sc){
        case (short int)0b1001110000010100: //se estiver no estado 3 (LW)
            *MDRnew = memory[ALUOUT];   //coloca a informação da memória no registrador de dados de memória
            break;

        case (short int)0b1011110000011000: //se estiver no estado 5 (conclusão SW)
            memory[ALUOUT] = B;       //coloca na memoria o conteúdo de rt
            break;

        case (short int)0b1001010001000111: //se estiver no estado 7 (conclusão tipo R)
            reg[rd] = ALUOUT;               //coloca o resultado da operacao no registrador de destino rd
            break;

        default:
            break;
    }
    return;
}

void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
    if(sc == (short int)0b1001110000010111){ //se estiver no estado 4 (conclusão LW)
        int rt = ((IR & separa_rt) >> 16);
        reg[rt] = MDR;
    }
    return;
}


