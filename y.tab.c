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
#line 1 "translate.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int linha_atual;
extern char *yytext;
extern FILE *yyin;

int yylex(void);
void yyerror(const char *s);

/* ===================== TABELA DE SIMBOLOS ===================== */

typedef struct {
    char nome[50];
    char tipo[20];
} Simbolo;

Simbolo tabela[100];
int total_simbolos = 0;

int inserir_tabela(const char *nome, const char *tipo);
char *getDataType(const char *nome);
void imprimir_tabela_simbolos(void);
int imprimir_fonte_numerado(const char *nome_arquivo);

/* ===================== ERROS SEMANTICOS ========================= */

#define MAX_ERROS 200
char semanticErros[MAX_ERROS][256];
int numSemanticErros = 0;

void semantic_error(int linha, const char *fmt, ...) {
    char msg[200];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    if (numSemanticErros < MAX_ERROS) {
        snprintf(semanticErros[numSemanticErros], sizeof(semanticErros[0]),
                 "Erro semantico (linha %d): %s", linha, msg);
        numSemanticErros++;
    }
}

/* ===================== CODIGO DE TRES ENDERECOS ================= */

#define MAX_TAC 4000
#define TAC_LINE_LEN 200
char tac_buffer[MAX_TAC][TAC_LINE_LEN];
int tac_count = 0;

void add_tac(const char *fmt, ...) {
    if (tac_count >= MAX_TAC) return;
    va_list args;
    va_start(args, fmt);
    vsnprintf(tac_buffer[tac_count], TAC_LINE_LEN, fmt, args);
    va_end(args);
    tac_count++;
}

void imprimir_codigo_intermediario(void) {
    int i;
    printf("\n\n=== CODIGO INTERMEDIARIO (TRES ENDERECOS) ===\n");
    if (tac_count == 0) {
        printf("(nenhuma instrucao gerada)\n");
    }
    for (i = 0; i < tac_count; i++) {
        int len = strlen(tac_buffer[i]);
        if (len > 0 && tac_buffer[i][len - 1] == ':')
            printf("%s\n", tac_buffer[i]);          /* rotulo: sem indentacao */
        else
            printf("    %s\n", tac_buffer[i]);       /* instrucao: indentada   */
    }
    printf("==============================================\n");
}

/* contadores para temporarios e rotulos */
int temp_count = 0;
int label_count = 0;

char *novo_temp(void) {
    char buf[16];
    sprintf(buf, "t%d", temp_count++);
    return strdup(buf);
}

char *novo_label(void) {
    char buf[16];
    sprintf(buf, "L%d", label_count++);
    return strdup(buf);
}

/* pilha auxiliar de rotulos, usada para gerar corretamente o codigo
 * de comandos condicionais e de repeticao, inclusive quando aninhados */
#define MAX_PILHA 128
char *pilha_labels[MAX_PILHA];
int topo_labels = 0;

void push_label(char *l) { pilha_labels[topo_labels++] = l; }
char *pop_label(void)     { return pilha_labels[--topo_labels]; }
char *peek_label(void)    { return pilha_labels[topo_labels - 1]; }

/* pilha auxiliar de "marcas" (posicoes no buffer de codigo), usada
 * pelo DESTILAR (for) para reordenar o codigo do passo de atualizacao,
 * que e escrito antes do corpo do laco no codigo fonte, mas que deve
 * ser executado depois do corpo no codigo de tres enderecos */
int pilha_marcas[MAX_PILHA];
int topo_marcas = 0;
void push_marca(int m) { pilha_marcas[topo_marcas++] = m; }
int pop_marca(void)     { return pilha_marcas[--topo_marcas]; }

#define MAX_NEST 32
char update_temp[MAX_NEST][10][TAC_LINE_LEN];
int update_temp_count[MAX_NEST];
int update_temp_top = 0;

void mover_para_buffer_temporario(int marca) {
    int d = update_temp_top++;
    int i;
    update_temp_count[d] = 0;
    for (i = marca; i < tac_count && update_temp_count[d] < 10; i++) {
        strcpy(update_temp[d][update_temp_count[d]], tac_buffer[i]);
        update_temp_count[d]++;
    }
    tac_count = marca;
}

void restaurar_buffer_temporario(void) {
    int d = --update_temp_top;
    int i;
    for (i = 0; i < update_temp_count[d]; i++) {
        add_tac("%s", update_temp[d][i]);
    }
}

/* ===================== ANALISE SEMANTICA (TIPOS) ================= */

int numerico(const char *t) {
    return strcmp(t, "chumbo") == 0 || strcmp(t, "mercurio") == 0;
}

/* permite atribuir chumbo (int) a uma variavel mercurio (float),
 * por promocao implicita, assim como em C */
int tipos_compativeis(const char *destino, const char *origem) {
    if (strcmp(destino, origem) == 0) return 1;
    if (strcmp(destino, "mercurio") == 0 && strcmp(origem, "chumbo") == 0) return 1;
    return 0;
}

/* ===================== GERACAO DE ASSEMBLY MOS6502 ================= */

typedef struct { char name[64]; unsigned char addr; } Addr6502;
static Addr6502 addr6502[256];
static int addr6502_count = 0;
static unsigned char next_zp_addr = 0x00;
static int asm_label_id = 0;

unsigned char get_zp_addr(const char *name);
int is_int_literal(const char *s);
int is_float_literal(const char *s);
int is_char_literal(const char *s);
int literal_to_byte(const char *s);
void emit_load(FILE *f, const char *operand);
void emit_cmp_op(FILE *f, const char *operand);
void emit_adc_op(FILE *f, const char *operand);
void emit_sbc_op(FILE *f, const char *operand);
void emit_comparison(FILE *f, const char *dst, const char *a, const char *b, const char *oper);
void emit_multiply(FILE *f, const char *dst, const char *a, const char *b);
void emit_divide(FILE *f, const char *dst, const char *a, const char *b);
void emit_modulo(FILE *f, const char *dst, const char *a, const char *b);
void emit_and(FILE *f, const char *dst, const char *a, const char *b);
void emit_or(FILE *f, const char *dst, const char *a, const char *b);
void gerar_assembly_6502(const char *filename);


#line 250 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 180 "translate.y"

    char *strval;
    struct expr_s {
        char name[64];
        char tipo[20];
    } expr;

