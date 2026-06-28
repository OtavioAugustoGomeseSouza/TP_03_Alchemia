%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h" // Cabeçalho gerado pelo Bison

int linha_atual = 1;
%}

%option noyywrap

/* Definições regulares originais do vosso grupo */
delim              [ \t\r]
letra              [A-Za-z_]
digito             [0-9]
identificador      {letra}({letra}|{digito})*
inteiro            [+-]?{digito}+
decimal            [+-]?{digito}+"."{digito}+
caractere          \'([^\\\']|\\.)\'
comentario         "||".*
atribuicao         "<-"
ws                 {delim}+

%%

{ws}                { /* ignora espacos e tabs */ }
\n                  { linha_atual++; }

{comentario}        { /* ignora comentarios */ }

"chumbo"            { return TIPO_CHUMBO; }
"mercurio"          { return TIPO_MERCURIO; }
"elixir"            { return TIPO_ELIXIR; }
"runa"              { return TIPO_RUNA; }

"verdade"           { yylval.strval = strdup(yytext); return VAL_VERDADE; }
"falso"             { yylval.strval = strdup(yytext); return VAL_FALSO; }

"REAGIR"            { return REAGIR; }
"TRANSMUTAR"        { return TRANSMUTAR; }
"NEUTRALIZAR"       { return NEUTRALIZAR; }
"FERMENTAR"         { return FERMENTAR; }
"DESTILAR"          { return DESTILAR; }
"CATALISAR"         { return CATALISAR; }

"FACA"              { return FACA; }
"FIM"               { return FIM; }

"FUSAO"             { return OP_SOMA; }
"CORROSAO"          { return OP_SUB; }
"CATALISADOR"       { return OP_MULT; }
"EXTRATO"           { return OP_DIV; }
"RESIDUO"           { return OP_MOD; }

"AMALGAMA"          { return OP_AND; }
"VAPOR"             { return OP_OR; }
"OPOSTO"            { return OP_NOT; }

"DENSO"[ \t]+"OU"[ \t]+"PURO"    { return OP_GE; }
"LEVE"[ \t]+"OU"[ \t]+"PURO"     { return OP_LE; }

"DENSO"             { return OP_GT; }
"LEVE"              { return OP_LT; }
"PURO"              { return OP_EQ; }
"IMPURO"            { return OP_NE; }

{atribuicao}        { return ATRIBUICAO; }
";"                 { return ';'; }
"("                 { return '('; }
")"                 { return ')'; }

{decimal}           { yylval.strval = strdup(yytext); return NUM_REAL; }
{inteiro}           { yylval.strval = strdup(yytext); return NUM_INT; }
{caractere}         { yylval.strval = strdup(yytext); return LIT_CHAR; }

{identificador}     { yylval.strval = strdup(yytext); return ID; }

.                   { printf("Erro lexico na linha %d: caractere invalido '%s'\n", linha_atual, yytext); exit(1); }

%%
