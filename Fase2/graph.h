#pragma once
#ifndef GRAPH_H
#define GRAPH_H

// Estrutura de antena com posição (x, y) e uma frequência (char)
typedef struct Antenna {
    int x, y;
    char freq;
} Antenna;

// Node da lista de adjacência, que representa as conexões (arestas) entre antenas
typedef struct Node {
    Antenna data;
    struct Node* next; // Próximo nó da lista (próxima ligacao entre antenas)
} Node;

// Estrutura de vértice que armazena uma antena e a sua lista de adjacência
typedef struct Vertex {
    Antenna antenna; // Antena do vértice atual
    Node* adjList;   // Lista ligada de antenas conectadas (arestas) || Lista de adjacência
} Vertex;

// Estrutura do grafo
typedef struct Graph {
    Vertex* vertices; // Vetor de vértices
    int size;         // tamanho do grafo (vertices)
    int capacity;     // Capacidade máxima alocada (para vértices)
} Graph;


Graph* createGraph();
void freeGraph(Graph* g);
void loadGraphFromFile(Graph* g, const char* filename);
void printGraph(const Graph* g);
int findVertexIndex(Graph* g, char freq, int x, int y);
void dfs(Graph* g, int startIndex);
void bfs(Graph* g, int startIndex);
void findAllPaths(Graph* g, int startIndex, int endIndex);
void findIntersections(Graph* g, char freq1, char freq2);

#endif