#line 381 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_NUM_INT = 4,                    /* NUM_INT  */
  YYSYMBOL_NUM_REAL = 5,                   /* NUM_REAL  */
  YYSYMBOL_LIT_CHAR = 6,                   /* LIT_CHAR  */
  YYSYMBOL_VAL_VERDADE = 7,                /* VAL_VERDADE  */
  YYSYMBOL_VAL_FALSO = 8,                  /* VAL_FALSO  */
  YYSYMBOL_TIPO_CHUMBO = 9,                /* TIPO_CHUMBO  */
  YYSYMBOL_TIPO_MERCURIO = 10,             /* TIPO_MERCURIO  */
  YYSYMBOL_TIPO_ELIXIR = 11,               /* TIPO_ELIXIR  */
  YYSYMBOL_TIPO_RUNA = 12,                 /* TIPO_RUNA  */
  YYSYMBOL_REAGIR = 13,                    /* REAGIR  */
  YYSYMBOL_TRANSMUTAR = 14,                /* TRANSMUTAR  */
  YYSYMBOL_NEUTRALIZAR = 15,               /* NEUTRALIZAR  */
  YYSYMBOL_FACA = 16,                      /* FACA  */
  YYSYMBOL_FIM = 17,                       /* FIM  */
  YYSYMBOL_FERMENTAR = 18,                 /* FERMENTAR  */
  YYSYMBOL_DESTILAR = 19,                  /* DESTILAR  */
  YYSYMBOL_CATALISAR = 20,                 /* CATALISAR  */
  YYSYMBOL_ATRIBUICAO = 21,                /* ATRIBUICAO  */
  YYSYMBOL_OP_SOMA = 22,                   /* OP_SOMA  */
  YYSYMBOL_OP_SUB = 23,                    /* OP_SUB  */
  YYSYMBOL_OP_MULT = 24,                   /* OP_MULT  */
  YYSYMBOL_OP_DIV = 25,                    /* OP_DIV  */
  YYSYMBOL_OP_MOD = 26,                    /* OP_MOD  */
  YYSYMBOL_OP_AND = 27,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 28,                     /* OP_OR  */
  YYSYMBOL_OP_NOT = 29,                    /* OP_NOT  */
  YYSYMBOL_OP_GT = 30,                     /* OP_GT  */
  YYSYMBOL_OP_LT = 31,                     /* OP_LT  */
  YYSYMBOL_OP_GE = 32,                     /* OP_GE  */
  YYSYMBOL_OP_LE = 33,                     /* OP_LE  */
  YYSYMBOL_OP_EQ = 34,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 35,                     /* OP_NE  */
  YYSYMBOL_36_ = 36,                       /* '('  */
  YYSYMBOL_37_ = 37,                       /* ')'  */
  YYSYMBOL_38_ = 38,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_programa = 40,                  /* programa  */
  YYSYMBOL_lista_declaracoes = 41,         /* lista_declaracoes  */
  YYSYMBOL_declaracao = 42,                /* declaracao  */
  YYSYMBOL_declaracao_variavel = 43,       /* declaracao_variavel  */
  YYSYMBOL_tipo = 44,                      /* tipo  */
  YYSYMBOL_atribuicao = 45,                /* atribuicao  */
  YYSYMBOL_comando_condicional = 46,       /* comando_condicional  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_49_3 = 49,                      /* $@3  */
  YYSYMBOL_sub_condicional = 50,           /* sub_condicional  */
  YYSYMBOL_51_4 = 51,                      /* $@4  */
  YYSYMBOL_52_5 = 52,                      /* $@5  */
  YYSYMBOL_comando_repeticao = 53,         /* comando_repeticao  */
  YYSYMBOL_comando_fermentar = 54,         /* comando_fermentar  */
  YYSYMBOL_55_6 = 55,                      /* $@6  */
  YYSYMBOL_56_7 = 56,                      /* $@7  */
  YYSYMBOL_57_8 = 57,                      /* $@8  */
  YYSYMBOL_comando_destilar = 58,          /* comando_destilar  */
  YYSYMBOL_59_9 = 59,                      /* $@9  */
  YYSYMBOL_60_10 = 60,                     /* $@10  */
  YYSYMBOL_61_11 = 61,                     /* $@11  */
  YYSYMBOL_62_12 = 62,                     /* $@12  */
  YYSYMBOL_comando_catalisar = 63,         /* comando_catalisar  */
  YYSYMBOL_64_13 = 64,                     /* $@13  */
  YYSYMBOL_expressao = 65                  /* expressao  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 188 "translate.y"

typedef struct expr_s Expr;

void verificar_booleano(Expr e) {
    if (strcmp(e.tipo, "elixir") != 0)
        semantic_error(linha_atual,
            "a condicao deve ser do tipo elixir (booleano), mas é do tipo \"%s\"",
            e.tipo);
}

void gerar_aritmetica(Expr *res, Expr e1, Expr e2, const char *op, int apenasInteiro) {
    if (!numerico(e1.tipo) || !numerico(e2.tipo)) {
        semantic_error(linha_atual,
            "operador \"%s\" exige operandos numericos (chumbo ou mercurio); recebidos \"%s\" e \"%s\"",
            op, e1.tipo, e2.tipo);
        strcpy(res->tipo, "chumbo");
    } else if (apenasInteiro && (strcmp(e1.tipo, "chumbo") != 0 || strcmp(e2.tipo, "chumbo") != 0)) {
        semantic_error(linha_atual,
            "operador \"%s\" (residuo) exige operandos do tipo chumbo (inteiro); recebidos \"%s\" e \"%s\"",
            op, e1.tipo, e2.tipo);
        strcpy(res->tipo, "chumbo");
    } else {
        strcpy(res->tipo, (strcmp(e1.tipo, "mercurio") == 0 || strcmp(e2.tipo, "mercurio") == 0)
                            ? "mercurio" : "chumbo");
    }
    char *t = novo_temp();
    add_tac("%s = %s %s %s", t, e1.name, op, e2.name);
    strcpy(res->name, t);
}

void gerar_logico(Expr *res, Expr e1, Expr e2, const char *op) {
    if (strcmp(e1.tipo, "elixir") != 0 || strcmp(e2.tipo, "elixir") != 0)
        semantic_error(linha_atual,
            "operador \"%s\" exige operandos do tipo elixir; recebidos \"%s\" e \"%s\"",
            op, e1.tipo, e2.tipo);
    char *t = novo_temp();
    add_tac("%s = %s %s %s", t, e1.name, op, e2.name);
    strcpy(res->name, t);
    strcpy(res->tipo, "elixir");
}

void gerar_comparacao(Expr *res, Expr e1, Expr e2, const char *op, int restritoNumerico) {
    if (restritoNumerico) {
        if (!numerico(e1.tipo) || !numerico(e2.tipo))
            semantic_error(linha_atual,
                "operador \"%s\" exige operandos numericos (chumbo ou mercurio); recebidos \"%s\" e \"%s\"",
                op, e1.tipo, e2.tipo);
    } else {
        if (strcmp(e1.tipo, e2.tipo) != 0)
            semantic_error(linha_atual,
                "operador \"%s\" exige operandos do mesmo tipo; recebidos \"%s\" e \"%s\"",
                op, e1.tipo, e2.tipo);
    }
    char *t = novo_temp();
    add_tac("%s = %s %s %s", t, e1.name, op, e2.name);
    strcpy(res->name, t);
    strcpy(res->tipo, "elixir");
}

void gerar_negacao(Expr *res, Expr e) {
    if (strcmp(e.tipo, "elixir") != 0)
        semantic_error(linha_atual,
            "operador OPOSTO exige operando do tipo elixir; recebido \"%s\"", e.tipo);
    char *t = novo_temp();
    add_tac("%s = ! %s", t, e.name);
    strcpy(res->name, t);
    strcpy(res->tipo, "elixir");
}

#line 542 "y.tab.c"


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
typedef yytype_int8 yy_state_t;

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
#define YYLAST   258

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  120

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


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
      36,    37,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
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
      35
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   279,   279,   283,   284,   288,   289,   290,   291,   295,
     299,   311,   312,   313,   314,   318,   333,   340,   348,   332,
     357,   364,   356,   370,   371,   375,   376,   377,   382,   388,
     395,   381,   406,   412,   420,   425,   405,   437,   436,   451,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479
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
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUM_INT",
  "NUM_REAL", "LIT_CHAR", "VAL_VERDADE", "VAL_FALSO", "TIPO_CHUMBO",
  "TIPO_MERCURIO", "TIPO_ELIXIR", "TIPO_RUNA", "REAGIR", "TRANSMUTAR",
  "NEUTRALIZAR", "FACA", "FIM", "FERMENTAR", "DESTILAR", "CATALISAR",
  "ATRIBUICAO", "OP_SOMA", "OP_SUB", "OP_MULT", "OP_DIV", "OP_MOD",
  "OP_AND", "OP_OR", "OP_NOT", "OP_GT", "OP_LT", "OP_GE", "OP_LE", "OP_EQ",
  "OP_NE", "'('", "')'", "';'", "$accept", "programa", "lista_declaracoes",
  "declaracao", "declaracao_variavel", "tipo", "atribuicao",
  "comando_condicional", "$@1", "$@2", "$@3", "sub_condicional", "$@4",
  "$@5", "comando_repeticao", "comando_fermentar", "$@6", "$@7", "$@8",
  "comando_destilar", "$@9", "$@10", "$@11", "$@12", "comando_catalisar",
  "$@13", "expressao", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-61)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -61,     2,    85,   -61,   -12,   -61,   -61,   -61,   -61,   -26,
     -61,   -19,   -61,   -61,   -61,    10,   -61,   -61,   -61,   -61,
     -61,   -61,    44,    44,   -17,    11,     0,    -1,   -61,   -61,
     -61,   -61,   -61,   -61,    44,    44,   181,   101,    44,    -4,
     -61,    44,   -61,   117,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,   -61,   133,   -61,
      73,   181,   -61,   -20,   -20,   -61,   -61,   -61,   209,   195,
     223,   223,   223,   223,   223,   223,    19,   -61,    44,    16,
     -61,    21,    84,     3,    85,   -61,   -61,    44,    -7,    85,
      11,   149,     4,    22,   -61,    25,     6,   -61,    44,   -61,
      27,    35,   -61,   165,    85,    32,   -61,    38,   -61,   -61,
     -61,    40,    85,   -61,    41,    85,    42,    -7,   -61,   -61
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,    11,    12,    13,    14,     0,
      28,     0,    37,     3,     5,     0,     6,     7,     8,    25,
      26,    27,     0,     0,     0,     0,     0,     9,    39,    40,
      41,    42,    43,    44,     0,     0,    15,     0,     0,     0,
       4,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,    32,
       0,    10,    45,    55,    56,    57,    58,    59,    48,    47,
      49,    50,    51,    52,    53,    54,     0,    29,     0,     0,
       4,     0,     0,     0,    17,     4,    33,     0,    24,    30,
       0,     0,     0,     0,    18,     0,     0,    38,     0,     4,
       0,     0,    34,     0,    23,     0,    31,     0,    20,    19,
       4,     0,    35,     4,     0,    21,     0,    24,    36,    22
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -61,   -61,   -39,   -61,   -61,   -61,   -22,   -61,   -61,   -61,
     -61,   -60,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -23
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    13,    14,    15,    16,    17,    76,    88,
     100,    94,   111,   117,    18,    19,    24,    81,    95,    20,
      78,    90,   107,   114,    21,    26,    36
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      37,    60,     3,    39,    46,    47,    48,    92,    93,    22,
      23,    42,    43,    27,     4,    58,    40,    25,    61,    38,
      41,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    59,    80,    83,    85,    99,    87,
      98,    84,   101,   102,   105,   109,    89,    28,    29,    30,
      31,    32,    33,   106,   110,    82,   113,   119,   116,     0,
     104,   118,     0,     0,    91,     0,     0,     0,    96,     0,
       0,   112,     0,    34,   115,   103,     4,     0,     0,     0,
      35,     0,     5,     6,     7,     8,     9,     0,     4,     0,
      79,    10,    11,    12,     5,     6,     7,     8,     9,     0,
       0,     0,     0,    10,    11,    12,    44,    45,    46,    47,
      48,    49,    50,     0,    51,    52,    53,    54,    55,    56,
       0,     0,    86,    44,    45,    46,    47,    48,    49,    50,
       0,    51,    52,    53,    54,    55,    56,     0,    57,    44,
      45,    46,    47,    48,    49,    50,     0,    51,    52,    53,
      54,    55,    56,     0,    62,    44,    45,    46,    47,    48,
      49,    50,     0,    51,    52,    53,    54,    55,    56,     0,
      77,    44,    45,    46,    47,    48,    49,    50,     0,    51,
      52,    53,    54,    55,    56,     0,    97,    44,    45,    46,
      47,    48,    49,    50,     0,    51,    52,    53,    54,    55,
      56,     0,   108,    44,    45,    46,    47,    48,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    44,    45,    46,
      47,    48,    49,     0,     0,    51,    52,    53,    54,    55,
      56,    44,    45,    46,    47,    48,     0,     0,     0,    51,
      52,    53,    54,    55,    56,    44,    45,    46,    47,    48,
       0,     0,     0,    -1,    -1,    -1,    -1,    -1,    -1
};

static const yytype_int8 yycheck[] =
{
      23,    40,     0,    25,    24,    25,    26,    14,    15,    21,
      36,    34,    35,     3,     3,    38,    16,    36,    41,    36,
      21,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    38,    16,    20,    16,    16,    36,
      36,    80,    17,    37,    17,    13,    85,     3,     4,     5,
       6,     7,     8,    18,    16,    78,    16,   117,    17,    -1,
      99,    19,    -1,    -1,    87,    -1,    -1,    -1,    90,    -1,
      -1,   110,    -1,    29,   113,    98,     3,    -1,    -1,    -1,
      36,    -1,     9,    10,    11,    12,    13,    -1,     3,    -1,
      17,    18,    19,    20,     9,    10,    11,    12,    13,    -1,
      -1,    -1,    -1,    18,    19,    20,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    22,    23,    24,    25,    26,    27,    28,
      -1,    30,    31,    32,    33,    34,    35,    -1,    37,    22,
      23,    24,    25,    26,    27,    28,    -1,    30,    31,    32,
      33,    34,    35,    -1,    37,    22,    23,    24,    25,    26,
      27,    28,    -1,    30,    31,    32,    33,    34,    35,    -1,
      37,    22,    23,    24,    25,    26,    27,    28,    -1,    30,
      31,    32,    33,    34,    35,    -1,    37,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    22,    23,    24,    25,    26,    27,    28,
      -1,    30,    31,    32,    33,    34,    35,    22,    23,    24,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      35,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    40,    41,     0,     3,     9,    10,    11,    12,    13,
      18,    19,    20,    42,    43,    44,    45,    46,    53,    54,
      58,    63,    21,    36,    55,    36,    64,     3,     3,     4,
       5,     6,     7,     8,    29,    36,    65,    65,    36,    45,
      16,    21,    65,    65,    22,    23,    24,    25,    26,    27,
      28,    30,    31,    32,    33,    34,    35,    37,    65,    38,
      41,    65,    37,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    47,    37,    59,    17,
      16,    56,    65,    20,    41,    16,    38,    36,    48,    41,
      60,    65,    14,    15,    50,    57,    45,    37,    36,    16,
      49,    17,    37,    65,    41,    17,    18,    61,    37,    13,
      16,    51,    41,    16,    62,    41,    17,    52,    19,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    42,    42,    43,
      43,    44,    44,    44,    44,    45,    47,    48,    49,    46,
      51,    52,    50,    50,    50,    53,    53,    53,    55,    56,
      57,    54,    59,    60,    61,    62,    58,    64,    63,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     1,     2,
       4,     1,     1,     1,     1,     3,     0,     0,     0,    12,
       0,     0,     9,     3,     0,     1,     1,     1,     0,     0,
       0,    11,     0,     0,     0,     0,    16,     0,     9,     1,
       1,     1,     1,     1,     1,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3
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
  case 9: /* declaracao_variavel: tipo ID  */
