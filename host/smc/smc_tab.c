/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ACTION_PREFIX = 258,
     BRANCH = 259,
     DEFAULT = 260,
     ELSE = 261,
     ENTER = 1,
     EXIT = 262,
     IDENTIFIER = 263,
     IDLE = 264,
     INPUTS = 265,
     INPUT_PREFIX = 266,
     INPUT_QUEUE = 267,
     NAME = 268,
     NEXT = 269,
     NO_BRANCH = 270,
     NUMBER = 271,
     PREVIOUS = 272,
     STATE = 273,
     STATE_PREFIX = 274
   };
#endif
/* Tokens.  */
#define ACTION_PREFIX 258
#define BRANCH 259
#define DEFAULT 260
#define ELSE 261
#define ENTER 1
#define EXIT 262
#define IDENTIFIER 263
#define IDLE 264
#define INPUTS 265
#define INPUT_PREFIX 266
#define INPUT_QUEUE 267
#define NAME 268
#define NEXT 269
#define NO_BRANCH 270
#define NUMBER 271
#define PREVIOUS 272
#define STATE 273
#define STATE_PREFIX 274




/* Copy the first part of user declarations.  */
#line 23 "smc_tab.y"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
   
#include <Log.h>

#include "smc.h"

#define  YYDEBUG  1

#define	DBG(fmt,args...)	if ( gDebugYacc ) Log( fmt, ## args );

BOOL     gDebugYacc;
BOOL     gParsedOk = TRUE;

#define	MAX_INPUTS	100
#define	MAX_ACTIONS	100
#define	MAX_STATES	100
#define	MAX_IDS		MAX_INPUTS

typedef struct
{
   char	name[ 64 ];
} Id;

BOOL		gParsingDefaultState;

int		gNumIds;
Id			gId[ MAX_IDS ];

typedef struct
{
	char		      name[ 64 ];
	char		      enterActionStr[ 64 ];
	char		      exitActionStr[ 64 ];
	SMC_StateInput	input[ MAX_INPUTS ];

} ParseState;

SMC_Input	   gSmInput[ MAX_INPUTS ];
SMC_Action		gSmAction[ MAX_ACTIONS ];
SMC_State		gSmState[ MAX_STATES ];
ParseState	   gCurrState;
ParseState	   gDefaultState;

char		      gCurrActionStr[ 64 ];
SMC_Branch		gCurrBranch;
char		      gCurrNextStateStr[ 64 ];

SMC_StateMachine	gSM;

static	void	AddId( char *idStr );
static	void	AddInput( char *inputStr, char *actionStr, SMC_Branch branch, char *nextStateStr );
static	void	AddState( char *stateStr );
static	int	FindAction( char *actionStr );
static	void	ResolveStates( void );

