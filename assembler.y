%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum instruction_format { IF_R, IF_I, IF_UI, IF_S, IF_B, IF_J };

int pc = 0;

typedef struct sym {
  char *name;
  int addr;
  struct sym *next;
} sym_t;

sym_t *sym_tab = NULL;

static struct instruction {
  enum instruction_format format;
  int funct3 : 3;
  int funct7 : 7;
  int imm : 20;
  int opcode : 7;
  int rd : 5;
  int rs1 : 5;
  int rs2 : 5;
} instruction;

static void printbin(int val, char bits);
static int bit_range(int val, char begin, char end);
static void print_instruction(struct instruction);
int yylex();
void yyerror(char* s);
void set_format_r();
void set_format_i();
void set_format_s();
void set_format_b();
void set_format_ui();
void set_format_j();
int label_high(char* label);
int label_low(char* label);
void insert_symbol(const char *name, int addr);
int find_symbol(const char *name);
%}

%start program
%union {
  long l;
  char *s;
}
%token <s> IDENTIFIER
%token <l> REGISTER NEWLINE COMMA LEFT_PAREN RIGHT_PAREN MINUS IMMEDIATE
%token ADD SUB SLL SLT SLTU XOR SRL SRA OR AND NEG SNEZ
%token ADDI SLLI LW SLTI SLTIU XORI ORI ANDI SRLI SRAI JALR LB LH LBU LHU RET LI MV NOP SUBI JR SEQZ
%token SW SH SB
%token BEQ BNE BLT BGE BLTU BGEU BEQZ BNEZ
%token J JAL
%token AUIPC LUI ADDR LA
%type <l> imm

%%
program : segments
;
segments : /* empty */
| segments segment
;
segment : label_list instruction NEWLINE
| instruction NEWLINE
| label_list NEWLINE
;
label_list : /* empty */
| label_list label
;
label : IDENTIFIER ':' {
  insert_symbol($1, pc);
  free($1);
}
;

instruction 
  : r-type { print_instruction(instruction); }
  | i-type { print_instruction(instruction); }
  | ui-type { print_instruction(instruction); }
  | s-type { print_instruction(instruction); }
  | b-type { print_instruction(instruction); }
  | j-type { print_instruction(instruction); }
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
  set_format_r();
  instruction.funct3 = 0;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
sub: SUB REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0;
  instruction.funct7 = 0b0100000;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
neg: NEG REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0;
  instruction.funct7 = 0b0100000;
  instruction.rd = $2;
  instruction.rs1 = 0;
  instruction.rs2 = $4;
}
;
sll: SLL REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b001;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
slt: SLT REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b010;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
sltu: SLTU REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b011;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
snez: SNEZ REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b011;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = 0;
  instruction.rs2 = $4;
}
;
xor: XOR REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b100;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
srl: SRL REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b101;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
sra: SRA REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b101;
  instruction.funct7 = 0b0100000;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
or: OR REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b110;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
}
;
and: AND REGISTER COMMA REGISTER COMMA REGISTER
{
  set_format_r();
  instruction.funct3 = 0b111;
  instruction.funct7 = 0;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.rs2 = $6;
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
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
nop: NOP
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = 0;
  instruction.rs1 = 0;
  instruction.imm = 0;
}
;
mv: MV REGISTER COMMA REGISTER
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = 0;
}
;
li: LI REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = 0;
  instruction.imm = $4;
}
;
subi: SUBI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = -($6);
}
;
slli: SLLI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b001;
  instruction.funct7 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
slti: SLTI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b010;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
sltiu: SLTIU REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b011;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
seqz: SEQZ REGISTER COMMA REGISTER
{
  set_format_i();
  instruction.funct3 = 0b011;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = 1;
}
;
xori: XORI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b100;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
ori: ORI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b110;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
andi: ANDI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b111;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
srli: SRLI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b101;
  instruction.funct7 = 0;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
