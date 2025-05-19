#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED
#include <stdbool.h>

typedef struct postoBrigadistas postoBrigadistas;

typedef struct grafo
{
    int nro_vertices;
    struct vertice* vertice;
    float** pesos;
    struct no** listaAdj;
} grafo;

typedef struct vertice
{
    int id;
    bool temPosto;
    bool temAgua;
    int area;
    postoBrigadistas* posto;
    bool temFogo;
    bool fogoApagado;
    bool jaPegouFogo;
} vertice;

typedef struct no
{
    int idVertice;
    struct no* prox;
} no;

grafo* cria_Grafo(int num);
struct no* criarNo(int v);
void infoVertice(grafo* gr,int id, bool temAgua,float area);
void adicionaAresta(grafo* gr, int origem, int destino, float peso);
void imprimirGrafo(grafo* gr);
void libera_Grafo(grafo* gr);






#endif // GRAFO_H_INCLUDED
