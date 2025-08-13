%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "symtab.h"
#include "pass.h"
#include "ir.h"
#include "pass1.h"
#include "pass2.h"

int yylex(void);
void yyerror(const char* s);
extern int yylineno;
%}

%start program

%union {
  long l;
  long long ll;
  char* s;
}

%token <s> IDENTIFIER STRING_LITERAL
%token <ll> REGISTER IMMEDIATE
%token <l> NEWLINE COMMA LEFT_PAREN RIGHT_PAREN MINUS
%token ADD SUB SLL SLT SLTU XOR SRL SRA OR AND NEG SNEZ MUL MULH MULHSU MULHU DIV DIVU REM REMU ADDW SUBW SLLW SRLW 
%token SRAW MULW DIVW DIVUW REMW REMUW
%token ADDI SLLI LW SLTI SLTIU XORI ORI ANDI SRLI SRAI JALR LB LH LBU LHU RET LI MV NOP SUBI JR SEQZ ECALL EBREAK
%token ADDIW SLLIW SRLIW SRAIW LWU LD
%token SW SH SB SD
%token BEQ BNE BLT BGE BLTU BGEU BEQZ BNEZ
%token J JAL
%token AUIPC LUI LA
%token DOT_TEXT DOT_DATA ALIGN
%token WORD BYTE HALF DWORD ASCII ASCIIZ SPACE
%type <ll> imm 
%type <l> align_pow2

%%
program : segments
;
segments : %empty
| segments segment
;
segment : label_list opt_item NEWLINE
;
label_list : %empty
| label_list label
;
opt_item : %empty
| instruction
| directive
;
label : IDENTIFIER ':' {
  if (!insert_sym(symtab, $1, (int)pass_current_pc())) {
    /* Duplicate label error reported by insert_sym */
  }
  free($1);
}
;
directive : DOT_TEXT { pass_set_section(SEC_TEXT);}
| DOT_DATA { pass_set_section(SEC_DATA);}
| ALIGN align_pow2 {
  uint32_t addr_before = pass_current_pc();
  uint32_t pad = pass_align_current_pc((uint32_t)$2);
  if (pad > 0) {
    ir_append_align(cur_section, addr_before, pad, yylineno);
  }
}
| BYTE imm { 
  if ($2 < INT8_MIN || $2 > INT8_MAX) { 
    char buf[100];
    snprintf(buf, sizeof(buf), "Value %lld out of range for .byte", $2);
    yyerror(buf);
  }
  else { pass1_emit_data($2, 1, yylineno);  }
}
| HALF imm { 
  if ($2 < INT16_MIN || $2 > INT16_MAX) { 
    char buf[100];
    snprintf(buf, sizeof(buf), "Value %lld out of range for .half", $2);
    yyerror(buf);
  }
  else { pass1_emit_data($2, 2, yylineno);  }
}
| WORD imm { 
  if ($2 < INT32_MIN || $2 > INT32_MAX) { 
    char buf[100];
    snprintf(buf, sizeof(buf), "Value %lld out of range for .word", $2);
    yyerror(buf);
  }
  else { pass1_emit_data($2, 4, yylineno);  }
}
| DWORD imm { 
  pass1_emit_data($2, 8, yylineno);
}
| ASCII STRING_LITERAL {
  const char* s = $2;
  while (*s) {
    pass1_emit_data((uint8_t)*s, 1, yylineno);
    s++;
  }
  free($2);
}
| ASCIIZ STRING_LITERAL {
  const char* s = $2;
  while (*s) {
    pass1_emit_data((uint8_t)*s, 1, yylineno);
    s++;
  }
  pass1_emit_data(0, 1, yylineno);
  free($2);
}
| SPACE imm {
  if ($2 < 0) {
    char buf[100];
    snprintf(buf, sizeof(buf), "Negative size for .space: %lld", $2);
    yyerror(buf);
  }
  else {
    pass1_emit_space($2, yylineno);
  }
}
;

instruction 
  : r_type
  | i_type
  | ui_type
  | s_type
  | b_type
  | j_type
