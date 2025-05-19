#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h> // Incluir para definir FLT_MAX
#include "grafo.h"
#include "brigadistas.h"

grafo* cria_Grafo(int num)
{
    grafo* g = malloc(sizeof(grafo));
    if (g == NULL)
    {
        printf("Erro ao alocar memoria para o grafo!\n");
        return NULL;
    }

    g->nro_vertices = num;

    // Alocar memória para o vetor de vértices
    g->vertice = malloc(num * sizeof(vertice));
    if (g->vertice == NULL)
    {
        printf("Erro ao alocar memoria para os vertices!\n");
        free(g);
        return NULL;
    }

    // Alocar memória para a lista de adjacências
    g->listaAdj = malloc(num * sizeof(no*));
    if (g->listaAdj == NULL)
    {
        printf("Erro ao alocar memoria para a lista de adjacencias!\n");
        free(g->vertice);
        free(g);
        return NULL;
    }

    // Alocar memória para os pesos
    g->pesos = malloc(num * sizeof(float*));
    if (g->pesos == NULL)
    {
        printf("Erro ao alocar memoria para os pesos!\n");
        free(g->listaAdj);
        free(g->vertice);
        free(g);
        return NULL;
    }

    // Inicializar os pesos e a lista de adjacências
    for (int i = 0; i < num; i++)
    {
        g->pesos[i] = malloc(num * sizeof(float));
        if (g->pesos[i] == NULL)
        {
            printf("Erro ao alocar memoria para os pesos da linha %d!\n", i);
            for (int j = 0; j < i; j++)
            {
                free(g->pesos[j]);
            }
            free(g->pesos);
            free(g->listaAdj);
            free(g->vertice);
            free(g);
            return NULL;
        }

        g->listaAdj[i] = NULL; // Inicializa as listas de adjacência como NULL
        // Inicializar a matriz de pesos com FLT_MAX
        for (int j = 0; j < num; j++)
        {
            g->pesos[i][j] = FLT_MAX;
        }
        // Inicializa os vértices
        g->vertice[i].id = i;
        g->vertice[i].temAgua = false;
        g->vertice[i].temPosto = false;
        g->vertice[i].area = 0;
        g->vertice[i].posto = NULL;
        g->vertice[i].temFogo = false;
        g->vertice[i].fogoApagado = false;
        g->vertice[i].jaPegouFogo = false;
    }

    return g;
}


struct no* criarNo(int v)
{
    no* novoNo = malloc(sizeof(no));
    if(novoNo == NULL)
    {
        printf("erro!");
        return NULL;
    }
    novoNo->idVertice = v;
    novoNo->prox = NULL;
    return novoNo;
}

void infoVertice(struct grafo* grafo, int id, bool temAgua, float area)
{
    grafo->vertice[id].area = area;
    grafo->vertice[id].temAgua = temAgua;
}

void adicionaAresta(grafo* grafo, int origem, int destino, float peso)
{
    no *novoNo = criarNo(destino);
    novoNo->prox = grafo->listaAdj[origem];
    grafo->listaAdj[origem] = novoNo;
    grafo->pesos[destino][origem] = peso;


    novoNo = criarNo(origem);
    novoNo->prox = grafo->listaAdj[destino];
    grafo->listaAdj[destino] = novoNo;
    grafo->pesos[origem][destino] = peso;


}
void imprimirGrafo(grafo* gr)
{
    if (!gr) return;

    for (int i = 0; i < gr->nro_vertices; i++)
    {
        printf("Vertice %d [area: %d | agua: %d | posto: %d]", i,
               gr->vertice[i].area,
               gr->vertice[i].temAgua,
               gr->vertice[i].temPosto);

        if (gr->vertice[i].temPosto && gr->vertice[i].posto != NULL)
        {
            postoBrigadistas* p = gr->vertice[i].posto;
            printf("\n  Posto ID: %d", p->id);
            printf("\n  Brigadistas: %d", p->qntBombeiros);
            printf("\n  Caminhao - Velocidade: %.2f km/h | Capacidade: %.2f L",
                   p->caminhao->velocidadeMedia, p->caminhao->capacidadeAgua);
        }

        printf("\n  Conexoes: ");
        no* temp = gr->listaAdj[i];
        if (!temp)
        {
            printf("Nenhuma");
        }
        while (temp)
        {
            printf("-> %d (peso %.2f) ", temp->idVertice, gr->pesos[i][temp->idVertice]);
            temp = temp->prox;
        }
        printf("\n\n");
    }
}


void libera_Grafo(grafo* gr)
{
    if (gr != NULL)
    {
        int i;

        for (i = 0; i < gr->nro_vertices; i++)
        {
            no* atual = gr->listaAdj[i];
            while (atual != NULL)
            {
                no* temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }

        free(gr->listaAdj);

        for (i = 0; i < gr->nro_vertices; i++)
        {
            free(gr->pesos[i]);
        }
        free(gr->pesos);

        for (i = 0; i < gr->nro_vertices; i++)
        {
            if (gr->vertice[i].posto != NULL)
            {
                free(gr->vertice[i].posto);
                free(gr->vertice[i].posto->caminhao);
            }
        }
        free(gr->vertice);

        free(gr);
    }
}
