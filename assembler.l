%option noyywrap

%{
#include "assembler.tab.h"
extern YYSTYPE yylval;
%}

%%
ADD                 return ADD;
SUB                 return SUB;
SLL                 return SLL;
SLT                 return SLT;
SLTU                return SLTU;
XOR                 return XOR;
SRL                 return SRL;
SRA                 return SRA;
OR                  return OR;
AND                 return AND;

ADDI                return ADDI;
SLLI                return SLLI;
LW                  return LW;
SLTI                return SLTI;
SLTIU               return SLTIU;
XORI                return XORI;
ORI                 return ORI;
ANDI                return ANDI;
SRLI                return SRLI;
SRAI                return SRAI;
JALR                return JALR;
LB                  return LB;
LH                  return LH;
LBU                 return LBU;
LHU                 return LHU;
RET                 return RET;
LI                  return LI;
MV                  return MV;
NOP                 return NOP;
SUBI                return SUBI;

SW                  return SW;
SH                  return SH;
SB                  return SB;

BEQ                 return BEQ;
BNE                 return BNE;
BLT                 return BLT;
BGE                 return BGE;
BLTU                return BLTU;
BGEU                return BGEU;

J                   return J;
JAL                 return JAL;

AUIPC               return AUIPC;
LUI                 return LUI;
ADDR                return ADDR;
0x[0-9a-fA-F]+      yylval.l = strtol(yytext, NULL, 16); return IMMEDIATE;
x[0-9]+             yylval.l = strtol(yytext + 1, NULL, 10); return REGISTER;
0b[0-1]+            yylval.l = strtol(yytext, NULL, 2); return IMMEDIATE;
0[0-7]+             yylval.l = strtol(yytext, NULL, 8); return IMMEDIATE;
[0-9]+              yylval.l = strtol(yytext, NULL, 10); return IMMEDIATE;
\r?\n               return NEWLINE;
\(                  return LEFT_PAREN;
\)                  return RIGHT_PAREN;
,                   return COMMA;
-                   return MINUS;
[ \t]+              ;
%%
