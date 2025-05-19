# Simulação de Combate a Incêndios Florestais em C

Este projeto simula o combate a incêndios florestais usando C, representando a floresta como um grafo e a ação de brigadistas.

## Arquivos do Projeto

* `main.c`, `grafo.{h,c}`, `brigadistas.{h,c}`, `fogo.{h,c}`, `heap.{h,c}`, `fila.{h,c}`
  
## Vídeos Explicativos

- [Apresentação Carina](https://youtu.be/1EbjVQXZHNY?feature=shared)
- [Apresentação Vanessa](https://youtu.be/Vb6ajHqgrsM?si=XIdOrYPwxa5UuM_L)
  
## Como Executar (GitHub com Projeto Code::Blocks)

**Opção 1: Usando o Code::Blocks**

1.  Clone o repositório.
2.  Abra o arquivo `.cbp` no Code::Blocks.
3.  Clique em `Build` -> `Build` para compilar.
4.  Clique em `Build` -> `Run` para executar.

**Opção 2: Compilando no Terminal**

1.  Clone o repositório e navegue até a pasta do projeto.
2.  Compile com GCC:
    ```bash
    gcc main.c grafo.c brigadistas.c fogo.c heap.c fila.c -o simulacao
    ```
3.  Execute:
    ```bash
    ./simulacao
    ```
    (ou `simulacao.exe` no Windows)

## Contribuições

Pull Requests são bem-vindos.

