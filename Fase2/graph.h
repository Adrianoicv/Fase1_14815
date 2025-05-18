#pragma once
#ifndef GRAPH_H
#define GRAPH_H

// Estrutura de antena com posi��o (x, y) e uma frequ�ncia (char)
typedef struct Antenna {
    int x, y;
    char freq;
} Antenna;

// Node da lista de adjac�ncia, que representa as conex�es (arestas) entre antenas
typedef struct Node {
    Antenna data;
    struct Node* next; // Pr�ximo n� da lista (pr�xima ligacao entre antenas)
} Node;

// Estrutura de v�rtice que armazena uma antena e a sua lista de adjac�ncia
typedef struct Vertex {
    Antenna antenna; // Antena do v�rtice atual
    Node* adjList;   // Lista ligada de antenas conectadas (arestas) || Lista de adjac�ncia
} Vertex;

// Estrutura do grafo
typedef struct Graph {
    Vertex* vertices; // Vetor de v�rtices
    int size;         // tamanho do grafo (vertices)
    int capacity;     // Capacidade m�xima alocada (para v�rtices)
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