#line 295 "translate.y"
              {
          if (inserir_tabela((yyvsp[0].strval), (yyvsp[-1].strval)))
              semantic_error(linha_atual, "variavel \"%s\" ja foi declarada", (yyvsp[0].strval));
      }
#line 1602 "y.tab.c"
    break;

  case 10: /* declaracao_variavel: tipo ID ATRIBUICAO expressao  */
#line 299 "translate.y"
                                   {
          if (inserir_tabela((yyvsp[-2].strval), (yyvsp[-3].strval)))
              semantic_error(linha_atual, "variavel \"%s\" ja foi declarada", (yyvsp[-2].strval));
          if (!tipos_compativeis((yyvsp[-3].strval), (yyvsp[0].expr).tipo))
              semantic_error(linha_atual,
                  "tipos incompativeis na inicializacao de \"%s\": esperado \"%s\", recebido \"%s\"",
                  (yyvsp[-2].strval), (yyvsp[-3].strval), (yyvsp[0].expr).tipo);
          add_tac("%s = %s", (yyvsp[-2].strval), (yyvsp[0].expr).name);
      }
#line 1616 "y.tab.c"
    break;

  case 11: /* tipo: TIPO_CHUMBO  */
#line 311 "translate.y"
                       { (yyval.strval) = "chumbo"; }
