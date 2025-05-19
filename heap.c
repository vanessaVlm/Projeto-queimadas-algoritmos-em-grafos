#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"

MinHeap *criarHeap(int capacidade) {
    MinHeap *H;
    H = malloc(sizeof(MinHeap));
    if (H == NULL) {
        perror("Erro ao alocar memoria para MinHeap");
        return NULL;
    }
    H->tamanho = 0;
    H->capacidade = capacidade;
    H->dados = malloc(capacidade * sizeof(ElementoHeap));
    H->posicao = malloc(capacidade * sizeof(int));
    if (H->dados == NULL || H->posicao == NULL) {
        perror("Erro ao alocar memoria para dados/posicao do heap");
        free(H->dados);
        free(H->posicao);
        free(H);
        return NULL;
    }
    for (int i = 0; i < capacidade; i++) {
        H->posicao[i] = -1;
    }
    return H;
}

void inicializaHeap(MinHeap* h, int quant) {
    if (h != NULL) {
        h->tamanho = 0;
        for (int i = 0; i < h->capacidade; i++) {
            h->posicao[i] = -1;
        }
    }
}

void trocar(ElementoHeap* a, ElementoHeap* b) {
    ElementoHeap temp = *a;
    *a = *b;
    *b = temp;
}

void subir(MinHeap* h, int i) {
    while (i > 0 && h->dados[i].custo < h->dados[(i - 1) / 2].custo) {
        h->posicao[h->dados[i].v] = (i - 1) / 2;
        h->posicao[h->dados[(i - 1) / 2].v] = i;
        trocar(&h->dados[i], &h->dados[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void descer(MinHeap* h, int i) {
    int menor = i, e = 2 * i + 1, d = 2 * i + 2;

    if (e < h->tamanho && h->dados[e].custo < h->dados[menor].custo)
        menor = e;
    if (d < h->tamanho && h->dados[d].custo < h->dados[menor].custo)
        menor = d;

    if (menor != i) {
        h->posicao[h->dados[i].v] = menor;
        h->posicao[h->dados[menor].v] = i;
        trocar(&h->dados[i], &h->dados[menor]);
        descer(h, menor);
    }
}

int insereHeap(MinHeap *H, int valor, int dist) {
    if (H == NULL) {
        printf("Erro: Heap nao inicializado.\n");
        return 0;
    }
    if (H->tamanho == H->capacidade) {
        printf("Erro: Heap cheio!\n");
        return 0;
    }
    /* insere na primeira posição livre */
    H->dados[H->tamanho].v = valor;
    H->dados[H->tamanho].custo = dist;
    H->posicao[valor] = H->tamanho; // Atualiza a posição do vértice
    /* desloca elemento para posição correta */
    subir(H, H->tamanho);
    /* incrementa número de elementos no heap */
    H->tamanho++;
    return 1;
}

ElementoHeap remover(MinHeap* h) {
    if (h == NULL || h->tamanho == 0) {
        ElementoHeap vazio = {-1, -1}; // Retorna um elemento inválido
        printf("Erro: Heap vazio.\n");
        return vazio;
    }
    ElementoHeap min = h->dados[0];
    h->posicao[min.v] = -1; // Remove a posição do elemento removido
    h->dados[0] = h->dados[h->tamanho - 1];
    if (h->tamanho > 0) {
        h->posicao[h->dados[0].v] = 0;
    }
    h->tamanho--;
    descer(h, 0);
    return min;
}

void atualizaHeap(MinHeap* h, int v, int novoCusto) {
    if (h == NULL || h->posicao[v] == -1) {
        printf("Erro: Vertice nao encontrado no heap.\n");
        return;
    }
    int i = h->posicao[v];
    h->dados[i].custo = novoCusto;
    subir(h, i);
}

void liberaHeap(MinHeap* H) {
    if (H != NULL) {
        free(H->dados);
        free(H->posicao);
        free(H);
    }
}
