%option noyywrap

%{
#include "assembler.tab.h"
#include <string.h>
extern YYSTYPE yylval;
%}

%%
[ \t]+              ;
"#".*               ;
\r?\n               return NEWLINE;

add                 return ADD;
sub                 return SUB;
sll                 return SLL;
slt                 return SLT;
sltu                return SLTU;
xor                 return XOR;
srl                 return SRL;
sra                 return SRA;
or                  return OR;
and                 return AND;
neg                 return NEG;
snez                return SNEZ;

addi                return ADDI;
slli                return SLLI;
lw                  return LW;
slti                return SLTI;
sltiu               return SLTIU;
xori                return XORI;
ori                 return ORI;
andi                return ANDI;
srli                return SRLI;
srai                return SRAI;
jalr                return JALR;
lb                  return LB;
lh                  return LH;
lbu                 return LBU;
lhu                 return LHU;
ret                 return RET;
li                  return LI;
mv                  return MV;
nop                 return NOP;
subi                return SUBI;
jr                  return JR;
seqz                return SEQZ;

sw                  return SW;
sh                  return SH;
sb                  return SB;

beq                 return BEQ;
bne                 return BNE;
blt                 return BLT;
bge                 return BGE;
bltu                return BLTU;
bgeu                return BGEU;
beqz                return BEQZ;
bnez                return BNEZ;

j                   return J;
jal                 return JAL;

auipc               return AUIPC;
lui                 return LUI;
addr                return ADDR;
la                  return LA;

0x[0-9a-fA-F]+      yylval.l = strtol(yytext, NULL, 16); return IMMEDIATE;
x([0-2]?[0-9]|3[0-1])   yylval.l = strtol(yytext + 1, NULL, 10); return REGISTER;
0b[0-1]+            yylval.l = strtol(yytext + 2, NULL, 2); return IMMEDIATE;
0[0-7]+             yylval.l = strtol(yytext, NULL, 8); return IMMEDIATE;
[0-9]+              yylval.l = strtol(yytext, NULL, 10); return IMMEDIATE;
[a-zA-Z_][a-zA-Z0-9_]*  yylval.s = strdup(yytext); return IDENTIFIER;
:                   return ':';
\(                  return LEFT_PAREN;
\)                  return RIGHT_PAREN;
,                   return COMMA;
-                   return MINUS;
%%