#line 1622 "y.tab.c"
    break;

  case 12: /* tipo: TIPO_MERCURIO  */
#line 312 "translate.y"
                       { (yyval.strval) = "mercurio"; }
#line 1628 "y.tab.c"
    break;

  case 13: /* tipo: TIPO_ELIXIR  */
#line 313 "translate.y"
                       { (yyval.strval) = "elixir"; }
#line 1634 "y.tab.c"
    break;

  case 14: /* tipo: TIPO_RUNA  */
#line 314 "translate.y"
                       { (yyval.strval) = "runa"; }
#line 1640 "y.tab.c"
    break;

  case 15: /* atribuicao: ID ATRIBUICAO expressao  */
#line 318 "translate.y"
                              {
          char *tipoVar = getDataType((yyvsp[-2].strval));
          if (!tipoVar) {
              semantic_error(linha_atual, "variavel \"%s\" nao foi declarada", (yyvsp[-2].strval));
          } else if (!tipos_compativeis(tipoVar, (yyvsp[0].expr).tipo)) {
              semantic_error(linha_atual,
                  "tipos incompativeis na atribuicao a \"%s\": esperado \"%s\", recebido \"%s\"",
                  (yyvsp[-2].strval), tipoVar, (yyvsp[0].expr).tipo);
          }
          add_tac("%s = %s", (yyvsp[-2].strval), (yyvsp[0].expr).name);
      }
#line 1656 "y.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 333 "translate.y"
      {
          verificar_booleano((yyvsp[-1].expr));
          char *lelse = novo_label();
          add_tac("ifFalse %s goto %s", (yyvsp[-1].expr).name, lelse);
          push_label(lelse);
      }
#line 1667 "y.tab.c"
    break;

  case 17: /* $@2: %empty  */
#line 340 "translate.y"
      {
          char *lelse = pop_label();
          char *lend = novo_label();
          add_tac("goto %s", lend);
          add_tac("%s:", lelse);
          push_label(lend);
      }
#line 1679 "y.tab.c"
    break;

  case 18: /* $@3: %empty  */
#line 348 "translate.y"
      {
          char *lend = pop_label();
          add_tac("%s:", lend);
      }
#line 1688 "y.tab.c"
    break;

  case 20: /* $@4: %empty  */