;

r_type 
  : add
  | sub
  | sll
  | slt
  | sltu
  | xor
  | srl
  | sra
  | or
  | and
  | neg
  | snez
  | mul
  | mulh
  | mulhsu
  | mulhu
  | div
  | divu
  | rem
  | remu
  | mulw
  | divw
  | divuw
  | remw
  | remuw
;

add: ADD REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_ADD, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
sub: SUB REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SUB, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
neg: NEG REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SUB, IF_R, $2, 0, $4, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
sll: SLL REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLL, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
slt: SLT REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLT, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
sltu: SLTU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLTU, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
snez: SNEZ REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLTU, IF_R, $2, 0, $4, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
xor: XOR REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_XOR, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
srl: SRL REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SRL, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
sra: SRA REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SRA, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
or: OR REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_OR, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
and: AND REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_AND, IF_R, $2, $4, $6, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
mul: MUL REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_MUL, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
mulh: MULH REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_MULH, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
mulhsu: MULHSU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_MULHSU, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
mulhu: MULHU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_MULHU, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
div: DIV REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_DIV, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
divu: DIVU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_DIVU, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
rem: REM REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_REM, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
remu: REMU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_REMU, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
addw: ADDW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_ADDW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
subw: SUBW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SUBW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
sllw: SLLW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLLW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
srlw: SRLW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SRLW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
sraw: SRAW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SRAW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
remu: REMU REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_REMU, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
mulw: MULW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_MULW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
divw: DIVW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_DIVW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
divuw: DIVUW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_DIVUW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
remw: REMW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_REMW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;
remuw: REMUW REGISTER COMMA REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_REMUW, IF_R, $2, $4, $6, 0, 
                         NULL, RELOC_NONE, yylineno);
}
;

i_type 
  : addi
  | slli
  | lw
  | slti
  | sltiu
  | xori
  | ori
  | andi
  | srli
  | srai
  | jalr
  | lb
  | lh
  | lbu
  | lhu
  | subi
  | ret
  | nop
  | mv
  | li
  | jr
  | seqz
  | ebreak
  | ecall
  | addiw
  | srliw
  | sraiw
  | slliw
  | lwu
  | ld
;

