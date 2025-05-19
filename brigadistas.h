#ifndef BRIGADISTAS_H_INCLUDED
#define BRIGADISTAS_H_INCLUDED

typedef struct grafo grafo;

typedef struct postoBrigadistas
{
    int id;
    int qntBombeiros;
    int idVertice;
    struct caminhao* caminhao;
    bool utilizadoNestaIteracao;
} postoBrigadistas;

typedef struct caminhao
{
    int id;
    float velocidadeMedia;
    float capacidadeAgua;
} caminhao;

typedef struct
{
    float* dist;
    float* somaTempos;
    int* anterior;
} CaminhoMinimoTempo;


postoBrigadistas* criarPosto(int idVertice, int qntBombeiros, float velocidade);
bool existe(int guardaVerticeNum[],int tam, int valor);
void distribuirPostos(grafo* grafo, int qntBombeiros, float velocidade);
void liberarPosto(postoBrigadistas* posto);
CaminhoMinimoTempo* encontrarCaminhoComTempo(grafo* grafo, int verticeOrigem, float velocidadeBrigadista);
void simularApagarTodosOsFocos(grafo* grafo, float velocidadeBrigadista, float intervaloPropagacao, postoBrigadistas* postos[]);
void reabastecerAguaPosto(grafo* grafo, postoBrigadistas* posto);
void reabastecerAgua(grafo* grafo, postoBrigadistas* postos[]);




#endif // BRIGADISTAS_H_INCLUDED
