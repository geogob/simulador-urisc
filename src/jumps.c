/*
 * =======================================================================================
 *
 *      Arquivo: jumps.c
 *        Autores: Let�cia Lana Cherchiglia, Leonardo Vilela Teixeira, Lucas Alves Guerra Fran�a
 *        email: letslc@dcc.ufmg.br, vilela@dcc.ufmg.br, lguerra@dcc.ufmg.br
 *
 * =======================================================================================
 */

#include "jumps.h"
#include "registradores.h"
#include "alu.h"

/* ************************************************************************************ */
/* ============================== UNIDADE DE DESVIOS (JUMPS) ========================== */
/* ************************************************************************************ */

/* ==================================================================================== */

//Extende o sinal do campo offset presente nas instru��es jump condicional e incondicional e coloca
//o offset extendido na word 'ALU_B'. Se a instru��o n�o possui offset(jump and link e jump register),
//a word 'ALU_B' � zerada.
void Jump_Extende_Sinal_Offset_Desvio( Word *ALU_B, Registrador IR )
{
    int i;
    //Se o jump � condicional, de formato IV.
    if( (IR)[2] == 0 ) {

        //Transfere o offset presente no registrador de instru��o para a word "ALU_B".
        for(i = BITS_ARQ / 2; i < BITS_ARQ; i++)
            (*ALU_B)[i] = (IR)[i];

        //Extende o sinal do offset.
        for(i = 0; i < BITS_ARQ /2; i ++)
            (*ALU_B)[i] = (*ALU_B)[BITS_ARQ / 2];
    }
    else {

        //Se o jump � incondicional, de formato V.
        if( (IR)[3] == 0 ) {

            //Transfere o offset presente no registrador de instru��o para a word "ALU_B".     
            for(i = BITS_ARQ - 12; i < BITS_ARQ; i++)
                (*ALU_B)[i] = (IR)[i];

            //Extende o sinal do offset.
            for(i = 0; i < BITS_ARQ - 12; i++)
                (*ALU_B)[i] = (*ALU_B)[BITS_ARQ - 12];
        }
        
        //Se o jump � 'jump and link' ou 'jump register'(ambos de formato VI), a word 'ALU_B' � definida como sendo zero.
        else {
            for(i = 0; i < BITS_ARQ; i++)
              (*ALU_B)[i] = 0;
        }
    }
}

/* ============================== FORMATO V =========================================== */

//Coloca o endere�o de destino do desvio incondicional(presente no registrador 'B') na word 'end_destino'.
void Jump_Incondicional( Word *end_destino, Registrador B )
{
    int i;
    for (i = 0 ; i < BITS_ARQ; i++)
        (*end_destino)[i] = (B)[i];
}

/* ============================== FORMATO IV ========================================== */

void Jump_Condicional_True( Word *end_destino, Registrador SaidaALU, Flags_ALU *flags, Registrador *PC, Registrador IR )
{
    int i;
    bool condicao_satisfeita = false;
    // Checa qual � a condi��o da instru��o de desvio e se ela � verdadeira.
    //Checa se a condi��o � o resultado da ALU ser diferente de zero (representada pela flag 'f_true').
    if( ((IR)[5] == 0) && ((IR)[6] == 0) ) {
        if(flags->f_true == true)
            condicao_satisfeita = true;
    }
    else {
        //Checa se a condi��o � o resultado da ALU ter sofrido overflow (representada pela flag 'overflow').
        if((IR)[5] == 0) {
            if(flags->overflow == true)
                condicao_satisfeita = true;              
        }
        else {
            //Checa se a condi��o � o resultado da ALU ser negativo (representada pela flag 'neg').
            if( ((IR)[6] == 0) && ((IR)[7] == 0) ) {
                if(flags->neg == true)
                    condicao_satisfeita = true;
            }
            else {
                //Checa se a condi��o � o resultado da ALU ser igual a zero (representada pela flag 'zero').
                if((IR)[6] == 0) {
                    if(flags->zero == true)
                        condicao_satisfeita = true;
                }
                else {
                    //Checa se a condi��o � o resultado da ALU ter tido carry (representada pela flag 'carry').
                    if((IR)[7] == 0) {
                        if(flags->carry == true)
                            condicao_satisfeita = true;
                    }
                    else {
                        //Checa se a condi��o � o resultado da ALU ser negativo ou igual a zero (representada pela flag 'negzero').
                        if(flags->negzero == true)
                            condicao_satisfeita = true;
                    }
                }
            }
        }
    }
    //Se a condi��o de desvio for verdadeira, coloca o endere�o presente no registrador 'SaidaALU' na word 'end_destino'.
    if(condicao_satisfeita == true)
        for(i = 0; i < BITS_ARQ; i++)
            (*end_destino)[i] = (SaidaALU)[i];
    //Se a condi��o de desvio for falsa, coloca o endere�o presente no registrador 'PC' na word 'end_destino'.
    else
        for(i = 0; i < BITS_ARQ; i++)
            (*end_destino)[i] = (*PC)[i];
}