addi: ADDI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ADDI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
nop: NOP
{
  pass1_emit_instruction(OP_ADDI, IF_I, 0, 0, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
mv: MV REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_ADDI, IF_I, $2, $4, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
li: LI REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ADDI, IF_I, $2, 0, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
subi: SUBI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ADDI, IF_I, $2, $4, 0, -($6),
                         NULL, RELOC_NONE, yylineno);
}
;
slli: SLLI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SLLI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
slti: SLTI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SLTI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
sltiu: SLTIU REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SLTIU, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
seqz: SEQZ REGISTER COMMA REGISTER
{
  pass1_emit_instruction(OP_SLTIU, IF_I, $2, $4, 0, 1,
                         NULL, RELOC_NONE, yylineno);
}
;
xori: XORI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_XORI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
ori: ORI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ORI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
andi: ANDI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ANDI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
srli: SRLI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SRLI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
srai: SRAI REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SRAI, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
jalr: JALR REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_JALR, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
jr: JR REGISTER
{
  pass1_emit_instruction(OP_JALR, IF_I, 0, $2, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
ret: RET
{
  pass1_emit_instruction(OP_JALR, IF_I, 0, 1, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
lw: LW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LW, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
lb: LB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LB, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
lh: LH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LH, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
lbu: LBU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LBU, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
lhu: LHU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LHU, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
ebreak: EBREAK
{
  pass1_emit_instruction(OP_EBREAK, IF_I, 0, 0, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
ecall: ECALL
{
  pass1_emit_instruction(OP_ECALL, IF_I, 0, 0, 0, 0,
                         NULL, RELOC_NONE, yylineno);
}
;
addiw: ADDIW REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_ADDIW, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
slliw: SLLIW REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SLLIW, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
srliw: SRLIW REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SRLIW, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
sraiw: SRAIW REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_SRAIW, IF_I, $2, $4, 0, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
lwu: LWU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LWU, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
ld: LD REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_LD, IF_I, $2, $6, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;

s_type 
  : sw
  | sb
  | sh
  | sd
;

sw: SW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_SW, IF_S, 0, $6, $2, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
sb: SB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_SB, IF_S, 0, $6, $2, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
sh: SH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_SH, IF_S, 0, $6, $2, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
sd: SD REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  pass1_emit_instruction(OP_SD, IF_S, 0, $6, $2, $4,
                         NULL, RELOC_NONE, yylineno);
}
;

b_type 
  : beq
  | bne
  | blt
  | bge
  | bltu
  | bgeu
  | beqz
  | bnez
;

beq: BEQ REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BEQ, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
beq: BEQ REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BEQ, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;
beqz: BEQZ REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BEQ, IF_B, 0, $2, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
beqz: BEQZ REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BEQ, IF_B, 0, $2, 0, 0,
                         $4, RELOC_BOFF, yylineno);
  free($4);
}
;
bne: BNE REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BNE, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
bne: BNE REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BNE, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;
bnez: BNEZ REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BNE, IF_B, 0, $2, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
bnez: BNEZ REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BEQ, IF_B, 0, $2, 0, 0,
                         $4, RELOC_BOFF, yylineno);
  free($4);
}
;
blt: BLT REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BLT, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
blt: BLT REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BLT, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;
bge: BGE REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BGE, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
bge: BGE REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BGE, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;
bltu: BLTU REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BLTU, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
bltu: BLTU REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BLTU, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;
bgeu: BGEU REGISTER COMMA REGISTER COMMA imm
{
  pass1_emit_instruction(OP_BGEU, IF_B, 0, $2, $4, $6,
                         NULL, RELOC_NONE, yylineno);
}
;
bgeu: BGEU REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_BGEU, IF_B, 0, $2, $4, 0,
                         $6, RELOC_BOFF, yylineno);
  free($6);
}
;

j_type 
  : j
  | jal
;

j: J imm
{
  pass1_emit_instruction(OP_JAL, IF_J, 0, 0, 0, $2,
                         NULL, RELOC_NONE, yylineno);
}
;
j: J IDENTIFIER
{
  pass1_emit_instruction(OP_JAL, IF_J, 0, 0, 0, 0,
                         $2, RELOC_JTGT, yylineno);
  free($2);
}
;
jal: JAL REGISTER COMMA imm
{
  pass1_emit_instruction(OP_JAL, IF_J, $2, 0, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
jal: JAL REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_JAL, IF_J, $2, 0, 0, 0,
                         $4, RELOC_JTGT, yylineno);
  free($4);
}
;

ui_type 
  : auipc
  | lui
  | la
;

auipc: AUIPC REGISTER COMMA imm
{
  pass1_emit_instruction(OP_AUIPC, IF_UI, $2, 0, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
lui: LUI REGISTER COMMA imm
{
  pass1_emit_instruction(OP_LUI, IF_UI, $2, 0, 0, $4,
                         NULL, RELOC_NONE, yylineno);
}
;
la: LA REGISTER COMMA IDENTIFIER
{
  pass1_emit_instruction(OP_AUIPC, IF_UI, $2, 0, 0, 0,
                         $4, RELOC_HI20, yylineno);
  
  pass1_emit_instruction(OP_ADDI, IF_I, $2, $2, 0, 0,
                         $4, RELOC_LO12, yylineno);

  free($4);
}
;

align_pow2 : imm { $$ = $1; }
;

imm : MINUS IMMEDIATE
{
  $$ = -1 * $2;
}
| IMMEDIATE
{
  $$ = $1;
}
;
%%

void yyerror(const char *msg){
  fprintf(stderr, "Parse error (line: %d): %s\n", yylineno, msg);
  pass2_report_error();
}