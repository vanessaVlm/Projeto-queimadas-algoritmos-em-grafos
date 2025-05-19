#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Para usar a funçao rand()
#include "fogo.h"
#include "grafo.h"
#include "fila.h"

#define CHANCE_PROPAGACAO 0.7 // 70% de chance de o fogo se propagar

void propagarFogo(grafo *grafo, int inicio)
{
    fila* f = criarFila(grafo->nro_vertices);
    bool* visitado = malloc(sizeof(bool) * grafo->nro_vertices);
    for (int i = 0; i < grafo->nro_vertices; i++)
    {
        visitado[i] = false;
    }
    srand(time(NULL));

    if (inicio >= 0 && inicio < grafo->nro_vertices && grafo->vertice[inicio].temFogo && !grafo->vertice[inicio].fogoApagado)
    {
        enfileirar(f, inicio);
        visitado[inicio] = true;
    }

    while (!estaVazia(f))
    {
        int atual = desenfileirar(f);

        if (grafo->vertice[atual].fogoApagado)
        {
            continue;
        }

        no* temp = grafo->listaAdj[atual];
        while (temp)
        {
            int adj = temp->idVertice;
            if (adj >= 0 && adj < grafo->nro_vertices && !grafo->vertice[adj].temFogo && !grafo->vertice[adj].fogoApagado && !visitado[adj] && !grafo->vertice[adj].temPosto)   // Adicionada verificação para posto
            {
                if ((float)rand() / RAND_MAX < CHANCE_PROPAGACAO)
                {
                    grafo->vertice[adj].temFogo = true;
                    grafo->vertice[adj].jaPegouFogo = true;
                    printf("Fogo tentou se propagar do vertice %d para o vertice %d (sucesso).\n", atual, adj);
                    enfileirar(f, adj);
                    visitado[adj] = true;
                }
                else
                {
                    printf("Fogo tentou se propagar do vertice %d para o vertice %d (falhou).\n", atual, adj);
                }
            }
            temp = temp->prox;
        }
    }
    free(f->itens);
    free(f);
    free(visitado);
}



