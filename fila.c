#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

fila* criarFila(int tamanho)
{
    fila* f = malloc(sizeof(fila));
    f->itens = malloc(tamanho * sizeof(int));
    f->inicio = -1;
    f->fim = -1;
    return f;
}
int estaVazia(fila* f)
{
    return f->inicio == -1;
}
void enfileirar(fila* f, int valor)
{
    if (f->fim == -1)
    {
        f->inicio = 0;
        f->fim = 0;
    }
    else
    {
        f->fim++;
    }
    f->itens[f->fim] = valor;
}

int desenfileirar(fila* f)
{
    int item = f->itens[f->inicio];
    if (f->inicio == f->fim)
    {
        f->inicio = -1;
        f->fim = -1;
    }
    else
    {
        f->inicio++;
    }
    return item;
}
