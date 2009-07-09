/*
 * =============================================================================
 *
 *      Arquivo: registradores.c
 *        Autores: Let�cia Lana Cherchiglia, Leonardo Vilela Teixeira, Lucas Alves Guerra Fran�a
 *        email: letslc@dcc.ufmg.br, vilela@dcc.ufmg.br, lguerra@dcc.ufmg.br
 *
 * =============================================================================
 */

#include "registradores.h"

/* ************************************************************************** */
/* ============================== REGISTRADOR =============================== */
/* ************************************************************************** */

void Reg_Escreve_Word( Registrador * R, Word W )
{
    int i;
    for (i = 0 ; i < BITS_ARQ ; i++)
        (*R)[i] = W[i];
}

/* ========================================================================== */

void Reg_Le_Word( Registrador R, Word Destino )
{
    int i;
    for (i = 0 ; i < BITS_ARQ ; i++)
        Destino[i] = R[i];

}

/* ************************************************************************** */
/* ========================= BANCO DE REGISTRADORES ========================= */
/* ************************************************************************** */

void B_Reg_Escreve_Word( Banco_de_Registradores * B, bool reg_num[3], Word D )
{
    /* Calcula Endereço */
    int End = 0;
    End += reg_num[0] ? 1 : 0;
    End += reg_num[1] ? 2 : 0;
    End += reg_num[2] ? 4 : 0;

    /* Copia Word */
    int i;
    for (i = 0 ; i < BITS_ARQ ; i++)
        B->Regs[End][i] = D[i];
}

/* ========================================================================== */

void B_Reg_Le_Word( Banco_de_Registradores B, bool reg_num[3], Word Destino )
{
    /* Calcula Endereço */
    int End = 0;
    End += reg_num[0] ? 1 : 0;
    End += reg_num[1] ? 2 : 0;
    End += reg_num[2] ? 4 : 0;

    /* Copia Word */
    int i;
    for (i = 0 ; i < BITS_ARQ ; i++)
    {
       Destino[i] = B.Regs[End][i];
    }

}

/* ************************************************************************** */

