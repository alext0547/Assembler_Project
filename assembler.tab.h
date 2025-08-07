/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_ASSEMBLER_TAB_H_INCLUDED
# define YY_YY_ASSEMBLER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    REGISTER = 259,                /* REGISTER  */
    NEWLINE = 260,                 /* NEWLINE  */
    COMMA = 261,                   /* COMMA  */
    LEFT_PAREN = 262,              /* LEFT_PAREN  */
    RIGHT_PAREN = 263,             /* RIGHT_PAREN  */
    MINUS = 264,                   /* MINUS  */
    IMMEDIATE = 265,               /* IMMEDIATE  */
    ADD = 266,                     /* ADD  */
    SUB = 267,                     /* SUB  */
    SLL = 268,                     /* SLL  */
    SLT = 269,                     /* SLT  */
    SLTU = 270,                    /* SLTU  */
    XOR = 271,                     /* XOR  */
    SRL = 272,                     /* SRL  */
    SRA = 273,                     /* SRA  */
    OR = 274,                      /* OR  */
    AND = 275,                     /* AND  */
    NEG = 276,                     /* NEG  */
    SNEZ = 277,                    /* SNEZ  */
    ADDI = 278,                    /* ADDI  */
    SLLI = 279,                    /* SLLI  */
    LW = 280,                      /* LW  */
    SLTI = 281,                    /* SLTI  */
    SLTIU = 282,                   /* SLTIU  */
    XORI = 283,                    /* XORI  */
    ORI = 284,                     /* ORI  */
    ANDI = 285,                    /* ANDI  */
    SRLI = 286,                    /* SRLI  */
    SRAI = 287,                    /* SRAI  */
    JALR = 288,                    /* JALR  */
    LB = 289,                      /* LB  */
    LH = 290,                      /* LH  */
    LBU = 291,                     /* LBU  */
    LHU = 292,                     /* LHU  */
    RET = 293,                     /* RET  */
    LI = 294,                      /* LI  */
    MV = 295,                      /* MV  */
    NOP = 296,                     /* NOP  */
    SUBI = 297,                    /* SUBI  */
    JR = 298,                      /* JR  */
    SEQZ = 299,                    /* SEQZ  */
    SW = 300,                      /* SW  */
    SH = 301,                      /* SH  */
    SB = 302,                      /* SB  */
    BEQ = 303,                     /* BEQ  */
    BNE = 304,                     /* BNE  */
    BLT = 305,                     /* BLT  */
    BGE = 306,                     /* BGE  */
    BLTU = 307,                    /* BLTU  */
    BGEU = 308,                    /* BGEU  */
    BEQZ = 309,                    /* BEQZ  */
    BNEZ = 310,                    /* BNEZ  */
    J = 311,                       /* J  */
    JAL = 312,                     /* JAL  */
    AUIPC = 313,                   /* AUIPC  */
    LUI = 314,                     /* LUI  */
    LA = 315,                      /* LA  */
    DOT_TEXT = 316,                /* DOT_TEXT  */
    DOT_DATA = 317,                /* DOT_DATA  */
    ALIGN = 318                    /* ALIGN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "assembler.y"

  long l;
  char* s;

#line 132 "assembler.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_ASSEMBLER_TAB_H_INCLUDED  */