srai: SRAI REGISTER COMMA REGISTER COMMA imm
{
  set_format_i();
  instruction.funct3 = 0b101;
  instruction.funct7 = 0b0100000;
  instruction.opcode = 0b0010011;
  instruction.rd = $2;
  instruction.rs1 = $4;
  instruction.imm = $6;
}
;
jalr: JALR REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b1100111;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
jr: JR REGISTER
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b1100111;
  instruction.rd = 0;
  instruction.rs1 = $2;
  instruction.imm = 0;
}
;
ret: RET
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b1100111;
  instruction.rd = 0;
  instruction.imm = 0;
  instruction.rs1 = 1;
}
;
lw: LW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0b010;
  instruction.opcode = 0b0000011;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
lb: LB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0;
  instruction.opcode = 0b0000011;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
lh: LH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0b001;
  instruction.opcode = 0b0000011;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
lbu: LBU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0b100;
  instruction.opcode = 0b0000011;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
lhu: LHU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_i();
  instruction.funct3 = 0b101;
  instruction.opcode = 0b0000011;
  instruction.rd = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;

s-type 
  : sw
  | sb
  | sh
;

sw: SW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_s();
  instruction.funct3 = 0b010;
  instruction.rs2 = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
sb: SB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_s();
  instruction.funct3 = 0;
  instruction.rs2 = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
}
;
sh: SH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN
{
  set_format_s();
  instruction.funct3 = 0b001;
  instruction.rs2 = $2;
  instruction.imm = $4;
  instruction.rs1 = $6;
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
  set_format_b();
  instruction.funct3 = 0;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;
beqz: BEQZ REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0;
  instruction.rs1 = $2;
  instruction.rs2 = 0;
  instruction.imm = $4;
}
;
bne: BNE REGISTER COMMA REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b001;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;
bnez: BNEZ REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b001;
  instruction.rs1 = $2;
  instruction.rs2 = 0;
  instruction.imm = $4;
}
;
blt: BLT REGISTER COMMA REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b100;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;
bge: BGE REGISTER COMMA REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b101;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;
bltu: BLTU REGISTER COMMA REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b110;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;
bgeu: BGEU REGISTER COMMA REGISTER COMMA imm
{
  set_format_b();
  instruction.funct3 = 0b111;
  instruction.rs1 = $2;
  instruction.rs2 = $4;
  instruction.imm = $6;
}
;

j-type 
  : j
  | jal
;

j: J imm
{
  set_format_j();
  instruction.rd = 0;
  instruction.imm = $2;
}
jal: JAL REGISTER COMMA imm
{
  set_format_j();
  instruction.rd = $2;
  instruction.imm = $4;
}
;

ui-type 
  : auipc
  | lui
  | la
;

