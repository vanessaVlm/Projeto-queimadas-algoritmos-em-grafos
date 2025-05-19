#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>
#include "brigadistas.h"
#include "grafo.h"
#include "heap.h"
#include "fogo.h"
#include "fila.h"
#define CAP 4000 //capacidade de agua do caminhao
#define AGUA_POR_UNIDADE_AREA 5.0 // Litros de agua por unidade de area para apagar


postoBrigadistas* criarPosto(int idVertice, int qntBombeiros, float velocidade)
{
    postoBrigadistas* posto = malloc(sizeof(postoBrigadistas));
    if(posto == NULL)
    {
        printf("erro!");
        return NULL;
    }
    posto->id = idVertice;
    posto->idVertice = idVertice;
    posto->qntBombeiros = qntBombeiros;
    posto->utilizadoNestaIteracao = false;

    posto->caminhao = malloc(sizeof(caminhao));
    if (!posto->caminhao)
    {
        free(posto);
        return NULL;
    }

    posto->caminhao->velocidadeMedia = velocidade;
    posto->caminhao->capacidadeAgua = CAP;

    return posto;
}



bool existe(int guardaVerticeNum[], int tam, int valor)
{
    int i;
    for(i = 0; i < tam; i++)
    {
        if(guardaVerticeNum[i] == valor)
        {
            return true;
        }
    }
    return false;
}

void distribuirPostos(grafo* grafo, int qnt, float velocidade)
{
    srand(time(NULL));
    int verticesEscolhidos[3];
    int num_posto = 0;

    while(num_posto < 3)
    {
        int valorAleatorio = rand() % grafo->nro_vertices;
        if (!existe(verticesEscolhidos, num_posto, valorAleatorio))
        {
            verticesEscolhidos[num_posto] = valorAleatorio;
            num_posto++;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        int id = verticesEscolhidos[i];
        grafo->vertice[id].temPosto = true;
        grafo->vertice[id].posto = criarPosto(id, qnt, velocidade);
    }
    printf("Postos distribuidos nos vertices: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", verticesEscolhidos[i]);
    }
    printf("\n");


}
CaminhoMinimoTempo* encontrarCaminhoComTempo(grafo* grafo, int verticeOrigem, float velocidadeBrigadista)
{
    int n = grafo->nro_vertices;
    float* dist = malloc(sizeof(float) * n);
    float* somaTempos = malloc(sizeof(float) * n);
    int* anterior = malloc(sizeof(int) * n);
    bool* visitado = malloc(sizeof(bool) * n);

    MinHeap* H = criarHeap(n);
    if (H == NULL)
    {
        printf("Erro ao criar heap.\n");
        free(dist);
        free(somaTempos);
        free(anterior);
        free(visitado);
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        dist[i] = FLT_MAX;
        somaTempos[i] = FLT_MAX;
        anterior[i] = -1;
        visitado[i] = false;
    }

    dist[verticeOrigem] = 0;
    somaTempos[verticeOrigem] = 0;
    insereHeap(H, verticeOrigem, 0);

    while (H->tamanho != 0)
    {
        ElementoHeap atual = remover(H);
        int u = atual.v;
        visitado[u] = true;

        no* adj = grafo->listaAdj[u];
        while (adj != NULL)
        {
            int v = adj->idVertice;
            float tempoAresta = (float)grafo->pesos[u][v] / velocidadeBrigadista;

            if (!visitado[v] && dist[u] + tempoAresta < dist[v])
            {
                dist[v] = dist[u] + tempoAresta;
                somaTempos[v] = somaTempos[u] + tempoAresta; // Soma dos tempos ate o vizinho
                anterior[v] = u;
                if (H->posicao[v] == -1)
                    insereHeap(H, v, dist[v]);
                else
                    atualizaHeap(H, v, dist[v]);
            }
            adj = adj->prox;
        }
    }

    liberaHeap(H);
    free(visitado);

    CaminhoMinimoTempo* resultado = malloc(sizeof(CaminhoMinimoTempo));
    resultado->dist = dist;
    resultado->somaTempos = somaTempos;
    resultado->anterior = anterior;
    return resultado;
}

