# Compilador Alchemia - TP3

Este repositório contém um compilador simples para a linguagem Alchemia, incluindo análise léxica, sintática, semântica e geração de código intermediário em três endereços.

## Estrutura do projeto

- `lex.l` - especificação do analisador léxico (Flex)
- `translate.y` - especificação da gramática e semântica (Bison/Yacc)
- `y.tab.c`, `y.tab.h`, `lex.yy.c` - arquivos gerados pelo Bison/Flex
- `comp` - executável do compilador gerado localmente
- `*.txt` - casos de teste válidos e inválidos da linguagem Alchemia
- `test_results.txt` - saída de execução dos testes atuais

## Como compilar

No diretório do projeto, execute:

```bash
yacc -d translate.y
flex lex.l
gcc y.tab.c lex.yy.c -o comp
```

## Como executar

```bash
./comp nome_do_arquivo.txt
```

O programa imprime:
- fonte numerado
- tabela de símbolos reconhecidos
- resultado da análise semântica
- código intermediário de três endereços

## Tipos suportados

- `chumbo` — inteiro
- `mercurio` — real
- `elixir` — booleano
- `runa` — caractere

## Operadores e tradução

- `FUSAO` → `+`
- `CORROSAO` → `-`
- `CATALISADOR` → `*`
- `EXTRATO` → `/`
- `RESIDUO` → `%`
- `AMALGAMA` → `&&`
- `VAPOR` → `||`
- `OPOSTO` → `!`

Comparações:
- `DENSO` → `>`
- `LEVE` → `<`
- `PURO` → `==`
- `IMPURO` → `!=`
- `DENSO OU PURO` → `>=`
- `LEVE OU PURO` → `<=`

Atribuição: `ID <- expressao`

## Comandos da linguagem Alchemia

### Condicional

- `REAGIR (condicao) FACA ... FIM REAGIR`
- `TRANSMUTAR (condicao) FACA ...` para `else if`
- `NEUTRALIZAR FACA ...` para `else`

### Laços

- `FERMENTAR (condicao) FACA ... FIM FERMENTAR` → enquanto
- `DESTILAR (atribuicao; expressao; atribuicao) FACA ... FIM DESTILAR` → for
- `CATALISAR FACA ... FIM CATALISAR (expressao)` → do-while

## Regras de análise semântica

1. Variáveis devem ser declaradas antes do uso.
2. Atribuição exige tipos compatíveis:
   - mesmo tipo exato
   - `chumbo` pode ser atribuído para `mercurio` (promoção implícita)
3. Operadores aritméticos exigem operandos numéricos (`chumbo` ou `mercurio`).
4. Operador `%` exige operandos do tipo `chumbo`.
5. Operadores lógicos exigem `elixir`.
6. Condições de `REAGIR`, `FERMENTAR`, `DESTILAR` e `CATALISAR` exigem tipo `elixir`.
7. Comparações restritas a tipos numéricos ou ao mesmo tipo, dependendo do operador.

## Geração de código intermediário

O compilador produz código de três endereços com:
- temporários `t0`, `t1`, ...
- rótulos `L0`, `L1`, ...
- instruções de salto `ifFalse <cond> goto <label>` e `goto <label>`
- atribuições simples `x = y`

### Exemplo de saída

```text
Analise semantica: nenhum erro encontrado.

=== CODIGO INTERMEDIARIO (TRES ENDERECOS) ===
    i = 0
    j = 0
    soma = 0
    i = 0
L0:
    t0 = i < 3
    ifFalse t0 goto L1
    j = 0
L2:
    t2 = j < 2
    ifFalse t2 goto L3
    t4 = soma + 1
    soma = t4
    t3 = j + 1
    j = t3
    goto L2
L3:
    t1 = i + 1
    i = t1
    goto L0
L1:
    t5 = soma > 100
    ifFalse t5 goto L4
    soma = 1
    goto L5
L4:
    t6 = soma > 50
    ifFalse t6 goto L6
    soma = 2
    goto L5
L6:
    soma = 3
L5:
==============================================
```

## Testes incluídos

Casos válidos:
- `CATALISAR.txt`
- `DESTILAR.txt`
- `DESTILAR_TP3.txt`
- `FERMENTAR.txt`
- `FUSAO.txt`
- `IMPURO.txt`
- `LACOS_E_CONDICOES_ANINHADOS.txt`
- `REAGIR.txt`
- `TRANSMUTAR.txt`
- `VAPOR.txt`

Casos inválidos:
- `CATALISAR_ERRADA.txt`
- `DESTILAR_ERRADA.txt`
- `FERMENTAR_ERRADA.txt`
- `FUSAO_ERRADA.txt`
- `IMPURO_ERRADA.txt`
- `REAGIR_ERRADA.txt`
- `TRANSMUTAR_ERRADA.txt`
- `VAPOR_ERRADA.txt`

O teste `DESTILAR_TP3.txt` valida o requisito TP3 do laço `DESTILAR` com variável de controle já declarada.

## Observações de TP3

- A implementação atual trata corretamente o passo de atualização do `DESTILAR` após o corpo do laço.
- O caso `LACOS_E_CONDICOES_ANINHADOS.txt` demonstra loops aninhados e cadeia `REAGIR` / `TRANSMUTAR` / `NEUTRALIZAR`.
- `DESTILAR.txt` gera erro semântico quando a variável de controle não foi declarada antes do laço.

## Como gerar documentação de acordo

Esta documentação foi criada a partir da implementação atual de `lex.l` e `translate.y`, além dos testes presentes no repositório.

Para manter a documentação sincronizada com o compilador, atualize as seções de:
- novos comandos ou palavras-chave
- alterações na semântica de tipos
- novos formatos de saída de código intermediário

---

Se desejar, posso também criar um `DOCUMENTACAO_TP3.md` mais detalhado com exemplos de código-fonte e saída em cada comando.
