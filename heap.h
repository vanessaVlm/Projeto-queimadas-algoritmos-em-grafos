#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef struct {
    int v;
    int custo;
} ElementoHeap;

typedef struct {
    ElementoHeap* dados;
    int* posicao;
    int tamanho;
    int capacidade; // Adicionado capacidade
} MinHeap;

MinHeap *criarHeap(int capacidade); // Modificado para receber capacidade
int insereHeap(MinHeap *H, int valor, int dist);
void inicializaHeap(MinHeap* h, int quant);
void trocar(ElementoHeap* a, ElementoHeap* b);
void subir(MinHeap* h, int i);
void descer(MinHeap* h, int i);
ElementoHeap remover(MinHeap* h);
void atualizaHeap(MinHeap* h, int v, int novoCusto);
void liberaHeap(MinHeap *H);

#endif // HEAP_H_INCLUDED