void simularApagarTodosOsFocos(grafo* grafo, float velocidadeBrigadista, float intervaloPropagacao, postoBrigadistas* postos[])
{
    bool incendioAtivo = true;
    float tempoTotalDeslocamento = 0.0;
    float tempoSimulado = 0.0;
    int brigadistasDisponiveis = 3;

    while (incendioAtivo)
    {
        incendioAtivo = false;


        printf("Propagando o fogo\n");
        printf("\n");

        int novosFocos = 0;
        bool* propagouNestaIteracao = calloc(grafo->nro_vertices, sizeof(bool));
        for (int i = 0; i < grafo->nro_vertices; i++)
        {
            if (grafo->vertice[i].temFogo && !grafo->vertice[i].fogoApagado && !propagouNestaIteracao[i])
            {
                int focosAnteriores = 0;
                for (int j = 0; j < grafo->nro_vertices; j++)
                {
                    if (grafo->vertice[j].temFogo && !grafo->vertice[j].fogoApagado) focosAnteriores++;
                }
                propagarFogo(grafo, i);
                propagouNestaIteracao[i] = true;
                int focosAtuais = 0;
                for (int j = 0; j < grafo->nro_vertices; j++)
                {
                    if (grafo->vertice[j].temFogo && !grafo->vertice[j].fogoApagado) focosAtuais++;
                }
                novosFocos += (focosAtuais - focosAnteriores);
            }
        }
        free(propagouNestaIteracao);
        if (novosFocos > 0) printf(" %d novos focos de incendio se iniciaram.\n", novosFocos);
        else printf("Nenhum novo foco de incendio.\n");

        printf("Despachando brigadistas\n");
        printf("\n");

        int brigadistasUsadosNestaIteracao = 0;
        for (int i = 0; i < grafo->nro_vertices && brigadistasUsadosNestaIteracao < brigadistasDisponiveis; i++)
        {
            if (grafo->vertice[i].temFogo && !grafo->vertice[i].fogoApagado)
            {
                float menorTempoChegada = FLT_MAX;
                int postoEscolhidoIndex = -1;
                CaminhoMinimoTempo* melhorCaminho = NULL;
                float aguaNecessaria = grafo->vertice[i].area * AGUA_POR_UNIDADE_AREA;

                printf("Fogo no vertice %d (agua necessaria: %.2f).\n",i, aguaNecessaria);

                for (int j = 0; j < 3; j++)
                {
                    if (postos[j]->caminhao->capacidadeAgua >= aguaNecessaria)
                    {
                        CaminhoMinimoTempo* caminho = encontrarCaminhoComTempo(grafo, postos[j]->idVertice, velocidadeBrigadista);
                        if (caminho && caminho->dist[i] < menorTempoChegada)
                        {
                            menorTempoChegada = caminho->dist[i];
                            postoEscolhidoIndex = j;
                            if (melhorCaminho)
                            {
                                free(melhorCaminho->anterior);
                                free(melhorCaminho->dist);
                                free(melhorCaminho->somaTempos);
                                free(melhorCaminho);
                            }
                            melhorCaminho = caminho;
                        }
                        else if (caminho)
                        {
                            free(caminho->anterior);
                            free(caminho->dist);
                            free(caminho->somaTempos);
                            free(caminho);
                        }
                    }
                    else
                    {
                        printf("Posto %d (Vertice %d) com pouca agua (%.2f) para o vertice %d.\n",
                               postos[j]->id, postos[j]->idVertice, postos[j]->caminhao->capacidadeAgua, i);
                    }
                }

                if (postoEscolhidoIndex != -1 && melhorCaminho && melhorCaminho->anterior[i] != -1 && melhorCaminho->dist[i] <= intervaloPropagacao)
                {
                    printf("Posto %d (vertice %d) a caminho do vertice %d (tempo: %.3f, agua disponivel: %.2f, necessaria: %.2f).\n",
                           postos[postoEscolhidoIndex]->id, postos[postoEscolhidoIndex]->idVertice, i, melhorCaminho->dist[i],
                           postos[postoEscolhidoIndex]->caminhao->capacidadeAgua, aguaNecessaria);
                    printf("\n");


                    // Imprimir o caminho percorrido
                    printf("Caminho percorrido: ");
                    int atual = i;
                    while (atual != -1)
                    {
                        printf("%d", atual);
                        if (melhorCaminho->anterior[atual] != -1)
                        {
                            printf(" <- ");
                        }
                        atual = melhorCaminho->anterior[atual];
                    }
                    printf("\n");

                    postos[postoEscolhidoIndex]->caminhao->capacidadeAgua -= aguaNecessaria;
                    printf("Fogo no vertice %d apagado pelo posto %d (vertice %d). agua restante: %.2f.\n",
                           i, postos[postoEscolhidoIndex]->id, postos[postoEscolhidoIndex]->idVertice, postos[postoEscolhidoIndex]->caminhao->capacidadeAgua);
                    grafo->vertice[i].temFogo = false;
                    grafo->vertice[i].fogoApagado = true;
                    tempoTotalDeslocamento += melhorCaminho->dist[i];
                    postos[postoEscolhidoIndex]->utilizadoNestaIteracao = true;
                    brigadistasUsadosNestaIteracao++;
                }
                else if (grafo->vertice[i].temFogo && !grafo->vertice[i].fogoApagado)
                {
                    if (melhorCaminho && melhorCaminho->anterior[i] != -1)
                    {
                        printf("Fogo ativo no vertice %d (tempo para chegar: %.3f).\n", i, melhorCaminho->dist[i]);
                    }
                    else
                    {
                        printf("Fogo ativo no vertice %d (sem posto disponivel ou dentro do tempo).\n", i);
                    }
                }

                if (melhorCaminho)
                {
                    free(melhorCaminho->anterior);
                    free(melhorCaminho->dist);
                    free(melhorCaminho->somaTempos);
                    free(melhorCaminho);
                }
            }
        }

        tempoSimulado += intervaloPropagacao;

        for (int i = 0; i < grafo->nro_vertices; i++)
        {
            if (grafo->vertice[i].temFogo && !grafo->vertice[i].fogoApagado)
            {
                incendioAtivo = true;
                break;
            }
        }
        if (!incendioAtivo)
        {
            printf("\nTodos os focos de incendio foram apagados.\n");
            printf("\nTempo total de deslocamento dos brigadistas: %.3f unidades.\n", tempoTotalDeslocamento);
        }
    }
}

