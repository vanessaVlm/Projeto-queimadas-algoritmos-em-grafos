#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED

typedef struct Fila
{
    int* itens;
    int inicio;
    int fim;
} fila;
fila* criarFila(int tamanho);
int estaVazia(fila* f);
void enfileirar(fila* f, int valor);
int desenfileirar(fila* f);



#endif // FILA_H_INCLUDED
