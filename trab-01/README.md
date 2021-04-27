# Trabalho 1: Quicksort concorrente

Implementação do quicksort (versão de Sedgewick) concorrente.

Executa a versão sequencial e concorrente e mostra os tempos.

```
Nível de recursividade: 10
-----Quicksort Sequencial----------
Gerando vetor de 10000000 elementos!
Ordenando...
Tempo gasto para ordenar: 1.642970
Vetor ordenado com sucesso.


-----Quicksort Concorrente---------
Gerando vetor de 10000000 elementos!
Ordenando...
Tempo gasto para ordenar: 0.558856
Vetor ordenado com sucesso.
```

## Como Compilar

> $ make

## Como executar

> $ ./quicksort <tamanho_do_vetor> <nivel_de_recursividade>

> $ ./quicksort 10000000

> $ ./quicksort 10000000 10