void reabastecerAguaPosto(grafo* grafo, postoBrigadistas* posto)
{
    int idPosto = posto->idVertice;
    CaminhoMinimoTempo* caminhoReabastecimento = encontrarCaminhoComTempo(grafo, idPosto, posto->caminhao->velocidadeMedia);
    float menorTempo = FLT_MAX;
    int verticeComAgua = -1;

    for (int j = 0; j < grafo->nro_vertices; j++)
    {
        if (grafo->vertice[j].temAgua && caminhoReabastecimento && caminhoReabastecimento->dist[j] < menorTempo)
        {
            menorTempo = caminhoReabastecimento->dist[j];
            verticeComAgua = j;
        }
    }

    if (verticeComAgua != -1)
    {
        printf("Posto %d (vertice %d) indo reabastecer no vertice %d.\n",posto->id, idPosto, verticeComAgua);
        // Simula o tempo de viagem para reabastecer
        float tempoReabastecimentoTotal = 2 * menorTempo;
        posto->caminhao->capacidadeAgua = CAP;
    }
    else
    {
        printf("Posto %d (vertice %d) nao conseguiu reabastecer: nenhum local com agua alcancavel.\n",
               posto->id, idPosto);
    }

    if (caminhoReabastecimento)
    {
        free(caminhoReabastecimento->anterior);
        free(caminhoReabastecimento->dist);
        free(caminhoReabastecimento->somaTempos);
        free(caminhoReabastecimento);
    }
}

void reabastecerAgua(grafo* grafo, postoBrigadistas* postos[])
{
    printf("\nReabastecendo os postos que apagaram os fogos...\n");
    for (int i = 0; i < 3; i++)
    {
        if (postos[i]->utilizadoNestaIteracao)
        {
            reabastecerAguaPosto(grafo, postos[i]);
        }
        else
        {
            printf("Posto %d (vertice %d) nao precisou reabastecer.\n", postos[i]->id, postos[i]->idVertice);
        }
    }
}






