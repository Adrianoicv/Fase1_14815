#include <stdio.h>
#include "graph.h"

int main() {
    // Cria o grafo vazio
    Graph* g = createGraph();

    // Carrega o grafo a partir do ficheiro "input.txt"
    loadGraphFromFile(g, "input.txt");

    // Exemplos de uso

    /*
    printf("\n--- GRAFO CARREGADO ---\n");
    printGraph(g); // Imprime o grafo com todas as conexões

    // Procura em profundidade (DFS) a partir de uam antena
    int startIndex = findVertexIndex(g, '0', 4, 4);
    if (startIndex != -1) {
        dfs(g, startIndex);
    }
    else {
        printf("Antena nao encontrada.\n");
    }

    // Procura em largura (BFS) a partir de uma antena
    startIndex = findVertexIndex(g, '0', 4, 4);
    if (startIndex != -1) {
        bfs(g, startIndex);
    }
    else {
        printf("Antena nao encontrada.\n");
    }

    // Procura todos os caminhos possíveis entre duas antenas
    int a = findVertexIndex(g, 'A', 5, 6);
    int b = findVertexIndex(g, 'A', 9, 9);
    findAllPaths(g, a, b);
    */

    // Executa a procura de interseções entre segmentos formados por antenas 'A' e 'B'
    findIntersections(g, 'A', 'B');

    // Liberta toda a memória associada ao grafo
    freeGraph(g);

    return 0; // Termina o programa com sucesso
}
