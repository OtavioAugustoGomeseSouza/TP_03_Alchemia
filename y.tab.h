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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    ID = 258,                      /* ID  */
    NUM_INT = 259,                 /* NUM_INT  */
    NUM_REAL = 260,                /* NUM_REAL  */
    LIT_CHAR = 261,                /* LIT_CHAR  */
    VAL_VERDADE = 262,             /* VAL_VERDADE  */
    VAL_FALSO = 263,               /* VAL_FALSO  */
    TIPO_CHUMBO = 264,             /* TIPO_CHUMBO  */
    TIPO_MERCURIO = 265,           /* TIPO_MERCURIO  */
    TIPO_ELIXIR = 266,             /* TIPO_ELIXIR  */
    TIPO_RUNA = 267,               /* TIPO_RUNA  */
    REAGIR = 268,                  /* REAGIR  */
    TRANSMUTAR = 269,              /* TRANSMUTAR  */
    NEUTRALIZAR = 270,             /* NEUTRALIZAR  */
    FACA = 271,                    /* FACA  */
    FIM = 272,                     /* FIM  */
    FERMENTAR = 273,               /* FERMENTAR  */
    DESTILAR = 274,                /* DESTILAR  */
    CATALISAR = 275,               /* CATALISAR  */
    ATRIBUICAO = 276,              /* ATRIBUICAO  */
    OP_SOMA = 277,                 /* OP_SOMA  */
    OP_SUB = 278,                  /* OP_SUB  */
    OP_MULT = 279,                 /* OP_MULT  */
    OP_DIV = 280,                  /* OP_DIV  */
    OP_MOD = 281,                  /* OP_MOD  */
    OP_AND = 282,                  /* OP_AND  */
    OP_OR = 283,                   /* OP_OR  */
    OP_NOT = 284,                  /* OP_NOT  */
    OP_GT = 285,                   /* OP_GT  */
    OP_LT = 286,                   /* OP_LT  */
    OP_GE = 287,                   /* OP_GE  */
    OP_LE = 288,                   /* OP_LE  */
    OP_EQ = 289,                   /* OP_EQ  */
    OP_NE = 290                    /* OP_NE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define NUM_INT 259
#define NUM_REAL 260
#define LIT_CHAR 261
#define VAL_VERDADE 262
#define VAL_FALSO 263
#define TIPO_CHUMBO 264
#define TIPO_MERCURIO 265
#define TIPO_ELIXIR 266
#define TIPO_RUNA 267
#define REAGIR 268
#define TRANSMUTAR 269
#define NEUTRALIZAR 270
#define FACA 271
#define FIM 272
#define FERMENTAR 273
#define DESTILAR 274
#define CATALISAR 275
#define ATRIBUICAO 276
#define OP_SOMA 277
#define OP_SUB 278
#define OP_MULT 279
#define OP_DIV 280
#define OP_MOD 281
#define OP_AND 282
#define OP_OR 283
#define OP_NOT 284
#define OP_GT 285
#define OP_LT 286
#define OP_GE 287
#define OP_LE 288
#define OP_EQ 289
#define OP_NE 290

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 278 "translate.y"

    char *strval;
    struct expr_s {
        char name[64];
        char tipo[20];
    } expr;

#line 145 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
