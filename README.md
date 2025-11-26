# Sistema de linhas de ônibus
## Objetivo:
Sistemas de paradas de ônibus usando listas duplamente encadeadas e simplesmente encadeadas.

## Arquitetura:
O programa funciona como uma maquina de estado. O programa é dividido em 4 partes chamadas de Manager, Handles, Pages, Logic. O Manager é um inicializador e um gerenciador de Handles, faz chamadas para inicializar a tela e arquivos e chama um handle baseado em um estado. Os handles são funções onde a logica da aplicação acontece fazendo chamadas das funções do Logic e tratando os resultados das Pages. As pages são funções que sabem como desenhar na tela e tratam da entrada do usuário retornando um resultado para o handle. O Manager é unico nesse programa e cada Handle tem sua respectiva Page. A Logic ou a lógica de negócio e modelo é a parte onde ocorre a manipulação das listas e dados expondo essas funções aos handles.

## Estruturas:
- Lista duplamente encadeada
- Lista simplismente encadeada

## Bibliotecas:
- Ncurses
- Abstrações proprias

## Como compilar:
### Bibliotecas:
Primeiro é necessário baixar a biblioteca ncurses.
### Para compilar:
Abra a pasta do projeto onde o manager.c está e execute o seguinte comando:
