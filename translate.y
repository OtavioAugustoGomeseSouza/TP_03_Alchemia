%{
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

%}

%union {
    char *strval;
    struct expr_s {
        char name[64];
        char tipo[20];
    } expr;
}

%{
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
%}

%token <strval> ID NUM_INT NUM_REAL LIT_CHAR VAL_VERDADE VAL_FALSO
%token TIPO_CHUMBO TIPO_MERCURIO TIPO_ELIXIR TIPO_RUNA
%token REAGIR TRANSMUTAR NEUTRALIZAR FACA FIM FERMENTAR DESTILAR CATALISAR
%token ATRIBUICAO
%token OP_SOMA OP_SUB OP_MULT OP_DIV OP_MOD
%token OP_AND OP_OR OP_NOT
%token OP_GT OP_LT OP_GE OP_LE OP_EQ OP_NE

%type <strval> tipo
%type <expr> expressao

%left OP_OR
%left OP_AND
%nonassoc OP_GT OP_LT OP_GE OP_LE OP_EQ OP_NE
%left OP_SOMA OP_SUB
%left OP_MULT OP_DIV OP_MOD
%right OP_NOT

%%

programa:
      lista_declaracoes
    ;

lista_declaracoes:
      lista_declaracoes declaracao
    | /* vazio */
    ;

declaracao:
      declaracao_variavel
    | atribuicao
    | comando_condicional
    | comando_repeticao
    ;

declaracao_variavel:
      tipo ID {
          if (inserir_tabela($2, $1))
              semantic_error(linha_atual, "variavel \"%s\" ja foi declarada", $2);
      }
    | tipo ID ATRIBUICAO expressao {
          if (inserir_tabela($2, $1))
              semantic_error(linha_atual, "variavel \"%s\" ja foi declarada", $2);
          if (!tipos_compativeis($1, $4.tipo))
              semantic_error(linha_atual,
                  "tipos incompativeis na inicializacao de \"%s\": esperado \"%s\", recebido \"%s\"",
                  $2, $1, $4.tipo);
          add_tac("%s = %s", $2, $4.name);
      }
    ;

tipo:
      TIPO_CHUMBO      { $$ = "chumbo"; }
    | TIPO_MERCURIO    { $$ = "mercurio"; }
    | TIPO_ELIXIR      { $$ = "elixir"; }
    | TIPO_RUNA        { $$ = "runa"; }
    ;

atribuicao:
      ID ATRIBUICAO expressao {
          char *tipoVar = getDataType($1);
          if (!tipoVar) {
              semantic_error(linha_atual, "variavel \"%s\" nao foi declarada", $1);
          } else if (!tipos_compativeis(tipoVar, $3.tipo)) {
              semantic_error(linha_atual,
                  "tipos incompativeis na atribuicao a \"%s\": esperado \"%s\", recebido \"%s\"",
                  $1, tipoVar, $3.tipo);
          }
          add_tac("%s = %s", $1, $3.name);
      }
    ;

comando_condicional:
      REAGIR '(' expressao ')'
      {
          verificar_booleano($3);
          char *lelse = novo_label();
          add_tac("ifFalse %s goto %s", $3.name, lelse);
          push_label(lelse);
      }
      FACA lista_declaracoes
      {
          char *lelse = pop_label();
          char *lend = novo_label();
          add_tac("goto %s", lend);
          add_tac("%s:", lelse);
          push_label(lend);
      }
      sub_condicional
      {
          char *lend = pop_label();
          add_tac("%s:", lend);
      }
      FIM REAGIR
    ;

sub_condicional:
      TRANSMUTAR '(' expressao ')'
      {
          verificar_booleano($3);
          char *lelse = novo_label();
          add_tac("ifFalse %s goto %s", $3.name, lelse);
          push_label(lelse);
      }
      FACA lista_declaracoes
      {
          char *lelse = pop_label();
          add_tac("goto %s", peek_label());
          add_tac("%s:", lelse);
      }
      sub_condicional
    | NEUTRALIZAR FACA lista_declaracoes
    | /* vazio */
    ;

