/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "assembler.y"

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

#line 85 "assembler.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "assembler.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_REGISTER = 4,                   /* REGISTER  */
  YYSYMBOL_NEWLINE = 5,                    /* NEWLINE  */
  YYSYMBOL_COMMA = 6,                      /* COMMA  */
  YYSYMBOL_LEFT_PAREN = 7,                 /* LEFT_PAREN  */
  YYSYMBOL_RIGHT_PAREN = 8,                /* RIGHT_PAREN  */
  YYSYMBOL_MINUS = 9,                      /* MINUS  */
  YYSYMBOL_IMMEDIATE = 10,                 /* IMMEDIATE  */
  YYSYMBOL_ADD = 11,                       /* ADD  */
  YYSYMBOL_SUB = 12,                       /* SUB  */
  YYSYMBOL_SLL = 13,                       /* SLL  */
  YYSYMBOL_SLT = 14,                       /* SLT  */
  YYSYMBOL_SLTU = 15,                      /* SLTU  */
  YYSYMBOL_XOR = 16,                       /* XOR  */
  YYSYMBOL_SRL = 17,                       /* SRL  */
  YYSYMBOL_SRA = 18,                       /* SRA  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_NEG = 21,                       /* NEG  */
  YYSYMBOL_SNEZ = 22,                      /* SNEZ  */
  YYSYMBOL_ADDI = 23,                      /* ADDI  */
  YYSYMBOL_SLLI = 24,                      /* SLLI  */
  YYSYMBOL_LW = 25,                        /* LW  */
  YYSYMBOL_SLTI = 26,                      /* SLTI  */
  YYSYMBOL_SLTIU = 27,                     /* SLTIU  */
  YYSYMBOL_XORI = 28,                      /* XORI  */
  YYSYMBOL_ORI = 29,                       /* ORI  */
  YYSYMBOL_ANDI = 30,                      /* ANDI  */
  YYSYMBOL_SRLI = 31,                      /* SRLI  */
  YYSYMBOL_SRAI = 32,                      /* SRAI  */
  YYSYMBOL_JALR = 33,                      /* JALR  */
  YYSYMBOL_LB = 34,                        /* LB  */
  YYSYMBOL_LH = 35,                        /* LH  */
  YYSYMBOL_LBU = 36,                       /* LBU  */
  YYSYMBOL_LHU = 37,                       /* LHU  */
  YYSYMBOL_RET = 38,                       /* RET  */
  YYSYMBOL_LI = 39,                        /* LI  */
  YYSYMBOL_MV = 40,                        /* MV  */
  YYSYMBOL_NOP = 41,                       /* NOP  */
  YYSYMBOL_SUBI = 42,                      /* SUBI  */
  YYSYMBOL_JR = 43,                        /* JR  */
  YYSYMBOL_SEQZ = 44,                      /* SEQZ  */
  YYSYMBOL_SW = 45,                        /* SW  */
  YYSYMBOL_SH = 46,                        /* SH  */
  YYSYMBOL_SB = 47,                        /* SB  */
  YYSYMBOL_BEQ = 48,                       /* BEQ  */
  YYSYMBOL_BNE = 49,                       /* BNE  */
  YYSYMBOL_BLT = 50,                       /* BLT  */
  YYSYMBOL_BGE = 51,                       /* BGE  */
  YYSYMBOL_BLTU = 52,                      /* BLTU  */
  YYSYMBOL_BGEU = 53,                      /* BGEU  */
  YYSYMBOL_BEQZ = 54,                      /* BEQZ  */
  YYSYMBOL_BNEZ = 55,                      /* BNEZ  */
  YYSYMBOL_J = 56,                         /* J  */
  YYSYMBOL_JAL = 57,                       /* JAL  */
  YYSYMBOL_AUIPC = 58,                     /* AUIPC  */
  YYSYMBOL_LUI = 59,                       /* LUI  */
  YYSYMBOL_LA = 60,                        /* LA  */
  YYSYMBOL_DOT_TEXT = 61,                  /* DOT_TEXT  */
  YYSYMBOL_DOT_DATA = 62,                  /* DOT_DATA  */
  YYSYMBOL_ALIGN = 63,                     /* ALIGN  */
  YYSYMBOL_64_ = 64,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_program = 66,                   /* program  */
  YYSYMBOL_segments = 67,                  /* segments  */
  YYSYMBOL_segment = 68,                   /* segment  */
  YYSYMBOL_label_list = 69,                /* label_list  */
  YYSYMBOL_opt_item = 70,                  /* opt_item  */
  YYSYMBOL_label = 71,                     /* label  */
  YYSYMBOL_directive = 72,                 /* directive  */
  YYSYMBOL_instruction = 73,               /* instruction  */
  YYSYMBOL_r_type = 74,                    /* r_type  */
  YYSYMBOL_add = 75,                       /* add  */
  YYSYMBOL_sub = 76,                       /* sub  */
  YYSYMBOL_neg = 77,                       /* neg  */
  YYSYMBOL_sll = 78,                       /* sll  */
  YYSYMBOL_slt = 79,                       /* slt  */
  YYSYMBOL_sltu = 80,                      /* sltu  */
  YYSYMBOL_snez = 81,                      /* snez  */
  YYSYMBOL_xor = 82,                       /* xor  */
  YYSYMBOL_srl = 83,                       /* srl  */
  YYSYMBOL_sra = 84,                       /* sra  */
  YYSYMBOL_or = 85,                        /* or  */
  YYSYMBOL_and = 86,                       /* and  */
  YYSYMBOL_i_type = 87,                    /* i_type  */
  YYSYMBOL_addi = 88,                      /* addi  */
  YYSYMBOL_nop = 89,                       /* nop  */
  YYSYMBOL_mv = 90,                        /* mv  */
  YYSYMBOL_li = 91,                        /* li  */
  YYSYMBOL_subi = 92,                      /* subi  */
  YYSYMBOL_slli = 93,                      /* slli  */
  YYSYMBOL_slti = 94,                      /* slti  */
  YYSYMBOL_sltiu = 95,                     /* sltiu  */
  YYSYMBOL_seqz = 96,                      /* seqz  */
  YYSYMBOL_xori = 97,                      /* xori  */
  YYSYMBOL_ori = 98,                       /* ori  */
  YYSYMBOL_andi = 99,                      /* andi  */
  YYSYMBOL_srli = 100,                     /* srli  */
  YYSYMBOL_srai = 101,                     /* srai  */
  YYSYMBOL_jalr = 102,                     /* jalr  */
  YYSYMBOL_jr = 103,                       /* jr  */
  YYSYMBOL_ret = 104,                      /* ret  */
  YYSYMBOL_lw = 105,                       /* lw  */
  YYSYMBOL_lb = 106,                       /* lb  */
  YYSYMBOL_lh = 107,                       /* lh  */
  YYSYMBOL_lbu = 108,                      /* lbu  */
  YYSYMBOL_lhu = 109,                      /* lhu  */
  YYSYMBOL_s_type = 110,                   /* s_type  */
  YYSYMBOL_sw = 111,                       /* sw  */
  YYSYMBOL_sb = 112,                       /* sb  */
  YYSYMBOL_sh = 113,                       /* sh  */
  YYSYMBOL_b_type = 114,                   /* b_type  */
  YYSYMBOL_beq = 115,                      /* beq  */
  YYSYMBOL_beqz = 116,                     /* beqz  */
  YYSYMBOL_bne = 117,                      /* bne  */
  YYSYMBOL_bnez = 118,                     /* bnez  */
  YYSYMBOL_blt = 119,                      /* blt  */
  YYSYMBOL_bge = 120,                      /* bge  */
  YYSYMBOL_bltu = 121,                     /* bltu  */
  YYSYMBOL_bgeu = 122,                     /* bgeu  */
  YYSYMBOL_j_type = 123,                   /* j_type  */
  YYSYMBOL_j = 124,                        /* j  */
  YYSYMBOL_jal = 125,                      /* jal  */
  YYSYMBOL_ui_type = 126,                  /* ui_type  */
  YYSYMBOL_auipc = 127,                    /* auipc  */
  YYSYMBOL_lui = 128,                      /* lui  */
  YYSYMBOL_la = 129,                       /* la  */
  YYSYMBOL_align_pow2 = 130,               /* align_pow2  */
  YYSYMBOL_imm = 131                       /* imm  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   316

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  356

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   318


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    35,    35,    37,    38,    40,    42,    43,    45,    46,
      47,    49,    56,    57,    58,    68,    69,    70,    71,    72,
      73,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    91,    99,   107,   115,   123,   131,   139,
     147,   155,   163,   171,   179,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   213,   221,   229,
     237,   245,   253,   261,   269,   277,   285,   293,   301,   309,
     317,   325,   333,   341,   349,   357,   365,   373,   381,   391,
     392,   393,   396,   404,   412,   422,   423,   424,   425,   426,
     427,   428,   429,   432,   440,   449,   457,   466,   474,   483,
     491,   500,   508,   517,   525,   534,   542,   551,   559,   570,
     571,   574,   582,   591,   599,   610,   611,   612,   615,   623,
     631,   647,   650,   654
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "REGISTER", "NEWLINE", "COMMA", "LEFT_PAREN", "RIGHT_PAREN", "MINUS",
  "IMMEDIATE", "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA",
  "OR", "AND", "NEG", "SNEZ", "ADDI", "SLLI", "LW", "SLTI", "SLTIU",
  "XORI", "ORI", "ANDI", "SRLI", "SRAI", "JALR", "LB", "LH", "LBU", "LHU",
  "RET", "LI", "MV", "NOP", "SUBI", "JR", "SEQZ", "SW", "SH", "SB", "BEQ",
  "BNE", "BLT", "BGE", "BLTU", "BGEU", "BEQZ", "BNEZ", "J", "JAL", "AUIPC",
  "LUI", "LA", "DOT_TEXT", "DOT_DATA", "ALIGN", "':'", "$accept",
  "program", "segments", "segment", "label_list", "opt_item", "label",
  "directive", "instruction", "r_type", "add", "sub", "neg", "sll", "slt",
  "sltu", "snez", "xor", "srl", "sra", "or", "and", "i_type", "addi",
  "nop", "mv", "li", "subi", "slli", "slti", "sltiu", "seqz", "xori",
  "ori", "andi", "srli", "srai", "jalr", "jr", "ret", "lw", "lb", "lh",
  "lbu", "lhu", "s_type", "sw", "sb", "sh", "b_type", "beq", "beqz", "bne",
  "bnez", "blt", "bge", "bltu", "bgeu", "j_type", "j", "jal", "ui_type",
  "auipc", "lui", "la", "align_pow2", "imm", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-60)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -60,     4,     5,   -60,   -60,    -2,   -58,     3,    96,    97,
      98,    99,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   -60,   124,   125,   -60,   127,   128,
     129,   130,   131,   132,   133,   140,   141,   142,   146,   147,
     148,   149,    59,   150,   151,   154,   158,   -60,   -60,    -7,
      58,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,     2,    69,    81,    93,   157,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   -60,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   -60,    55,   -60,   -60,   196,   197,
     198,   199,   -60,   -60,   -60,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,    -7,
     216,   217,   220,   228,   229,   230,   231,    -7,    -7,    -7,
      -7,    -7,    -7,   232,   234,   235,    -7,    -7,    -7,   236,
     243,   244,   245,   246,   247,    61,    63,   -60,    71,    -7,
      -7,    64,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   -60,   -60,   258,   259,    70,   260,   261,   262,
     263,   264,   265,   266,    72,    82,    84,   267,   268,   -60,
     -60,   270,   -60,   271,   272,   273,   275,   276,   277,   278,
     279,   280,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   269,   283,   284,   285,   286,   287,   288,   289,   290,
     291,    -7,    -7,   292,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,   293,   294,   295,   296,   297,    -7,   298,   299,   300,
      73,    75,    83,    85,    87,    95,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   156,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   301,   302,   303,   304,
     305,   -60,   306,   307,   308,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     6,     1,     4,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    12,    13,     0,
       0,     7,    10,     9,    15,    21,    22,    31,    23,    24,
      25,    32,    26,    27,    28,    29,    30,    16,    45,    62,
      63,    64,    60,    46,    48,    49,    66,    50,    51,    52,
      53,    54,    55,    65,    61,    47,    56,    57,    58,    59,
      18,    89,    90,    91,    19,    95,   101,    96,   102,    97,
      98,    99,   100,    20,   119,   120,    17,   125,   126,   127,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,   133,   121,     0,     0,
       0,     0,    14,   131,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    35,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
      69,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,   105,   110,   109,   124,   123,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    34,    36,    37,
      38,    40,    41,    42,    43,    44,    67,    72,     0,    73,
      74,    76,    77,    78,    79,    80,     0,     0,     0,     0,
       0,    71,     0,     0,     0,   104,   103,   108,   107,   112,
     111,   114,   113,   116,   115,   118,   117,    84,    81,    85,
      86,    87,    88,    92,    94,    93
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -59
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,     5,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   172,   167
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     173,     6,   165,   166,     3,    -2,   120,   121,   175,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,   164,   174,   262,   217,   264,   270,   165,   166,
     165,   166,   165,   166,   266,   176,   335,   283,   337,   291,
     165,   166,   165,   166,   165,   166,   339,   177,   341,   292,
     343,   293,   165,   166,   165,   166,   165,   166,   345,   178,
     122,   123,   124,   125,   165,   166,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     236,   150,   151,   152,   153,   154,   155,   156,   244,   245,
     246,   247,   248,   249,   157,   158,   159,   253,   254,   255,
     160,   161,   162,   163,   168,   169,   263,   265,   170,   267,
     268,   269,   171,   179,   347,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     237,   238,   316,   317,   239,   319,   320,   321,   322,   323,
     324,   325,   240,   241,   242,   243,   250,   331,   251,   252,
     256,   336,   338,   340,   342,   344,   346,   257,   258,   259,
     260,   261,     0,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   284,   285,   286,   287,
     288,   289,   290,   306,   294,   295,   296,     0,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   318,   326,   327,   328,
     329,   330,   332,   333,   334,     0,     0,     0,     0,   348,
     349,   350,   351,   352,   353,   354,   355
};

static const yytype_int16 yycheck[] =
{
      59,     3,     9,    10,     0,     0,    64,     4,     6,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,     3,     5,     3,    10,     3,     3,     9,    10,
       9,    10,     9,    10,     3,     6,     3,     7,     3,     7,
       9,    10,     9,    10,     9,    10,     3,     6,     3,     7,
       3,     7,     9,    10,     9,    10,     9,    10,     3,     6,
       4,     4,     4,     4,     9,    10,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     189,     4,     4,     4,     4,     4,     4,     4,   197,   198,
     199,   200,   201,   202,     4,     4,     4,   206,   207,   208,
       4,     4,     4,     4,     4,     4,   215,   216,     4,   218,
     219,   220,     4,     6,     8,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,   281,   282,     4,   284,   285,   286,   287,   288,
     289,   290,     4,     4,     4,     4,     4,   296,     4,     4,
       4,   300,   301,   302,   303,   304,   305,     4,     4,     4,
       4,     4,    -1,    -1,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     4,     7,     7,     6,    -1,     7,     7,
       7,     6,     6,     6,     6,     6,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,    -1,    -1,    -1,    -1,     8,
       8,     8,     8,     8,     8,     8,     8
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    66,    67,     0,    68,    69,     3,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      64,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     3,     9,    10,   131,     4,     4,
       4,     4,   130,   131,     5,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,    10,     6,     6,
       6,     6,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,   131,     4,     4,     4,
       4,     4,     4,     4,   131,   131,   131,   131,   131,   131,
       4,     4,     4,   131,   131,   131,     4,     4,     4,     4,
       4,     4,     3,   131,     3,   131,     3,   131,   131,   131,
       3,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     7,     6,     6,     6,     6,     6,     6,
       6,     7,     7,     7,     7,     7,     6,     7,     7,     7,
       6,     6,     6,     6,     6,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,   131,   131,     4,   131,
     131,   131,   131,   131,   131,   131,     4,     4,     4,     4,
       4,   131,     4,     4,     4,     3,   131,     3,   131,     3,
     131,     3,   131,     3,   131,     3,   131,     8,     8,     8,
       8,     8,     8,     8,     8,     8
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    67,    68,    69,    69,    70,    70,
      70,    71,    72,    72,    72,    73,    73,    73,    73,    73,
      73,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     110,   110,   111,   112,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   115,   115,   116,   116,   117,   117,   118,
     118,   119,   119,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   124,   125,   125,   126,   126,   126,   127,   128,
     129,   130,   131,   131
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     3,     0,     2,     0,     1,
       1,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     6,     4,     6,     6,     6,     4,
       6,     6,     6,     6,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     1,     4,
       4,     6,     6,     6,     6,     4,     6,     6,     6,     6,
       6,     7,     2,     1,     7,     7,     7,     7,     7,     1,
       1,     1,     7,     7,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     6,     4,     4,     6,     6,     4,
       4,     6,     6,     6,     6,     6,     6,     6,     6,     1,
       1,     2,     2,     4,     4,     1,     1,     1,     4,     4,
       4,     1,     2,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 11: /* label: IDENTIFIER ':'  */