#line 357 "translate.y"
      {
          verificar_booleano((yyvsp[-1].expr));
          char *lelse = novo_label();
          add_tac("ifFalse %s goto %s", (yyvsp[-1].expr).name, lelse);
          push_label(lelse);
      }
#line 1699 "y.tab.c"
    break;

  case 21: /* $@5: %empty  */
#line 364 "translate.y"
      {
          char *lelse = pop_label();
          add_tac("goto %s", peek_label());
          add_tac("%s:", lelse);
      }
#line 1709 "y.tab.c"
    break;

  case 28: /* $@6: %empty  */
#line 382 "translate.y"
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
#line 1719 "y.tab.c"
    break;

  case 29: /* $@7: %empty  */
#line 388 "translate.y"
      {
          verificar_booleano((yyvsp[-1].expr));
          char *lend = novo_label();
          add_tac("ifFalse %s goto %s", (yyvsp[-1].expr).name, lend);
          push_label(lend);
      }
#line 1730 "y.tab.c"
    break;

  case 30: /* $@8: %empty  */
#line 395 "translate.y"
      {
          char *lend = pop_label();
          char *lstart = pop_label();
          add_tac("goto %s", lstart);
          add_tac("%s:", lend);
      }
#line 1741 "y.tab.c"
    break;

  case 32: /* $@9: %empty  */
#line 406 "translate.y"
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
#line 1751 "y.tab.c"
    break;

  case 33: /* $@10: %empty  */
#line 412 "translate.y"
      {
          verificar_booleano((yyvsp[-1].expr));
          char *lend = novo_label();
          add_tac("ifFalse %s goto %s", (yyvsp[-1].expr).name, lend);
          push_label(lend);
          push_marca(tac_count);
      }
#line 1763 "y.tab.c"
    break;

  case 34: /* $@11: %empty  */
#line 420 "translate.y"
      {
          int marca = pop_marca();
          mover_para_buffer_temporario(marca);
      }
#line 1772 "y.tab.c"
    break;

  case 35: /* $@12: %empty  */
#line 425 "translate.y"
      {
          restaurar_buffer_temporario();
          char *lend = pop_label();
          char *lstart = pop_label();
          add_tac("goto %s", lstart);
          add_tac("%s:", lend);
      }
#line 1784 "y.tab.c"
    break;

  case 37: /* $@13: %empty  */
#line 437 "translate.y"
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
#line 1794 "y.tab.c"
    break;

  case 38: /* comando_catalisar: CATALISAR $@13 FACA lista_declaracoes FIM CATALISAR '(' expressao ')'  */
#line 443 "translate.y"
      {
          verificar_booleano((yyvsp[-1].expr));
          char *lstart = pop_label();
          add_tac("if %s goto %s", (yyvsp[-1].expr).name, lstart);
      }
#line 1804 "y.tab.c"
    break;

  case 39: /* expressao: ID  */
#line 451 "translate.y"
         {
          char *t = getDataType((yyvsp[0].strval));
          if (!t) {
              semantic_error(linha_atual, "variavel \"%s\" nao foi declarada", (yyvsp[0].strval));
              t = "chumbo";
          }
          strcpy((yyval.expr).name, (yyvsp[0].strval));
          strcpy((yyval.expr).tipo, t);
      }
#line 1818 "y.tab.c"
    break;

  case 40: /* expressao: NUM_INT  */
#line 460 "translate.y"
                   { strcpy((yyval.expr).name, (yyvsp[0].strval)); strcpy((yyval.expr).tipo, "chumbo"); }
#line 1824 "y.tab.c"
    break;

  case 41: /* expressao: NUM_REAL  */
#line 461 "translate.y"
                   { strcpy((yyval.expr).name, (yyvsp[0].strval)); strcpy((yyval.expr).tipo, "mercurio"); }
#line 1830 "y.tab.c"
    break;

  case 42: /* expressao: LIT_CHAR  */
#line 462 "translate.y"
                   { strcpy((yyval.expr).name, (yyvsp[0].strval)); strcpy((yyval.expr).tipo, "runa"); }
#line 1836 "y.tab.c"
    break;

  case 43: /* expressao: VAL_VERDADE  */
#line 463 "translate.y"
                   { strcpy((yyval.expr).name, "1"); strcpy((yyval.expr).tipo, "elixir"); }
#line 1842 "y.tab.c"
    break;

  case 44: /* expressao: VAL_FALSO  */
#line 464 "translate.y"
                   { strcpy((yyval.expr).name, "0"); strcpy((yyval.expr).tipo, "elixir"); }
#line 1848 "y.tab.c"
    break;

  case 45: /* expressao: '(' expressao ')'  */
#line 465 "translate.y"
                             { (yyval.expr) = (yyvsp[-1].expr); }
#line 1854 "y.tab.c"
    break;

  case 46: /* expressao: OP_NOT expressao  */
#line 466 "translate.y"
                             { gerar_negacao(&(yyval.expr), (yyvsp[0].expr)); }
#line 1860 "y.tab.c"
    break;

  case 47: /* expressao: expressao OP_OR expressao  */
#line 467 "translate.y"
                                  { gerar_logico(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "||"); }
#line 1866 "y.tab.c"
    break;

  case 48: /* expressao: expressao OP_AND expressao  */
#line 468 "translate.y"
                                  { gerar_logico(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "&&"); }
#line 1872 "y.tab.c"
    break;

  case 49: /* expressao: expressao OP_GT expressao  */
#line 469 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), ">", 1); }
#line 1878 "y.tab.c"
    break;

  case 50: /* expressao: expressao OP_LT expressao  */
#line 470 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "<", 1); }
#line 1884 "y.tab.c"
    break;

  case 51: /* expressao: expressao OP_GE expressao  */
#line 471 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), ">=", 1); }
#line 1890 "y.tab.c"
    break;

  case 52: /* expressao: expressao OP_LE expressao  */
#line 472 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "<=", 1); }
#line 1896 "y.tab.c"
    break;

  case 53: /* expressao: expressao OP_EQ expressao  */
#line 473 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "==", 0); }
#line 1902 "y.tab.c"
    break;

  case 54: /* expressao: expressao OP_NE expressao  */
#line 474 "translate.y"
                                  { gerar_comparacao(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "!=", 0); }
#line 1908 "y.tab.c"
    break;

  case 55: /* expressao: expressao OP_SOMA expressao  */
#line 475 "translate.y"
                                  { gerar_aritmetica(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "+", 0); }
#line 1914 "y.tab.c"
    break;

  case 56: /* expressao: expressao OP_SUB expressao  */
#line 476 "translate.y"
                                  { gerar_aritmetica(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "-", 0); }