/* ====================================================================================== */

void Jump_Condicional_False( Word *end_destino, Registrador SaidaALU, Flags_ALU *flags, Registrador *PC, Registrador IR )
{
    int i;
    bool condicao_satisfeita = false;
    // Checa qual � a condi��o da instru��o de desvio e se ela � verdadeira.
    //Checa se a condi��o � o resultado da ALU n�o ser diferente de zero (representada pela flag 'f_true').
    if( ((IR)[5] == 0) && ((IR)[6] == 0) ) {
        if(flags->f_true == false)
            condicao_satisfeita = true;
    }
    else {
        //Checa se a condi��o � o resultado da ALU n�o ter sofrido overflow (representada pela flag 'overflow').
        if((IR)[5] == 0) {
            if(flags->overflow == false)
                condicao_satisfeita = true;              
        }
        else {
            //Checa se a condi��o � o resultado da ALU n�o ser negativo (representada pela flag 'neg').
            if( ((IR)[6] == 0) && ((IR)[7] == 0) ) {
                if(flags->neg == false)
                    condicao_satisfeita = true;
            }
            else {
                //Checa se a condi��o � o resultado da ALU n�o ser igual a zero (representada pela flag 'zero').
                if((IR)[6] == 0) {
                    if(flags->zero == false)
                        condicao_satisfeita = true;
                }
                else {
                    //Checa se a condi��o � o resultado da ALU n�o ter tido carry (representada pela flag 'carry').
                    if((IR)[7] == 0) {
                        if(flags->carry == false)
                            condicao_satisfeita = true;
                    }
                    else {
                        //Checa se a condi��o � o resultado da ALU n�o ser negativo ou igual a zero (representada pela flag 'negzero').
                        if(flags->negzero == false)
                            condicao_satisfeita = true;
                    }
                }
            }
        }
    }
    //Se a condi��o de desvio for satisfeita, coloca o endere�o presente no registrador 'SaidaALU' na word 'end_destino'.
    if(condicao_satisfeita == true)
        for(i = 0; i < BITS_ARQ; i++)
            (*end_destino)[i] = (SaidaALU)[i];
    //Se a condi��o de desvio n�o for satisfeita, coloca o endere�o presente no registrador 'PC' na word 'end_destino'.
    else
        for(i = 0; i < BITS_ARQ; i++)
            (*end_destino)[i] = (*PC)[i];
}

/* ============================== FORMATO VI ========================================== */

void Jump_and_Link( Word *end_destino, Registrador B, Registrador *PC, Banco_de_Registradores *Breg )
{
    int i;
    //Coloca o endere�o de destino do desvio (presente no registrador 'B') na word 'end_destino'.
    for (i = 0 ; i < BITS_ARQ; i++)
        (*end_destino)[i] = (B)[i];

    Word posicao_atual;
    for (i = 0 ; i < BITS_ARQ; i++)
        posicao_atual[i] = (*PC)[i];
    
    bool reg_num[3];
    for(i = 0; i < 3; i++)
        reg_num[i] = 1;

    //Escreve o endere�o presente no registrador 'PC' no registrador 7 do banco de registradores.
    B_Reg_Escreve_Word( Breg, reg_num, posicao_atual);
}

/* ====================================================================================== */

void Jump_Register( Word *end_destino, Registrador B )
{
    int i;
    for (i = 0 ; i < BITS_ARQ; i++)
        (*end_destino)[i] = (B)[i];
}

/* ====================================================================================== */

void Jump_Calcula_Endereco_Desvio ( Word *end_destino, Registrador B, Registrador SaidaALU, Registrador *PC, Registrador IR, Banco_de_Registradores *Breg, Flags_ALU *flags )
{
    //Se o jump � condicional.
    if( (IR)[2] == 0 ) {
        //Se o jump � condicional false.
        if( (IR)[3] == 0 )
            Jump_Condicional_False(end_destino, SaidaALU, flags, PC, IR);
        //Se o jump � condicional true.
        else
            Jump_Condicional_True(end_destino, SaidaALU, flags, PC, IR);
    }
    else {
        //Se o jump � incondicional.
        if( (IR)[3] == 0 )
            Jump_Incondicional(end_destino, B);
        else {
            //Se o jump � 'jump and link'.
            if( (IR)[4] == 0 )
                Jump_and_Link(end_destino, B, PC, Breg);
            //Se o jump � 'jump register'.
            else
                Jump_Register(end_destino, B);
        }
    }
}
    
/* ************************************************************************************ */