extern	void	yyerror( char *str );
extern	int	yylex( void );



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 88 "smc_tab.y"
{
	long	 num;
	char	*str;
}
/* Line 193 of yacc.c.  */
#line 206 "smc_tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 219 "smc_tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   45

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNRULES -- Number of states.  */
#define YYNSTATES  72

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   274

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     7,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    23,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
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
       5,     6,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     8,    10,    15,    18,    20,    21,
      23,    26,    29,    32,    35,    38,    39,    40,    47,    50,
      51,    53,    56,    57,    67,    69,    71,    72,    74,    75,
      77,    80,    83,    84,    87,    88,    89,    90,    97,   102,
     103,   105,   107,   109,   111,   113,   116,   118,   121
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      25,     0,    -1,    -1,    27,    26,    36,    -1,     1,    -1,
      28,    29,    32,    35,    -1,    14,     9,    -1,    30,    -1,
      -1,    31,    -1,    30,    31,    -1,     3,     9,    -1,    12,
       9,    -1,    20,     9,    -1,    13,    17,    -1,    -1,    -1,
      11,    21,    33,    50,    34,    22,    -1,    10,     9,    -1,
      -1,    37,    -1,    36,    37,    -1,    -1,    19,    38,    39,
      21,    42,    43,    40,    47,    22,    -1,     9,    -1,     5,
      -1,    -1,    41,    -1,    -1,    44,    -1,    41,    44,    -1,
       7,    48,    -1,    -1,     8,    48,    -1,    -1,    -1,    -1,
      45,    50,    46,    23,    48,    49,    -1,     6,    23,    48,
      49,    -1,    -1,     9,    -1,    10,    -1,    16,    -1,    18,
      -1,    15,    -1,     4,     9,    -1,    51,    -1,    50,    51,
      -1,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   114,   149,   156,   159,   166,   167,   170,
     171,   174,   179,   184,   189,   197,   201,   196,   220,   234,
     247,   248,   252,   251,   309,   319,   325,   340,   341,   344,
     345,   348,   357,   360,   369,   372,   376,   372,   403,   426,
     429,   435,   442,   447,   452,   457,   466,   467,   470
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ACTION_PREFIX", "BRANCH", "DEFAULT",
  "ELSE", "ENTER", "EXIT", "IDENTIFIER", "IDLE", "INPUTS", "INPUT_PREFIX",
  "INPUT_QUEUE", "NAME", "NEXT", "NO_BRANCH", "NUMBER", "PREVIOUS",
  "STATE", "STATE_PREFIX", "'['", "']'", "':'", "$accept", "stateMachine",
  "@1", "declarations", "smName", "optDeclList", "declList", "decl",
  "inputList", "@2", "@3", "idleAction", "stateList", "stateItem", "@4",
  "stateId", "optCaseList", "caseList", "optEnterItem", "optExitItem",
  "caseItem", "@5", "@6", "optElseItem", "action", "branch", "idList",
  "idItem", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,     1,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,    91,    93,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    24,    26,    25,    25,    27,    28,    29,    29,    30,
      30,    31,    31,    31,    31,    33,    34,    32,    35,    35,
      36,    36,    38,    37,    39,    39,    39,    40,    40,    41,
      41,    42,    42,    43,    43,    45,    46,    44,    47,    47,
      48,    48,    49,    49,    49,    49,    50,    50,    51
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     4,     2,     1,     0,     1,
       2,     2,     2,     2,     2,     0,     0,     6,     2,     0,
       1,     2,     0,     9,     1,     1,     0,     1,     0,     1,
       2,     2,     0,     2,     0,     0,     0,     6,     4,     0,
       1,     1,     1,     1,     1,     2,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     2,     8,     6,     1,     0,     0,
       0,     0,     0,     0,     7,     9,    22,     3,    20,    11,
      12,    14,    13,     0,    19,    10,    26,    21,    15,     0,
       5,    25,    24,     0,     0,    18,    32,    48,    16,    46,
       0,    34,     0,    47,    40,    41,    31,     0,    28,    17,
      33,    39,    27,    29,     0,     0,     0,    30,    36,     0,
      23,     0,     0,     0,     0,    44,    42,    43,    38,     0,
      45,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     8,     4,     5,    13,    14,    15,    24,    34,
      42,    30,    17,    18,    26,    33,    51,    52,    41,    48,
      53,    54,    61,    56,    46,    68,    38,    39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -45