comando_repeticao:
      comando_fermentar
    | comando_destilar
    | comando_catalisar
    ;

comando_fermentar:
      FERMENTAR
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
      '(' expressao ')'
      {
          verificar_booleano($4);
          char *lend = novo_label();
          add_tac("ifFalse %s goto %s", $4.name, lend);
          push_label(lend);
      }
      FACA lista_declaracoes
      {
          char *lend = pop_label();
          char *lstart = pop_label();
          add_tac("goto %s", lstart);
          add_tac("%s:", lend);
      }
      FIM FERMENTAR
    ;

comando_destilar:
      DESTILAR '(' atribuicao ';'
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
      expressao ';'
      {
          verificar_booleano($6);
          char *lend = novo_label();
          add_tac("ifFalse %s goto %s", $6.name, lend);
          push_label(lend);
          push_marca(tac_count);
      }
      atribuicao ')'
      {
          int marca = pop_marca();
          mover_para_buffer_temporario(marca);
      }
      FACA lista_declaracoes
      {
          restaurar_buffer_temporario();
          char *lend = pop_label();
          char *lstart = pop_label();
          add_tac("goto %s", lstart);
          add_tac("%s:", lend);
      }
      FIM DESTILAR
    ;

comando_catalisar:
      CATALISAR
      {
          char *lstart = novo_label();
          add_tac("%s:", lstart);
          push_label(lstart);
      }
      FACA lista_declaracoes FIM CATALISAR '(' expressao ')'
      {
          verificar_booleano($8);
          char *lstart = pop_label();
          add_tac("if %s goto %s", $8.name, lstart);
      }
    ;

expressao:
      ID {
          char *t = getDataType($1);
          if (!t) {
              semantic_error(linha_atual, "variavel \"%s\" nao foi declarada", $1);
              t = "chumbo";
          }
          strcpy($$.name, $1);
          strcpy($$.tipo, t);
      }
    | NUM_INT      { strcpy($$.name, $1); strcpy($$.tipo, "chumbo"); }
    | NUM_REAL     { strcpy($$.name, $1); strcpy($$.tipo, "mercurio"); }
    | LIT_CHAR     { strcpy($$.name, $1); strcpy($$.tipo, "runa"); }
    | VAL_VERDADE  { strcpy($$.name, "1"); strcpy($$.tipo, "elixir"); }
    | VAL_FALSO    { strcpy($$.name, "0"); strcpy($$.tipo, "elixir"); }
    | '(' expressao ')'      { $$ = $2; }
    | OP_NOT expressao       { gerar_negacao(&$$, $2); }
    | expressao OP_OR expressao   { gerar_logico(&$$, $1, $3, "||"); }
    | expressao OP_AND expressao  { gerar_logico(&$$, $1, $3, "&&"); }
    | expressao OP_GT expressao   { gerar_comparacao(&$$, $1, $3, ">", 1); }
    | expressao OP_LT expressao   { gerar_comparacao(&$$, $1, $3, "<", 1); }
    | expressao OP_GE expressao   { gerar_comparacao(&$$, $1, $3, ">=", 1); }
    | expressao OP_LE expressao   { gerar_comparacao(&$$, $1, $3, "<=", 1); }
    | expressao OP_EQ expressao   { gerar_comparacao(&$$, $1, $3, "==", 0); }
    | expressao OP_NE expressao   { gerar_comparacao(&$$, $1, $3, "!=", 0); }
    | expressao OP_SOMA expressao { gerar_aritmetica(&$$, $1, $3, "+", 0); }
    | expressao OP_SUB expressao  { gerar_aritmetica(&$$, $1, $3, "-", 0); }
    | expressao OP_MULT expressao { gerar_aritmetica(&$$, $1, $3, "*", 0); }
    | expressao OP_DIV expressao  { gerar_aritmetica(&$$, $1, $3, "/", 0); }
    | expressao OP_MOD expressao  { gerar_aritmetica(&$$, $1, $3, "%", 1); }
    ;

%%

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

    if (yyin != NULL) {
        fclose(yyin);
    }

    return 0;
}