#line 49 "assembler.y"
                       {
  if (!insert_sym(symtab, (yyvsp[-1].s), (int)pass_current_pc())) {
    /* Duplicate label error reported by insert_sym */
  }
  free((yyvsp[-1].s));
}
#line 1432 "assembler.tab.c"
    break;

  case 12: /* directive: DOT_TEXT  */
#line 56 "assembler.y"
                     { pass_set_section(SEC_TEXT);}
#line 1438 "assembler.tab.c"
    break;

  case 13: /* directive: DOT_DATA  */
#line 57 "assembler.y"
           { pass_set_section(SEC_DATA);}
#line 1444 "assembler.tab.c"
    break;

  case 14: /* directive: ALIGN align_pow2  */
#line 58 "assembler.y"
                   {
  uint32_t addr_before = pass_current_pc();
  uint32_t pad = pass_align_current_pc((uint32_t)(yyvsp[0].l));
  if (pad > 0) {
    ir_append_align(cur_section, addr_before, pad, yylineno);
  }
}
#line 1456 "assembler.tab.c"
    break;

  case 33: /* add: ADD REGISTER COMMA REGISTER COMMA REGISTER  */
#line 92 "assembler.y"
{
  ir_append_instr(OP_ADD, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1467 "assembler.tab.c"
    break;

  case 34: /* sub: SUB REGISTER COMMA REGISTER COMMA REGISTER  */
#line 100 "assembler.y"
{
  ir_append_instr(OP_SUB, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1478 "assembler.tab.c"
    break;

  case 35: /* neg: NEG REGISTER COMMA REGISTER  */
#line 108 "assembler.y"
{
  ir_append_instr(OP_SUB, IF_R, (yyvsp[-2].l), 0, (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1489 "assembler.tab.c"
    break;

  case 36: /* sll: SLL REGISTER COMMA REGISTER COMMA REGISTER  */
#line 116 "assembler.y"
{
  ir_append_instr(OP_SLL, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1500 "assembler.tab.c"
    break;

  case 37: /* slt: SLT REGISTER COMMA REGISTER COMMA REGISTER  */
#line 124 "assembler.y"
{
  ir_append_instr(OP_SLT, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1511 "assembler.tab.c"
    break;

  case 38: /* sltu: SLTU REGISTER COMMA REGISTER COMMA REGISTER  */
#line 132 "assembler.y"
{
  ir_append_instr(OP_SLTU, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1522 "assembler.tab.c"
    break;

  case 39: /* snez: SNEZ REGISTER COMMA REGISTER  */
#line 140 "assembler.y"
{
  ir_append_instr(OP_SLTU, IF_R, (yyvsp[-2].l), 0, (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1533 "assembler.tab.c"
    break;

  case 40: /* xor: XOR REGISTER COMMA REGISTER COMMA REGISTER  */
#line 148 "assembler.y"
{
  ir_append_instr(OP_XOR, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1544 "assembler.tab.c"
    break;

  case 41: /* srl: SRL REGISTER COMMA REGISTER COMMA REGISTER  */
#line 156 "assembler.y"
{
  ir_append_instr(OP_SRL, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1555 "assembler.tab.c"
    break;

  case 42: /* sra: SRA REGISTER COMMA REGISTER COMMA REGISTER  */
#line 164 "assembler.y"
{
  ir_append_instr(OP_SRA, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1566 "assembler.tab.c"
    break;

  case 43: /* or: OR REGISTER COMMA REGISTER COMMA REGISTER  */
#line 172 "assembler.y"
{
  ir_append_instr(OP_OR, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1577 "assembler.tab.c"
    break;

  case 44: /* and: AND REGISTER COMMA REGISTER COMMA REGISTER  */
#line 180 "assembler.y"
{
  ir_append_instr(OP_AND, IF_R, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1588 "assembler.tab.c"
    break;

  case 67: /* addi: ADDI REGISTER COMMA REGISTER COMMA imm  */
#line 214 "assembler.y"
{
  ir_append_instr(OP_ADDI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1599 "assembler.tab.c"
    break;

  case 68: /* nop: NOP  */
#line 222 "assembler.y"
{
  ir_append_instr(OP_ADDI, IF_I, 0, 0, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1610 "assembler.tab.c"
    break;

  case 69: /* mv: MV REGISTER COMMA REGISTER  */
#line 230 "assembler.y"
{
  ir_append_instr(OP_ADDI, IF_I, (yyvsp[-2].l), (yyvsp[0].l), -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1621 "assembler.tab.c"
    break;

  case 70: /* li: LI REGISTER COMMA imm  */
#line 238 "assembler.y"
{
  ir_append_instr(OP_ADDI, IF_I, (yyvsp[-2].l), 0, -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1632 "assembler.tab.c"
    break;

  case 71: /* subi: SUBI REGISTER COMMA REGISTER COMMA imm  */
#line 246 "assembler.y"
{
  ir_append_instr(OP_ADDI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, -((yyvsp[0].l)), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1643 "assembler.tab.c"
    break;

  case 72: /* slli: SLLI REGISTER COMMA REGISTER COMMA imm  */
#line 254 "assembler.y"
{
  ir_append_instr(OP_SLLI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1654 "assembler.tab.c"
    break;

  case 73: /* slti: SLTI REGISTER COMMA REGISTER COMMA imm  */
#line 262 "assembler.y"
{
  ir_append_instr(OP_SLTI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1665 "assembler.tab.c"
    break;

  case 74: /* sltiu: SLTIU REGISTER COMMA REGISTER COMMA imm  */
#line 270 "assembler.y"
{
  ir_append_instr(OP_SLTIU, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1676 "assembler.tab.c"
    break;

  case 75: /* seqz: SEQZ REGISTER COMMA REGISTER  */
#line 278 "assembler.y"
{
  ir_append_instr(OP_SLTIU, IF_I, (yyvsp[-2].l), (yyvsp[0].l), -1, 1, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1687 "assembler.tab.c"
    break;

  case 76: /* xori: XORI REGISTER COMMA REGISTER COMMA imm  */
#line 286 "assembler.y"
{
  ir_append_instr(OP_XORI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1698 "assembler.tab.c"
    break;

  case 77: /* ori: ORI REGISTER COMMA REGISTER COMMA imm  */
#line 294 "assembler.y"
{
  ir_append_instr(OP_ORI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1709 "assembler.tab.c"
    break;

  case 78: /* andi: ANDI REGISTER COMMA REGISTER COMMA imm  */
#line 302 "assembler.y"
{
  ir_append_instr(OP_ANDI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1720 "assembler.tab.c"
    break;

  case 79: /* srli: SRLI REGISTER COMMA REGISTER COMMA imm  */
#line 310 "assembler.y"
{
  ir_append_instr(OP_SRLI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1731 "assembler.tab.c"
    break;

  case 80: /* srai: SRAI REGISTER COMMA REGISTER COMMA imm  */
#line 318 "assembler.y"
{
  ir_append_instr(OP_SRAI, IF_I, (yyvsp[-4].l), (yyvsp[-2].l), -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1742 "assembler.tab.c"
    break;

  case 81: /* jalr: JALR REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 326 "assembler.y"
{
  ir_append_instr(OP_JALR, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1753 "assembler.tab.c"
    break;

  case 82: /* jr: JR REGISTER  */
#line 334 "assembler.y"
{
  ir_append_instr(OP_JALR, IF_I, 0, (yyvsp[0].l), -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1764 "assembler.tab.c"
    break;

  case 83: /* ret: RET  */
#line 342 "assembler.y"
{
  ir_append_instr(OP_JALR, IF_I, 0, 1, -1, 0, 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1775 "assembler.tab.c"
    break;

  case 84: /* lw: LW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 350 "assembler.y"
{
  ir_append_instr(OP_LW, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1786 "assembler.tab.c"
    break;

  case 85: /* lb: LB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 358 "assembler.y"
{
  ir_append_instr(OP_LB, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1797 "assembler.tab.c"
    break;

  case 86: /* lh: LH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 366 "assembler.y"
{
  ir_append_instr(OP_LH, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1808 "assembler.tab.c"
    break;

  case 87: /* lbu: LBU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 374 "assembler.y"
{
  ir_append_instr(OP_LBU, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1819 "assembler.tab.c"
    break;

  case 88: /* lhu: LHU REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 382 "assembler.y"
{
  ir_append_instr(OP_LHU, IF_I, (yyvsp[-5].l), (yyvsp[-1].l), -1, (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1830 "assembler.tab.c"
    break;

  case 92: /* sw: SW REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 397 "assembler.y"
{
  ir_append_instr(OP_SW, IF_S, -1, (yyvsp[-1].l), (yyvsp[-5].l), (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1841 "assembler.tab.c"
    break;

  case 93: /* sb: SB REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 405 "assembler.y"
{
  ir_append_instr(OP_SB, IF_S, -1, (yyvsp[-1].l), (yyvsp[-5].l), (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1852 "assembler.tab.c"
    break;

  case 94: /* sh: SH REGISTER COMMA imm LEFT_PAREN REGISTER RIGHT_PAREN  */
#line 413 "assembler.y"
{
  ir_append_instr(OP_SH, IF_S, -1, (yyvsp[-1].l), (yyvsp[-5].l), (yyvsp[-3].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1863 "assembler.tab.c"
    break;

  case 103: /* beq: BEQ REGISTER COMMA REGISTER COMMA imm  */
#line 433 "assembler.y"
{
  ir_append_instr(OP_BEQ, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1874 "assembler.tab.c"
    break;

  case 104: /* beq: BEQ REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 441 "assembler.y"
{
  ir_append_instr(OP_BEQ, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 1886 "assembler.tab.c"
    break;

  case 105: /* beqz: BEQZ REGISTER COMMA imm  */
#line 450 "assembler.y"
{
  ir_append_instr(OP_BEQ, IF_B, -1, (yyvsp[-2].l), 0, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1897 "assembler.tab.c"
    break;

  case 106: /* beqz: BEQZ REGISTER COMMA IDENTIFIER  */
#line 458 "assembler.y"
{
  ir_append_instr(OP_BEQ, IF_B, -1, (yyvsp[-2].l), 0, 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 1909 "assembler.tab.c"
    break;

  case 107: /* bne: BNE REGISTER COMMA REGISTER COMMA imm  */
#line 467 "assembler.y"
{
  ir_append_instr(OP_BNE, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1920 "assembler.tab.c"
    break;

  case 108: /* bne: BNE REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 475 "assembler.y"
{
  ir_append_instr(OP_BNE, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 1932 "assembler.tab.c"
    break;

  case 109: /* bnez: BNEZ REGISTER COMMA imm  */
#line 484 "assembler.y"
{
  ir_append_instr(OP_BNE, IF_B, -1, (yyvsp[-2].l), 0, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1943 "assembler.tab.c"
    break;

  case 110: /* bnez: BNEZ REGISTER COMMA IDENTIFIER  */
#line 492 "assembler.y"
{
  ir_append_instr(OP_BNE, IF_B, -1, (yyvsp[-2].l), 0, 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 1955 "assembler.tab.c"
    break;

  case 111: /* blt: BLT REGISTER COMMA REGISTER COMMA imm  */
#line 501 "assembler.y"
{
  ir_append_instr(OP_BLT, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1966 "assembler.tab.c"
    break;

  case 112: /* blt: BLT REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 509 "assembler.y"
{
  ir_append_instr(OP_BLT, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 1978 "assembler.tab.c"
    break;

  case 113: /* bge: BGE REGISTER COMMA REGISTER COMMA imm  */
#line 518 "assembler.y"
{
  ir_append_instr(OP_BGE, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 1989 "assembler.tab.c"
    break;

  case 114: /* bge: BGE REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 526 "assembler.y"
{
  ir_append_instr(OP_BGE, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 2001 "assembler.tab.c"
    break;

  case 115: /* bltu: BLTU REGISTER COMMA REGISTER COMMA imm  */
#line 535 "assembler.y"
{
  ir_append_instr(OP_BLTU, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2012 "assembler.tab.c"
    break;

  case 116: /* bltu: BLTU REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 543 "assembler.y"
{
  ir_append_instr(OP_BLTU, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 2024 "assembler.tab.c"
    break;

  case 117: /* bgeu: BGEU REGISTER COMMA REGISTER COMMA imm  */
#line 552 "assembler.y"
{
  ir_append_instr(OP_BGEU, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2035 "assembler.tab.c"
    break;

  case 118: /* bgeu: BGEU REGISTER COMMA REGISTER COMMA IDENTIFIER  */
#line 560 "assembler.y"
{
  ir_append_instr(OP_BGEU, IF_B, -1, (yyvsp[-4].l), (yyvsp[-2].l), 0, 
                  (yyvsp[0].s), RELOC_BOFF, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 2047 "assembler.tab.c"
    break;

  case 121: /* j: J imm  */
#line 575 "assembler.y"
{
  ir_append_instr(OP_JAL, IF_J, 0, -1, -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2058 "assembler.tab.c"
    break;

  case 122: /* j: J IDENTIFIER  */
#line 583 "assembler.y"
{
  ir_append_instr(OP_JAL, IF_J, 0, -1, -1, 0, 
                  (yyvsp[0].s), RELOC_JTGT, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 2070 "assembler.tab.c"
    break;

  case 123: /* jal: JAL REGISTER COMMA imm  */
#line 592 "assembler.y"
{
  ir_append_instr(OP_JAL, IF_J, (yyvsp[-2].l), -1, -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2081 "assembler.tab.c"
    break;

  case 124: /* jal: JAL REGISTER COMMA IDENTIFIER  */
#line 600 "assembler.y"
{
  ir_append_instr(OP_JAL, IF_J, (yyvsp[-2].l), -1, -1, 0, 
                  (yyvsp[0].s), RELOC_JTGT, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
  free((yyvsp[0].s));
}
#line 2093 "assembler.tab.c"
    break;

  case 128: /* auipc: AUIPC REGISTER COMMA imm  */
#line 616 "assembler.y"
{
  ir_append_instr(OP_AUIPC, IF_UI, (yyvsp[-2].l), -1, -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2104 "assembler.tab.c"
    break;

  case 129: /* lui: LUI REGISTER COMMA imm  */
#line 624 "assembler.y"
{
  ir_append_instr(OP_LUI, IF_UI, (yyvsp[-2].l), -1, -1, (yyvsp[0].l), 
                  NULL, RELOC_NONE, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);
}
#line 2115 "assembler.tab.c"
    break;

  case 130: /* la: LA REGISTER COMMA IDENTIFIER  */
#line 632 "assembler.y"
{
  ir_append_instr(OP_AUIPC, IF_UI, (yyvsp[-2].l), -1, -1, 0, 
                  (yyvsp[0].s), RELOC_HI20, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);

  ir_append_instr(OP_ADDI, IF_I, (yyvsp[-2].l), (yyvsp[-2].l), -1, 0, 
                  (yyvsp[0].s), RELOC_LO12, cur_section, 
                  pass_current_pc(), 4, yylineno);
  pass_advance_pc(4);

  free((yyvsp[0].s));
}
#line 2133 "assembler.tab.c"
    break;

  case 131: /* align_pow2: imm  */
#line 647 "assembler.y"
                 { (yyval.l) = (yyvsp[0].l); }
#line 2139 "assembler.tab.c"
    break;

  case 132: /* imm: MINUS IMMEDIATE  */
#line 651 "assembler.y"
{
  (yyval.l) = -1 * (yyvsp[0].l);
}
#line 2147 "assembler.tab.c"
    break;

  case 133: /* imm: IMMEDIATE  */
#line 655 "assembler.y"
{
  (yyval.l) = (yyvsp[0].l);
}
#line 2155 "assembler.tab.c"
    break;


#line 2159 "assembler.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 659 "assembler.y"


void yyerror(char *msg){

}