auipc: AUIPC REGISTER COMMA imm
{
  set_format_ui();
  instruction.opcode = 0b0010111;
  instruction.rd = $2;
  instruction.imm = $4;
}
;
lui: LUI REGISTER COMMA imm
{
  set_format_ui();
  instruction.opcode = 0b0110111;
  instruction.rd = $2;
  instruction.imm = $4;
}
;
la: LA REGISTER COMMA IDENTIFIER
{
  char *lbl = $4;

  set_format_ui();
  instruction.opcode = 0b0010111;
  instruction.rd = $2;
  instruction.imm = label_high(lbl);
  print_instruction(instruction);

  set_format_i();
  instruction.opcode = 0b0010011;
  instruction.funct3 = 0;
  instruction.rd = $2;
  instruction.rs1 = $2;
  instruction.imm = label_low(lbl);
  print_instruction(instruction);

  free(lbl);
}
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
static void print_instruction(struct instruction instruction) {
  switch (instruction.format) {
    case IF_R:
      printbin(instruction.funct7, 7);
      printbin(instruction.rs2, 5);
      printbin(instruction.rs1, 5);
      printbin(instruction.funct3, 3);
      printbin(instruction.rd, 5);
      printbin(instruction.opcode, 7);
      break;
    case IF_I:
      printbin(instruction.imm, 12);
      printbin(instruction.rs1, 5);
      printbin(instruction.funct3, 3);
      printbin(instruction.rd, 5);
      printbin(instruction.opcode, 7);
      break;
    case IF_UI:
      printbin(instruction.imm, 20);
      printbin(instruction.rd, 5);
      printbin(instruction.opcode, 7);
      break;
    case IF_S:
      printbin(bit_range(instruction.imm, 5, 12), 7);
      printbin(instruction.rs2, 5);
      printbin(instruction.rs1, 5);
      printbin(instruction.funct3, 3);
      printbin(bit_range(instruction.imm, 0, 5), 5);
      printbin(instruction.opcode, 7);
      break;
    case IF_B:
      printbin(bit_range(instruction.imm, 12, 13), 1);
      printbin(bit_range(instruction.imm, 5, 11), 6);
      printbin(instruction.rs2, 5);
      printbin(instruction.rs1, 5);
      printbin(instruction.funct3, 3);
      printbin(bit_range(instruction.imm, 1, 5), 4);
      printbin(bit_range(instruction.imm, 11, 12), 1);
      printbin(instruction.opcode, 7);
      break;
    case IF_J:
      printbin(bit_range(instruction.imm, 20, 21), 1);
      printbin(bit_range(instruction.imm, 1, 11), 10);
      printbin(bit_range(instruction.imm, 11, 12), 1);
      printbin(bit_range(instruction.imm, 12, 20), 8);
      printbin(instruction.rd, 5);
      printbin(instruction.opcode, 7);
      break;
    default:
      exit(-1);
  }
  printf("\n");
  pc += 4;
}
static void printbin(int val, char bits) {
  for (char i = bits - 1; i >= 0; i--) {
    if (val & (1 << i)) {
      putchar('1');
    } else {
      putchar('0');
    }
  }
}

static int bit_range(int val, char begin, char end) {
  int mask = ((1 << end) - 1) ^ ((1 << begin) - 1);
  return (val & mask) >> begin;
}

void yyerror(char *msg){
    fprintf(stderr, "Parse error: %s\n", msg);
    exit(1);
}

void set_format_r(){ instruction.format = IF_R; instruction.opcode = 0b0110011; }
void set_format_i(){ instruction.format = IF_I; }
void set_format_s(){ instruction.format = IF_S; instruction.opcode = 0b0100011; }
void set_format_b(){ instruction.format = IF_B; instruction.opcode = 0b1100011; }
void set_format_ui(){ instruction.format = IF_UI; }
void set_format_j(){ instruction.format = IF_J; instruction.opcode = 0b1101111; }

int label_high(char* label){
  int target = find_symbol(label);
  int offset = target - pc;
  return (offset + 0x800) >> 12;
}

int label_low(char* label){
  int target = find_symbol(label);
  int offset = target - pc;
  return offset & 0xFFF;
}

void insert_symbol(const char *name, int addr){
  for (sym_t *p = sym_tab; p; p = p->next){
    if (strcmp(p->name, name) == 0){
      fprintf(stderr, "Error: duplicate label %s at 0x%08x\n", name, addr);
      exit(1);
    }
  }

  sym_t *n = malloc(sizeof *n);
  n -> name = strdup(name);
  n -> addr = addr;
  n -> next = sym_tab;
  sym_tab = n;
}

int find_symbol(const char *name){
  for (sym_t *p = sym_tab; p; p = p->next){
    if (strcmp(p->name, name) == 0){
      return p->addr;
    }
  }

  fprintf(stderr, "Error: undefined label '%s'\n", name);
  exit(1);
}

int main(){
 #ifdef YYDEBUG
 int yydebug = 1;
 #endif /* YYDEBUG */
 yyparse();
 return 0;
}
