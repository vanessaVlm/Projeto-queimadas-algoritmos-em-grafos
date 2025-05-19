#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"
#include "brigadistas.h"
#include "fogo.h"
#include "heap.h"
#include "fila.h"
#include <unistd.h>
#define NUM_VERTICES 8
#define BOMBEIROS_POR_POSTO 5
#define VELOCIDADE_CAMINHAO 60.0
#define INTERVALO_PROPAGACAO 2.0
#define AGUA_POR_UNIDADE_AREA 5.0 // Litros de água por unidade de area para apagar

int main()
{
    srand(time(NULL));

    // 1. Criar grafo
    grafo* g = cria_Grafo(NUM_VERTICES);
    if (!g) return 1;

    // 2. Inicializar vertices
    printf("Inicializando vertices...\n");
    for (int i = 0; i < NUM_VERTICES; i++)
    {
        bool temAgua = (rand() % 2 == 0);
        float area = rand() % 100 + 1;
        infoVertice(g, i, temAgua, area);
        printf("Vertice %d: Area %.2f, Agua: %s\n", i, area, g->vertice[i].temAgua ? "Sim" : "Nao");
        usleep(100 * 1000);
    }
    printf("\n");

    // 3. Adicionar arestas
    printf("Adicionando arestas...\n");
    adicionaAresta(g, 0, 1, 12);
    printf("    Aresta 0 <-> 1 (Peso: 12)\n");
    adicionaAresta(g, 0, 2, 4);
    printf("    Aresta 0 <-> 2 (Peso: 4)\n");
    adicionaAresta(g, 1, 3, 10);
    printf("    Aresta 1 <-> 3 (Peso: 10)\n");
    adicionaAresta(g, 2, 3, 2);
    printf("    Aresta 2 <-> 3 (Peso: 2)\n");
    adicionaAresta(g, 3, 4, 3);
    printf("    Aresta 3 <-> 4 (Peso: 3)\n");
    adicionaAresta(g, 4, 5, 6);
    printf("    Aresta 4 <-> 5 (Peso: 6)\n");
    adicionaAresta(g, 5, 6, 8);
    printf("    Aresta 5 <-> 6 (Peso: 8)\n");
    adicionaAresta(g, 5, 7, 3);
    printf("    Aresta 5 <-> 7 (Peso: 3)\n");
    printf("\n");

    // 4. Distribuir postos
    printf("Distribuindo postos de brigadistas...\n");
    distribuirPostos(g, BOMBEIROS_POR_POSTO, VELOCIDADE_CAMINHAO);
    postoBrigadistas* postos[NUM_VERTICES];
    int numPostos = 0;
    for (int i = 0; i < NUM_VERTICES; i++)
    {
        if (g->vertice[i].temPosto)
        {
            postos[numPostos] = g->vertice[i].posto;
            printf("Posto no vertice %d com %d bombeiros e velocidade %.2f km/h.\n",
                   i, g->vertice[i].posto->qntBombeiros, g->vertice[i].posto->caminhao->velocidadeMedia);
            numPostos++;
            usleep(500 * 1000);
        }
    }
    printf("\n");

    // 5. Imprimir grafo
    printf("Grafo:\n");
    imprimirGrafo(g);
    printf("\n");
    sleep(1);

    // Iniciar fogo em um vertice aleatorio (que nao seja um posto)
    int inicioFogo;
    do
    {
        inicioFogo = rand() % NUM_VERTICES;
    }
    while (g->vertice[inicioFogo].temPosto);
    printf("\nIniciando fogo no vertice %d\n", inicioFogo);
    printf("\n");
    g->vertice[inicioFogo].temFogo = true;
    g->vertice[inicioFogo].jaPegouFogo = true;
    sleep(1);

    // Simular o processo de apagar o fogo e sua propagaçao
    simularApagarTodosOsFocos(g, VELOCIDADE_CAMINHAO, INTERVALO_PROPAGACAO, postos);

    // Imprimir vertices salvos
    printf("\nVertices que nao pegaram fogo:\n");
    for (int i = 0; i < NUM_VERTICES; i++)
    {
        if (!g->vertice[i].jaPegouFogo)
        {
            printf("    Vertice %d\n", i);
        }
    }
    printf("\n");

    // 8. Reabastecer postos
    reabastecerAgua(g, postos);

    // 9. Liberar memoria
    libera_Grafo(g);

    return 0;
}