#line 1920 "y.tab.c"
    break;

  case 57: /* expressao: expressao OP_MULT expressao  */
#line 477 "translate.y"
                                  { gerar_aritmetica(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "*", 0); }
#line 1926 "y.tab.c"
    break;

  case 58: /* expressao: expressao OP_DIV expressao  */
#line 478 "translate.y"
                                  { gerar_aritmetica(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "/", 0); }
#line 1932 "y.tab.c"
    break;

  case 59: /* expressao: expressao OP_MOD expressao  */
#line 479 "translate.y"
                                  { gerar_aritmetica(&(yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr), "%", 1); }
#line 1938 "y.tab.c"
    break;


#line 1942 "y.tab.c"

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

#line 482 "translate.y"


/* ================================================================== */
/*               IMPLEMENTACAO — GERACAO DE ASSEMBLY MOS6502          */
/* ================================================================== */

unsigned char get_zp_addr(const char *name) {
    int i;
    for (i = 0; i < addr6502_count; i++)
        if (strcmp(addr6502[i].name, name) == 0)
            return addr6502[i].addr;
    strncpy(addr6502[addr6502_count].name, name, 63);
    addr6502[addr6502_count].name[63] = '\0';
    addr6502[addr6502_count].addr = next_zp_addr;
    addr6502_count++;
    return next_zp_addr++;
}

int is_int_literal(const char *s) {
    const char *p = s;
    if (!p || !*p) return 0;
    if (*p == '+' || *p == '-') p++;
    if (!*p) return 0;
    for (; *p; p++) if (*p < '0' || *p > '9') return 0;
    return 1;
}

int is_float_literal(const char *s) {
    const char *p = s;
    int has_dot = 0;
    if (!p || !*p) return 0;
    if (*p == '+' || *p == '-') p++;
    for (; *p; p++) {
        if (*p == '.') { has_dot = 1; continue; }
        if (*p < '0' || *p > '9') return 0;
    }
    return has_dot;
}

int is_char_literal(const char *s) {
    return s && s[0] == '\'';
}

int literal_to_byte(const char *s) {
    if (is_char_literal(s)) return (unsigned char)s[1];
    return (int)atof(s) & 0xFF;
}

void emit_load(FILE *f, const char *operand) {
    if (is_int_literal(operand) || is_char_literal(operand)) {
        fprintf(f, "  LDA #%d\n", literal_to_byte(operand));
    } else if (is_float_literal(operand)) {
        fprintf(f, "  LDA #%d  ; %.10s (float truncado para inteiro)\n",
                literal_to_byte(operand), operand);
    } else {
        fprintf(f, "  LDA $%02X  ; %s\n", get_zp_addr(operand), operand);
    }
}

void emit_cmp_op(FILE *f, const char *operand) {
    if (is_int_literal(operand) || is_float_literal(operand) || is_char_literal(operand))
        fprintf(f, "  CMP #%d\n", literal_to_byte(operand));
    else
        fprintf(f, "  CMP $%02X  ; %s\n", get_zp_addr(operand), operand);
}

void emit_adc_op(FILE *f, const char *operand) {
    if (is_int_literal(operand) || is_float_literal(operand) || is_char_literal(operand))
        fprintf(f, "  ADC #%d\n", literal_to_byte(operand));
    else
        fprintf(f, "  ADC $%02X  ; %s\n", get_zp_addr(operand), operand);
}

void emit_sbc_op(FILE *f, const char *operand) {
    if (is_int_literal(operand) || is_float_literal(operand) || is_char_literal(operand))
        fprintf(f, "  SBC #%d\n", literal_to_byte(operand));
    else
        fprintf(f, "  SBC $%02X  ; %s\n", get_zp_addr(operand), operand);
}