static const yytype_int8 yypact[] =
{
       3,   -45,    -4,     7,   -45,    -2,   -45,   -45,    -6,     5,
      12,    10,    13,    14,    -2,   -45,   -45,    -6,   -45,   -45,
     -45,   -45,   -45,    -5,    18,   -45,    -3,   -45,   -45,    20,
     -45,   -45,   -45,     9,    22,   -45,    25,   -45,    22,   -45,
      -1,    26,    11,   -45,   -45,   -45,   -45,    -1,    27,   -45,
     -45,    29,    27,   -45,    22,    15,    17,   -45,    22,    -1,
     -45,    19,     8,    -1,    28,   -45,   -45,   -45,   -45,     8,
     -45,   -45
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,   -45,   -45,   -45,   -45,   -45,    30,   -45,   -45,
     -45,   -45,   -45,    23,   -45,   -45,   -45,   -45,   -45,   -45,
     -11,   -45,   -45,   -45,   -44,   -26,    -9,   -38
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -36
static const yytype_int8 yytable[] =
{
      43,     9,    31,    50,     1,     6,    32,     7,    44,    45,
      10,    11,    64,    16,    19,    62,    28,     2,    12,    69,
      43,    20,    22,    65,    66,    23,    67,    21,    29,    35,
      36,    37,    40,    49,    47,    55,   -35,    70,    59,    60,
      27,    57,    63,    71,    25,    58
};

static const yytype_uint8 yycheck[] =
{
      38,     3,     5,    47,     1,     9,     9,     0,     9,    10,
      12,    13,     4,    19,     9,    59,    21,    14,    20,    63,
      58,     9,     9,    15,    16,    11,    18,    17,    10,     9,
      21,     9,     7,    22,     8,     6,     9,     9,    23,    22,
      17,    52,    23,    69,    14,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    14,    25,    27,    28,     9,     0,    26,     3,
      12,    13,    20,    29,    30,    31,    19,    36,    37,     9,
       9,    17,     9,    11,    32,    31,    38,    37,    21,    10,
      35,     5,     9,    39,    33,     9,    21,     9,    50,    51,
       7,    42,    34,    51,     9,    10,    48,     8,    43,    22,
      48,    40,    41,    44,    45,     6,    47,    44,    50,    23,
      22,    46,    48,    23,     4,    15,    16,    18,    49,    48,
       9,    49
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 115 "smc_tab.y"
    {
					int		i;

					/*
					 * Initialize the default state inputs
					 */

					memset( &gDefaultState, 0, sizeof( gDefaultState ));

					for ( i = 0; i < gSM.numInputs; i++ )
					{
						gDefaultState.input[ i ].dflt = TRUE;
						gDefaultState.input[ i ].actionStr = gSmAction[ 0 ].name;
						gDefaultState.input[ i ].nextStateStr = NULL;
						gDefaultState.input[ i ].action = 0;
						gDefaultState.input[ i ].branch = NoBranch;
						gDefaultState.input[ i ].nextState = -1;
					}
					gSM.queueLen = 10;
				;}
    break;

  case 3:
#line 136 "smc_tab.y"
    {
					gSM.input = gSmInput;
					gSM.action = gSmAction;
					gSM.state = gSmState;

					ResolveStates();
					if ( !gParsedOk )
					{
						YYABORT;
					}

					DBG( "Parsing completed\n" );
				;}
    break;

  case 4:
#line 150 "smc_tab.y"
    {
					gParsedOk = FALSE;
					YYABORT;
				;}
    break;

  case 6:
#line 160 "smc_tab.y"
    {
					gSM.name = strdup( (yyvsp[(2) - (2)].str) );
					DBG( "Parsed SM %s\n", gSM.name );
				;}
    break;

  case 11:
#line 175 "smc_tab.y"
    {
					gSM.actionPrefix = strdup( (yyvsp[(2) - (2)].str) );
					DBG( "Parsed ActionPrefix %s\n", gSM.actionPrefix );
				;}
    break;

  case 12:
#line 180 "smc_tab.y"
    {
					gSM.inputPrefix = strdup( (yyvsp[(2) - (2)].str) );
					DBG( "Parsed InputPrefix %s\n", gSM.inputPrefix );
				;}
    break;

  case 13:
#line 185 "smc_tab.y"
    {
					gSM.statePrefix = strdup( (yyvsp[(2) - (2)].str) );
					DBG( "Parsed StatePrefix %s\n", gSM.statePrefix );
				;}
    break;

  case 14:
#line 190 "smc_tab.y"
    {
					gSM.queueLen = (yyvsp[(2) - (2)].num);
					DBG( "Parsed InputQueue %ld\n", gSM.queueLen );
				;}
    break;

  case 15:
#line 197 "smc_tab.y"
    {
					gNumIds = 0;
				;}
    break;

  case 16:
#line 201 "smc_tab.y"
    {
					int		i;

					/*
					 * Copy the inputs that were collected in idList into
					 * the gSmInput array.
					 */

					for ( i = 0; i < gNumIds; i++ )
					{
						gSmInput[ i ].name = strdup( gId[ i ].name );
						DBG( "Parsed input %2d: %s\n", i, gId[ i ].name );
					}
					gSM.input = gSmInput;
					gSM.numInputs = gNumIds;
			  	;}
    break;

  case 18:
#line 221 "smc_tab.y"
    {
					/*
					 * The idle action was specified, make it entry 1
					 */

					ASSERT( gSM.numActions == 0 );
					gSM.numActions = 2;
					gSmAction[ 0 ].name = NULL;
					gSmAction[ 1 ].name = strdup( (yyvsp[(2) - (2)].str) );

					DBG( "Parsed IdleAction %s\n", gSmAction[ 1 ].name );
				;}
    break;

  case 19:
#line 234 "smc_tab.y"
    {
					/*
					 * No idle action specified, make a NULL one anyway
					 */

					ASSERT( gSM.numActions == 0 );
					gSM.numActions = 1;
					gSmAction[ 0 ].name = NULL;

					DBG( "Parsed default IdleAction\n" );
				;}
    break;

  case 22:
#line 252 "smc_tab.y"
    {
					gCurrState = gDefaultState;
				;}
    break;

  case 23:
#line 256 "smc_tab.y"
    {
					int		i;

					/*
					 * Copy the state inputs from what we've just parsed
					 * into the appropriate state entry.
					 */

					if ( gParsingDefaultState )
					{
						gDefaultState = gCurrState;
						for ( i = 0; i < gSM.numInputs; i++ )
						{
							gDefaultState.input[ i ].dflt = TRUE;
						}
					}
					else
					{
						SMC_State	*st = &gSmState[ gSM.numStates++ ];

						/*
						 * Allocate an array of state inputs
						 */

						st->name = strdup( gCurrState.name );
						if ( strlen( gCurrState.enterActionStr ) > 0 )
						{
							st->enterActionStr = strdup( gCurrState.enterActionStr );
						}
						else
						{
							st->enterActionStr = NULL;
						}
						if ( strlen( gCurrState.exitActionStr ) > 0 )
						{
							st->exitActionStr = strdup( gCurrState.exitActionStr );
						}
						else
						{
							st->exitActionStr = NULL;
						}

						st->stateInput = calloc( gSM.numInputs,
												 sizeof( SMC_StateInput ));

						for ( i = 0; i < gSM.numInputs; i++ )
						{
							st->stateInput[ i ] = gCurrState.input[ i ];
						}
					}
				;}
    break;

  case 24:
#line 310 "smc_tab.y"
    {
					gParsingDefaultState = FALSE;
					AddState( (yyvsp[(1) - (1)].str) );
					if ( !gParsedOk )
					{
						YYABORT;
					}
					DBG( "Parsed State %s\n", gCurrState.name );
				;}
    break;

  case 25:
#line 320 "smc_tab.y"
    {
					gParsingDefaultState = TRUE;
					DBG( "Parsed Default State\n" );
				;}
    break;

  case 26:
#line 325 "smc_tab.y"
    {
					char	stateName[ 10 ];

					gParsingDefaultState = FALSE;

					sprintf( stateName, "S%2d", gSM.numStates );
					AddState( stateName );
					if ( !gParsedOk )
					{
						YYABORT;
					}
					DBG( "Parsed Empty State\n" );
				;}
    break;

  case 31:
#line 349 "smc_tab.y"
    {
					/*
					 * This action is performed every time the state is entered
					 * regardless of how it got here.
					 */

					strcpy( gCurrState.enterActionStr, gCurrActionStr );
				;}
    break;

  case 33:
#line 361 "smc_tab.y"
    {
					/*
					 * This action is performed every time the state is exited
					 * regardless of how it was exited.
					 */

					strcpy( gCurrState.exitActionStr, gCurrActionStr );
				;}
    break;

  case 35:
#line 372 "smc_tab.y"
    {
					gNumIds = 0;
				;}
    break;

  case 36:
#line 376 "smc_tab.y"
    {
					int		i;

					if ( gDebugYacc )
					{
						for ( i = 0; i < gNumIds; i++ )
						{
							printf( "Parsed Input %s\n", gId[ i ].name );
						}
					}
				;}
    break;

  case 37:
#line 388 "smc_tab.y"
    {
					int		i;

					for ( i = 0; i < gNumIds; i++ )
					{
						AddInput( gId[ i ].name, gCurrActionStr,
								  gCurrBranch, gCurrNextStateStr );
						if ( !gParsedOk )
						{
							YYABORT;
						}
					}
				;}
    break;

  case 38:
#line 404 "smc_tab.y"
    {
					int		i;

					/*
					 * Else applies to all other inputs which haven't been
					 * explicitly specified. This causes any defaults which
					 * have been specified to be over-ridden.
					 */

					for ( i = 0; i < gSM.numInputs; i++ )
					{
						if ( gCurrState.input[ i ].dflt )
						{
							AddInput( gSmInput[ i ].name, gCurrActionStr,
									  gCurrBranch, gCurrNextStateStr );
							if ( !gParsedOk )
							{
								YYABORT;
							}
						}
					}
				;}
    break;

  case 40:
#line 430 "smc_tab.y"
    {
					strncpy( gCurrActionStr, (yyvsp[(1) - (1)].str), sizeof( gCurrActionStr ) - 1 );
					gCurrActionStr[ sizeof( gCurrActionStr ) - 1 ] = '\0';
					DBG( "Parsed action %s\n", gCurrActionStr );
				;}
    break;

  case 41:
#line 436 "smc_tab.y"
    {
					strcpy( gCurrActionStr, "" );
					DBG( "Parsed action IDLE\n" );
				;}
    break;

  case 42:
#line 443 "smc_tab.y"
    {
					gCurrBranch = NoBranch;
					DBG( "Parsed NoBranch\n" );
				;}
    break;

  case 43:
#line 448 "smc_tab.y"
    {
					gCurrBranch = BranchPrevious;
					DBG( "Parsed BranchPrevious\n" );
				;}
    break;

  case 44:
#line 453 "smc_tab.y"
    {
					gCurrBranch = BranchNext;
					DBG( "Parsed BranchNext\n" );
				;}
    break;

  case 45:
#line 458 "smc_tab.y"
    {
					gCurrBranch = BranchId;
					strncpy( gCurrNextStateStr, (yyvsp[(2) - (2)].str), sizeof( gCurrNextStateStr ) - 1 );
					gCurrNextStateStr[ sizeof( gCurrNextStateStr ) - 1 ] = '\0';
					DBG( "Parsed BranchId %s\n", gCurrNextStateStr );
				;}
    break;

  case 48:
#line 471 "smc_tab.y"
    {
					AddId( (yyvsp[(1) - (1)].str) );
					if ( !gParsedOk )
					{
						YYABORT;
					}
				;}
    break;


/* Line 1267 of yacc.c.  */
#line 1868 "smc_tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 480 "smc_tab.y"


/************************************************************************
 *
 *	AddId
 *
 ************************************************************************/

void AddId( char *idStr )
{
	char		msg[ 100 ];

	if (( gNumIds + 1 ) >= MAX_IDS )
	{
		sprintf( msg, "ID Table size exceeded, size = %d", MAX_IDS );
		yyerror( msg );
		return;
	}

	if ( strlen( idStr ) >= sizeof( gId[ gNumIds ].name ))
	{
		sprintf( msg, "ID limited to %d characters", sizeof( gId[ gNumIds ].name ));
		yyerror( msg );
		return;
	}

	strcpy( gId[ gNumIds++ ].name, idStr );
}

/************************************************************************
 *
 *	AddInput
 *
 ************************************************************************/

void AddInput( char *inputStr, char *actionStr,
			   SMC_Branch branch, char *nextStateStr )
{
	int		i;
	char	msg[ 100 ];

	/*
	 * Determine the index of the input
	 */

	for ( i = 0; i < gSM.numInputs; i++ )
	{
		if ( strcmp( gSmInput[ i ].name, inputStr ) == 0 )
		{
			break;
		}
	}
	if ( i >= gSM.numInputs )
	{
		/*
		 * The input wasn't found
		 */

		sprintf( msg, "Undefined input: %s", inputStr );
		yyerror( msg );		
		return;
	}

	/*
	 * Add the action and the branch
	 */

	ASSERT( actionStr != NULL );

	gCurrState.input[ i ].dflt = FALSE;
	gCurrState.input[ i ].actionStr = strdup( actionStr );
	gCurrState.input[ i ].action = -1;
	gCurrState.input[ i ].branch = branch;
	if ( branch == BranchId )
	{
		ASSERT( nextStateStr != NULL );
		gCurrState.input[ i ].nextStateStr = strdup( nextStateStr );
	}
	else
	{
		gCurrState.input[ i ].nextStateStr = NULL;
	}
}

/************************************************************************
 *
 *	AddState
 *
 ************************************************************************/

void AddState( char *stateStr )
{
	int		i;
	char	msg[ 100 ];

	if (( gSM.numStates + 1 ) >= MAX_STATES )
	{
		sprintf( msg, "State Table size exceeded, size = %d", MAX_STATES );
		yyerror( msg );
		return;
	}

	/*
	 * Make sure that it's not a duplicate state name
	 */

	for ( i = 0; i < gSM.numStates; i++ )
	{
		if ( strcmp( gSmState[ i ].name, stateStr ) == 0 )
		{
			sprintf( msg, "State %s already defined", stateStr );
			yyerror( msg );
			return;
		}
	}

	strncpy( gCurrState.name, stateStr, sizeof( gCurrState.name ) - 1 );
	gCurrState.name[ sizeof( gCurrState.name ) - 1 ] = '\0';
}

/************************************************************************
 *
 *	FindAction
 *
 ************************************************************************/

int FindAction( char *actionStr )
{
	int		actionIdx;
	char	msg[ 100 ];

	if ( actionStr == NULL )
	{
		return 0;
	}

	/*
	 * Resolve actions into indexes
	 */

	for ( actionIdx = 1; actionIdx < gSM.numActions; actionIdx++ )
	{
		if  ( strcmp( actionStr, gSmAction[ actionIdx ].name ) == 0 )
		{
			return actionIdx;
		}
	}

	/*
	 * The action hasn't been resolved yet, add a new one
	 */

	if (( gSM.numActions + 1 ) >= MAX_ACTIONS )
	{
		sprintf( msg, "Action Table size exceeded, size = %d",
				 MAX_ACTIONS );
		yyerror( msg );
		return 0;
	}

	gSmAction[ actionIdx ].name = actionStr;
	gSM.numActions = actionIdx + 1;

	return actionIdx;
}

/************************************************************************
 *
 *	ResolveStates
 *
 ************************************************************************/

void ResolveStates( void )
{
	int		stateIdx;
	int		inputIdx;
	int		branchIdx;

	for ( stateIdx = 0; stateIdx < gSM.numStates; stateIdx++ )
	{
		SMC_State	*st = &gSM.state[ stateIdx ];

		st->enterAction = FindAction( st->enterActionStr );
		st->exitAction = FindAction( st->exitActionStr );

		for ( inputIdx = 0; inputIdx < gSM.numInputs; inputIdx++ )
		{
			SMC_StateInput	*si = &st->stateInput[ inputIdx ];

			/*
			 * Resolve actions into indexes
			 */

			si->action = FindAction( si->actionStr );

			/*
			 * Resolve branches into states
			 */

			switch ( si->branch )
			{
				case NoBranch:
				{
					si->nextState = stateIdx;
					break;
				}

				case BranchPrevious:
				{
					if ( stateIdx <= 0 )
					{
						fprintf( stderr, "ERROR: Can't branch to a previous "
								 "state from state %s (input %s)\n",
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}	
					else
					{
						si->nextState = stateIdx - 1;
					}
					break;
				}

				case BranchNext:
				{
					if (( stateIdx + 1 ) >= gSM.numStates )
					{
						fprintf( stderr, "ERROR: Can't branch to a next state "
								 "from state %s (input %s)\n",
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}
					else
					{
						si->nextState = stateIdx + 1;
					}
					break;
				}

				case BranchId:
				{
					for ( branchIdx = 0; branchIdx < gSM.numStates; branchIdx++ )
					{
						if ( strcmp( gSM.state[ branchIdx ].name,
									 si->nextStateStr ) == 0 )
						{
							si->nextState = branchIdx;
							break;
						}
					}
					if ( branchIdx >= gSM.numStates )
					{
						fprintf( stderr, "ERROR: Unrecognized branch: %s in "
								 "state %s, input %s\n", si->nextStateStr,
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}
					break;
				}

				default:
				{
					fprintf( stderr, "ERROR: Internal error, unrecognized "
							 "branch during resolve\n" );
					gParsedOk = FALSE;
					break;
				}
			}
		}
	}
}


