%option yylineno

%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"
#include "pass.h"
#include "ir.h"

int yylex(void);
void yyerror(char* s);
extern sym_t *symtab;
extern int yylineno;
%}

%start program

%union {
  long l;
  char* s;
}

%token <s> IDENTIFIER
%token <l> REGISTER NEWLINE COMMA LEFT_PAREN RIGHT_PAREN MINUS IMMEDIATE
%token ADD SUB SLL SLT SLTU XOR SRL SRA OR AND NEG SNEZ
%token ADDI SLLI LW SLTI SLTIU XORI ORI ANDI SRLI SRAI JALR LB LH LBU LHU RET LI MV NOP SUBI JR SEQZ
%token SW SH SB
%token BEQ BNE BLT BGE BLTU BGEU BEQZ BNEZ
%token J JAL
%token AUIPC LUI LA
%token DOT_TEXT DOT_DATA ALIGN
%type <l> imm
%type <l> align_pow2

%%
program : segments
;
segments : /* empty */
| segments segment
;
segment : label_list opt_item NEWLINE
;
label_list : /* empty */
| label_list label
;
opt_item : /* empty */
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
;

instruction 
  : r-type
  | i-type
  | ui-type
  | s-type
  | b-type
  | j-type
;

r-type 
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
;

add: ADD REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_ADD, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sub: SUB REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SUB, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
neg: NEG REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SUB, IF_R, $2, 0, $4, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sll: SLL REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SLL, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
slt: SLT REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SLT, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sltu: SLTU REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SLTU, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
snez: SNEZ REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SLTU, IF_R, $2, 0, $4, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
xor: XOR REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_XOR, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
srl: SRL REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SRL, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sra: SRA REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SRA, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
or: OR REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_OR, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
and: AND REGISTER COMMA REGISTER COMMA REGISTER
{
  ir_append_instr(OP_AND, IF_R, $2, $4, $6, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;

i-type 
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
;

addi: ADDI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_ADDI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
nop: NOP
{
  ir_append_instr(OP_ADDI, IF_I, 0, 0, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
mv: MV REGISTER COMMA REGISTER
{
  ir_append_instr(OP_ADDI, IF_I, $2, $4, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
li: LI REGISTER COMMA imm
{
  ir_append_instr(OP_ADDI, IF_I, $2, 0, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
subi: SUBI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_ADDI, IF_I, $2, $4, -1, -($6), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
slli: SLLI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_SLLI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
slti: SLTI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_SLTI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sltiu: SLTIU REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_SLTIU, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
seqz: SEQZ REGISTER COMMA REGISTER
{
  ir_append_instr(OP_SLTIU, IF_I, $2, $4, -1, 1, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
xori: XORI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_XORI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
ori: ORI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_ORI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
andi: ANDI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_ANDI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
srli: SRLI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_SRLI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
srai: SRAI REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_SRAI, IF_I, $2, $4, -1, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
jalr: JALR REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_JALR, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
jr: JR REGISTER
{
  ir_append_instr(OP_JALR, IF_I, 0, $2, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
ret: RET
{
  ir_append_instr(OP_JALR, IF_I, 0, 1, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lw: LW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_LW, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lb: LB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_LB, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lh: LH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_LH, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lbu: LBU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_LBU, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lhu: LHU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_LHU, IF_I, $2, $6, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;

s-type 
  : sw
  | sb
  | sh
;

sw: SW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_SW, IF_S, -1, $6, $2, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sb: SB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_SB, IF_S, -1, $6, $2, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
sh: SH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  ir_append_instr(OP_SH, IF_S, -1, $6, $2, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;

b-type 
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
  ir_append_instr(OP_BEQ, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
beq: BEQ REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BEQ, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;
beqz: BEQZ REGISTER COMMA imm
{
  ir_append_instr(OP_BEQ, IF_B, -1, $2, 0, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
beqz: BEQZ REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BEQ, IF_B, -1, $2, 0, 0, 
                  $4, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($4);
}
;
bne: BNE REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_BNE, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
bne: BNE REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BNE, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;
bnez: BNEZ REGISTER COMMA imm
{
  ir_append_instr(OP_BNE, IF_B, -1, $2, 0, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
bnez: BNEZ REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BNE, IF_B, -1, $2, 0, 0, 
                  $4, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($4);
}
;
blt: BLT REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_BLT, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
blt: BLT REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BLT, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;
bge: BGE REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_BGE, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
bge: BGE REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BGE, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;
bltu: BLTU REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_BLTU, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
bltu: BLTU REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BLTU, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;
bgeu: BGEU REGISTER COMMA REGISTER COMMA imm
{
  ir_append_instr(OP_BGEU, IF_B, -1, $2, $4, $6, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
bgeu: BGEU REGISTER COMMA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_BGEU, IF_B, -1, $2, $4, 0, 
                  $6, RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($6);
}
;

j-type 
  : j
  | jal
;

j: J imm
{
  ir_append_instr(OP_JAL, IF_J, 0, -1, -1, $2, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
j: J IDENTIFIER
{
  ir_append_instr(OP_JAL, IF_J, 0, -1, -1, 0, 
                  $2, RELOC_JTGT, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($2);
}
;
jal: JAL REGISTER COMMA imm
{
  ir_append_instr(OP_JAL, IF_J, $2, -1, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
jal: JAL REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_JAL, IF_J, $2, -1, -1, 0, 
                  $4, RELOC_JTGT, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free($4);
}
;

ui-type 
  : auipc
  | lui
  | la
;

auipc: AUIPC REGISTER COMMA imm
{
  ir_append_instr(OP_AUIPC, IF_UI, $2, -1, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
lui: LUI REGISTER COMMA imm
{
  ir_append_instr(OP_LUI, IF_UI, $2, -1, -1, $4, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
;
la: LA REGISTER COMMA IDENTIFIER
{
  ir_append_instr(OP_AUIPC, IF_UI, $2, -1, -1, 0, 
                  $4, RELOC_HI20, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);

  ir_append_instr(OP_ADDI, IF_I, $2, $2, -1, 0, 
                  $4, RELOC_LO12, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);

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

void yyerror(char *msg){

}