void emit_comparison(FILE *f, const char *dst, const char *a, const char *b, const char *oper) {
    int id = asm_label_id++;
    fprintf(f, "  ; %s = %s %s %s\n", dst, a, oper, b);
    emit_load(f, a);
    emit_cmp_op(f, b);
    if (strcmp(oper, "==") == 0) {
        fprintf(f, "  BEQ _cmp_true_%d\n", id);
    } else if (strcmp(oper, "!=") == 0) {
        fprintf(f, "  BNE _cmp_true_%d\n", id);
    } else if (strcmp(oper, "<") == 0) {
        fprintf(f, "  BCC _cmp_true_%d\n", id);
    } else if (strcmp(oper, ">=") == 0) {
        fprintf(f, "  BCS _cmp_true_%d\n", id);
    } else if (strcmp(oper, ">") == 0) {
        /* a > b: igual => falso, carry set e nao igual => verdadeiro */
        fprintf(f, "  BEQ _cmp_false_%d\n", id);
        fprintf(f, "  BCS _cmp_true_%d\n", id);
    } else if (strcmp(oper, "<=") == 0) {
        /* a <= b: menor ou igual => verdadeiro */
        fprintf(f, "  BCC _cmp_true_%d\n", id);
        fprintf(f, "  BEQ _cmp_true_%d\n", id);
    }
    fprintf(f, "_cmp_false_%d:\n", id);
    fprintf(f, "  LDA #0\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "  JMP _cmp_end_%d\n", id);
    fprintf(f, "_cmp_true_%d:\n", id);
    fprintf(f, "  LDA #1\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "_cmp_end_%d:\n", id);
}

void emit_multiply(FILE *f, const char *dst, const char *a, const char *b) {
    int id = asm_label_id++;
    fprintf(f, "  ; %s = %s * %s (adicao repetida — funciona para valores 0-255)\n", dst, a, b);
    fprintf(f, "  LDA #0\n");
    fprintf(f, "  STA $%02X  ; %s = 0\n", get_zp_addr(dst), dst);
    if (is_int_literal(b) || is_float_literal(b) || is_char_literal(b))
        fprintf(f, "  LDX #%d\n", literal_to_byte(b));
    else
        fprintf(f, "  LDX $%02X  ; %s\n", get_zp_addr(b), b);
    fprintf(f, "_mult_loop_%d:\n", id);
    fprintf(f, "  CPX #0\n");
    fprintf(f, "  BEQ _mult_end_%d\n", id);
    fprintf(f, "  CLC\n");
    fprintf(f, "  LDA $%02X  ; %s\n", get_zp_addr(dst), dst);
    emit_adc_op(f, a);
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "  DEX\n");
    fprintf(f, "  JMP _mult_loop_%d\n", id);
    fprintf(f, "_mult_end_%d:\n", id);
}

void emit_divide(FILE *f, const char *dst, const char *a, const char *b) {
    int id = asm_label_id++;
    unsigned char num_addr = get_zp_addr("__div_num");
    fprintf(f, "  ; %s = %s / %s (subtracao repetida — sem divisao por zero)\n", dst, a, b);
    emit_load(f, a);
    fprintf(f, "  STA $%02X  ; __div_num\n", num_addr);
    fprintf(f, "  LDA #0\n");
    fprintf(f, "  STA $%02X  ; %s = 0\n", get_zp_addr(dst), dst);
    fprintf(f, "_div_loop_%d:\n", id);
    fprintf(f, "  LDA $%02X  ; __div_num\n", num_addr);
    emit_cmp_op(f, b);
    fprintf(f, "  BCC _div_end_%d\n", id);
    fprintf(f, "  SEC\n");
    emit_sbc_op(f, b);
    fprintf(f, "  STA $%02X  ; __div_num\n", num_addr);
    fprintf(f, "  INC $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "  JMP _div_loop_%d\n", id);
    fprintf(f, "_div_end_%d:\n", id);
}

void emit_modulo(FILE *f, const char *dst, const char *a, const char *b) {
    int id = asm_label_id++;
    unsigned char num_addr = get_zp_addr("__mod_num");
    fprintf(f, "  ; %s = %s %% %s (subtracao repetida)\n", dst, a, b);
    emit_load(f, a);
    fprintf(f, "  STA $%02X  ; __mod_num\n", num_addr);
    fprintf(f, "_mod_loop_%d:\n", id);
    fprintf(f, "  LDA $%02X  ; __mod_num\n", num_addr);
    emit_cmp_op(f, b);
    fprintf(f, "  BCC _mod_end_%d\n", id);
    fprintf(f, "  SEC\n");
    emit_sbc_op(f, b);
    fprintf(f, "  STA $%02X  ; __mod_num\n", num_addr);
    fprintf(f, "  JMP _mod_loop_%d\n", id);
    fprintf(f, "_mod_end_%d:\n", id);
    fprintf(f, "  LDA $%02X  ; __mod_num\n", num_addr);
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
}

void emit_and(FILE *f, const char *dst, const char *a, const char *b) {
    int id = asm_label_id++;
    fprintf(f, "  ; %s = %s && %s\n", dst, a, b);
    emit_load(f, a);
    fprintf(f, "  BEQ _and_false_%d\n", id);
    emit_load(f, b);
    fprintf(f, "  BEQ _and_false_%d\n", id);
    fprintf(f, "  LDA #1\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "  JMP _and_end_%d\n", id);
    fprintf(f, "_and_false_%d:\n", id);
    fprintf(f, "  LDA #0\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "_and_end_%d:\n", id);
}

void emit_or(FILE *f, const char *dst, const char *a, const char *b) {
    int id = asm_label_id++;
    fprintf(f, "  ; %s = %s || %s\n", dst, a, b);
    emit_load(f, a);
    fprintf(f, "  BNE _or_true_%d\n", id);
    emit_load(f, b);
    fprintf(f, "  BNE _or_true_%d\n", id);
    fprintf(f, "  LDA #0\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "  JMP _or_end_%d\n", id);
    fprintf(f, "_or_true_%d:\n", id);
    fprintf(f, "  LDA #1\n");
    fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
    fprintf(f, "_or_end_%d:\n", id);
}

void gerar_assembly_6502(const char *filename) {
    FILE *f;
    int i, n;
    char line[TAC_LINE_LEN];
    char dst[64], a[64], b[64], op[8];
    int len;

    f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erro ao criar arquivo de assembly: %s\n", filename);
        return;
    }

    fprintf(f, "; ================================================\n");
    fprintf(f, "; Assembly MOS6502 gerado pela linguagem Alchemia\n");
    fprintf(f, "; Simulador: https://skilldrick.github.io/easy6502/\n");
    fprintf(f, "; Para verificar o resultado, consulte o endereco\n");
    fprintf(f, "; zero-page da variavel desejada no mapa ao final.\n");
    fprintf(f, "; ================================================\n");
    fprintf(f, "*=$0600\n\n");

    for (i = 0; i < tac_count; i++) {
        strncpy(line, tac_buffer[i], TAC_LINE_LEN - 1);
        line[TAC_LINE_LEN - 1] = '\0';
        len = (int)strlen(line);

        /* 1. Rotulo: termina com ':' */
        if (len > 0 && line[len - 1] == ':') {
            fprintf(f, "%s\n", line);
            continue;
        }

        /* 2. goto L */
        if (strncmp(line, "goto ", 5) == 0) {
            fprintf(f, "  JMP %s\n", line + 5);
            continue;
        }

        /* 3. ifFalse t goto L  — salta para L se t == 0 (falso) */
        if (sscanf(line, "ifFalse %63s goto %63s", a, b) == 2) {
            int id = asm_label_id++;
            fprintf(f, "  LDA $%02X  ; %s\n", get_zp_addr(a), a);
            fprintf(f, "  BNE _skpf_%d\n", id);
            fprintf(f, "  JMP %s\n", b);
            fprintf(f, "_skpf_%d:\n", id);
            continue;
        }

        /* 4. if t goto L  — salta para L se t != 0 (verdadeiro) */
        if (strncmp(line, "if ", 3) == 0 &&
            sscanf(line, "if %63s goto %63s", a, b) == 2) {
            int id = asm_label_id++;
            fprintf(f, "  LDA $%02X  ; %s\n", get_zp_addr(a), a);
            fprintf(f, "  BEQ _skpt_%d\n", id);
            fprintf(f, "  JMP %s\n", b);
            fprintf(f, "_skpt_%d:\n", id);
            continue;
        }

        /* 5. dst = ! src */
        if (sscanf(line, "%63s = ! %63s", dst, a) == 2) {
            int id = asm_label_id++;
            fprintf(f, "  ; %s = ! %s\n", dst, a);
            fprintf(f, "  LDA $%02X  ; %s\n", get_zp_addr(a), a);
            fprintf(f, "  BEQ _not_t_%d\n", id);
            fprintf(f, "  LDA #0\n");
            fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
            fprintf(f, "  JMP _not_e_%d\n", id);
            fprintf(f, "_not_t_%d:\n", id);
            fprintf(f, "  LDA #1\n");
            fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
            fprintf(f, "_not_e_%d:\n", id);
            continue;
        }

        /* 6. dst = a OP b */
        n = sscanf(line, "%63s = %63s %7s %63s", dst, a, op, b);
        if (n == 4) {
            if (strcmp(op, "+") == 0) {
                fprintf(f, "  ; %s = %s + %s\n", dst, a, b);
                emit_load(f, a);
                fprintf(f, "  CLC\n");
                emit_adc_op(f, b);
                fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
            } else if (strcmp(op, "-") == 0) {
                fprintf(f, "  ; %s = %s - %s\n", dst, a, b);
                emit_load(f, a);
                fprintf(f, "  SEC\n");
                emit_sbc_op(f, b);
                fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
            } else if (strcmp(op, "*") == 0) {
                emit_multiply(f, dst, a, b);
            } else if (strcmp(op, "/") == 0) {
                emit_divide(f, dst, a, b);
            } else if (strcmp(op, "%") == 0) {
                emit_modulo(f, dst, a, b);
            } else if (strcmp(op, "&&") == 0) {
                emit_and(f, dst, a, b);
            } else if (strcmp(op, "||") == 0) {
                emit_or(f, dst, a, b);
            } else {
                emit_comparison(f, dst, a, b, op);
            }
            continue;
        }

        /* 7. dst = src  (atribuicao simples: variavel ou literal) */
        if (sscanf(line, "%63s = %63s", dst, a) == 2) {
            fprintf(f, "  ; %s = %s\n", dst, a);
            emit_load(f, a);
            fprintf(f, "  STA $%02X  ; %s\n", get_zp_addr(dst), dst);
            continue;
        }

        fprintf(f, "  ; [nao traduzido]: %s\n", line);
    }

    fprintf(f, "\n  BRK\n");

    /* Mapa de enderecos zero-page */
    fprintf(f, "\n; ===================================\n");
    fprintf(f, "; MAPA DE MEMORIA (ZERO PAGE)\n");
    fprintf(f, "; ===================================\n");
    fprintf(f, "; Variaveis do programa:\n");
    for (i = 0; i < addr6502_count; i++) {
        const char *n2 = addr6502[i].name;
        int is_temp = (n2[0] == 't' && n2[1] >= '0' && n2[1] <= '9');
        int is_scratch = (strncmp(n2, "__", 2) == 0);
        if (!is_temp && !is_scratch)
            fprintf(f, ";   $%02X  =  %s\n", addr6502[i].addr, n2);
    }
    fprintf(f, "; Temporarios internos:\n");
    for (i = 0; i < addr6502_count; i++) {
        const char *n2 = addr6502[i].name;
        int is_temp = (n2[0] == 't' && n2[1] >= '0' && n2[1] <= '9');
        int is_scratch = (strncmp(n2, "__", 2) == 0);
        if (is_temp || is_scratch)
            fprintf(f, ";   $%02X  =  %s\n", addr6502[i].addr, n2);
    }

    fclose(f);
}

int inserir_tabela(const char *nome, const char *tipo) {
    int i;

    if (nome == NULL || tipo == NULL) {
        return 0;
    }

    for (i = 0; i < total_simbolos; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            return 1; /* ja existe */
        }
    }

    if (total_simbolos >= 100) {
        fprintf(stderr, "Tabela de simbolos cheia\n");
        exit(1);
    }

    strncpy(tabela[total_simbolos].nome, nome, sizeof(tabela[total_simbolos].nome) - 1);
    tabela[total_simbolos].nome[sizeof(tabela[total_simbolos].nome) - 1] = '\0';

    strncpy(tabela[total_simbolos].tipo, tipo, sizeof(tabela[total_simbolos].tipo) - 1);
    tabela[total_simbolos].tipo[sizeof(tabela[total_simbolos].tipo) - 1] = '\0';

    total_simbolos++;
    return 0;
}

char *getDataType(const char *nome) {
    int i;
    for (i = 0; i < total_simbolos; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            return tabela[i].tipo;
        }
    }
    return NULL;
}

void imprimir_tabela_simbolos(void) {
    int i;

    printf("\n====================================\n");
    printf("        TABELA DE SÍMBOLOS          \n");
    printf("====================================\n");
    printf("%-20s | %-15s\n", "IDENTIFICADOR", "TIPO RECONHECIDO");
    printf("------------------------------------\n");

    for (i = 0; i < total_simbolos; i++) {
        printf("%-20s | %-15s\n", tabela[i].nome, tabela[i].tipo);
    }

    printf("====================================\n");
}

int imprimir_fonte_numerado(const char *nome_arquivo) {
    FILE *arquivo;
    char linha_texto[256];
    int numero_linha = 1;

    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    printf("--- PROGRAMA FONTE NUMERADO ---\n");
    while (fgets(linha_texto, sizeof(linha_texto), arquivo) != NULL) {
        printf("%d: %s", numero_linha, linha_texto);
        numero_linha++;
    }
    printf("\n-------------------------------\n\n");

    fclose(arquivo);
    return 1;
}

void yyerror(const char *s) {
    (void)s;
    fprintf(stderr, "\n>> Erro proximo a linha %d: comando ou expressao invalida. <<\n", linha_atual);
    fprintf(stderr, "Programa sintaticamente incorreto\n");
    exit(1);
}

int main(int argc, char **argv) {
    int i;

    if (argc > 1) {
        if (!imprimir_fonte_numerado(argv[1])) {
            return 1;
        }

        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            printf("Erro ao abrir o arquivo %s\n", argv[1]);
            return 1;
        }
    }

    fflush(stdout);
    yyparse();

    imprimir_tabela_simbolos();
    printf("\nPrograma sintaticamente correto\n");

    if (numSemanticErros > 0) {
        printf("\n=== ANALISE SEMANTICA: %d ERRO(S) ENCONTRADO(S) ===\n", numSemanticErros);
        for (i = 0; i < numSemanticErros; i++) {
            printf("%s\n", semanticErros[i]);
        }
    } else {
        printf("\nAnalise semantica: nenhum erro encontrado.\n");
    }

    imprimir_codigo_intermediario();

    /* Gerar assembly MOS6502 */
    {
        char asm_filename[300] = "output.asm";
        if (argc > 1) {
            char *dot = strrchr(argv[1], '.');
            if (dot) {
                int base_len = (int)(dot - argv[1]);
                if (base_len > 0 && base_len < 290) {
                    strncpy(asm_filename, argv[1], base_len);
                    asm_filename[base_len] = '\0';
                    strcat(asm_filename, ".asm");
                }
            } else {
                snprintf(asm_filename, sizeof(asm_filename), "%s.asm", argv[1]);
            }
        }
        gerar_assembly_6502(asm_filename);
        printf("\nAssembly MOS6502 gerado em: %s\n", asm_filename);
    }

    if (yyin != NULL) {
        fclose(yyin);
    }

    return 0;
}
