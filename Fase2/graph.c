#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include <stdbool.h>
#include <math.h>

#define INITIAL_CAPACITY 10 // Capacidade inicial do grafo (qtd de v�rtices)
#define MAX_LINE 128        // Tamanho m�ximo de cada linha do ficheiro

// Cria e inicializa um grafo
Graph* createGraph() {
    Graph* g = malloc(sizeof(Graph));
    // Para resolver warning "C6011 � Desreferenciar de ponteiro NULL"
    // Verifica estado da aloca��o
    if (g == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para grafo\n");
        exit(EXIT_FAILURE);
    }

    // Aloca espa�o para os v�rtices
    g->vertices = malloc(sizeof(Vertex) * INITIAL_CAPACITY);
    if (g->vertices == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para vertices\n");
        free(g);
        exit(EXIT_FAILURE);
    }

    g->size = 0;    // N�mero de v�rtices atual
    g->capacity = INITIAL_CAPACITY; // Capacidade m�xima atual
    return g;
}


// Adiciona um v�rtice ao grafo | resolvido C6308 / C28182 � realloc|
void addVertex(Graph* g, Antenna a) {
    // Realoca espa�o se a capacidade foi atingida
    if (g->size == g->capacity) {
        g->capacity *= 2;
        Vertex* temp = realloc(g->vertices, sizeof(Vertex) * g->capacity);
        if (temp == NULL) {
            fprintf(stderr, "Erro ao realocar memoria\n");
            freeGraph(g);
            exit(EXIT_FAILURE);
        }
        g->vertices = temp;
    }

    // Inicializa o v�rtice com a antena e lista de adjac�ncia vazia
    g->vertices[g->size].antenna = a;
    g->vertices[g->size].adjList = NULL;
    g->size++;
}

// Adiciona uma aresta entre dois v�rtices
void addEdge(Graph* g, int i, int j) {
    Node* n = malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para uma aresta.\n");
        exit(EXIT_FAILURE);
    }
    // O node aponta para a antena do v�rtice j
    n->data = g->vertices[j].antenna;
    n->next = g->vertices[i].adjList;
    g->vertices[i].adjList = n;
}

// Liga v�rticescom a mesma freq
void linkSameFrequency(Graph* g) {
    for (int i = 0; i < g->size; i++) {
        for (int j = i + 1; j < g->size; j++) {
            if (g->vertices[i].antenna.freq == g->vertices[j].antenna.freq) {
                addEdge(g, i, j);
                addEdge(g, j, i);
            }
        }
    }
}

// Carrega grafo a partir do .txt
void loadGraphFromFile(Graph* g, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro");
        return;
    }

    char line[MAX_LINE];
    int y = 0; // linha atual
    while (fgets(line, MAX_LINE, f)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0'; // Remover newlines
        }
        for (int x = 0; line[x] != '\0'; x++) {
            // Reconhece n�meros ou letras como antenas
            char c = line[x];
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                Antenna a = { x, y, c };
                addVertex(g, a);
            }
        }
        y++; // pr�xima linha
    }

    fclose(f);
    linkSameFrequency(g); // liga as antenas da mesma frequ�ncia
}

// Imprime o grafo com todas as conex�es
void printGraph(const Graph* g) {
    printf("Total de antenas: %d\n", g->size);
    for (int i = 0; i < g->size; i++) {
        printf("Antenna %c (%d,%d): ", g->vertices[i].antenna.freq,
            g->vertices[i].antenna.y, g->vertices[i].antenna.x);
        Node* n = g->vertices[i].adjList;
        while (n) {
            printf("-> (%d,%d) ", n->data.y, n->data.x);
            n = n->next;
        }
        printf("\n");
    }
}

// Liberta o grafo da memoria
void freeGraph(Graph* g) {
    for (int i = 0; i < g->size; i++) {
        Node* n = g->vertices[i].adjList;
        while (n) {
            Node* temp = n;
            n = n->next;
            free(temp);
        }
    }
    free(g->vertices);
    free(g);
}

// Procura o indice de um v�rtice pelas coords
int findVertexIndex(Graph* g, char freq, int y, int x) {
    for (int i = 0; i < g->size; i++) {
        Antenna a = g->vertices[i].antenna;
        // Verifica se a antena atual partilha coords
        if (a.freq == freq && a.y == y && a.x == x) {
            return i;
        }
    }
    return -1;
}

// Fun��o auxiliar para DFS
void dfsReach(Graph* g, int index, int* visited) {
    visited[index] = 1; // Marca o v�rtice atual como visitado

    Antenna a = g->vertices[index].antenna;
    printf("Visitada: (%d, %d) [%c]\n", a.y, a.x, a.freq);

    Node* adj = g->vertices[index].adjList; // Lista de adjac�ncia do v�rtice atual
    while (adj) {
        // Procura o �ndice do v�rtice adjacente
        int next = findVertexIndex(g, adj->data.freq, adj->data.x, adj->data.y);

        // Se ainda n�o foi visitado, faz chamada recursiva
        if (next != -1 && !visited[next]) {
            dfsReach(g, next, visited);
        }
        adj = adj->next; // Passa ao pr�ximo vizinho
    }
}

// Fun��o para procura em profundidade (DFS)
void dfs(Graph* g, int startIndex) {
    if (startIndex < 0 || startIndex >= g->size) {
        printf("Indice invalido para procura em profundidade.\n");
        return;
    }

    // Vetor para marcar os v�rtices j� visitados
    int* visited = calloc(g->size, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Erro a alocar memoria para procura em profundidade.\n");
        return;
    }

    Antenna a = g->vertices[startIndex].antenna;
    printf("\n--- Procura em profundidade a partir da antena (%d, %d) [%c] ---\n",
        a.y, a.x, a.freq);

    dfsReach(g, startIndex, visited); // Inicia a DFS

    free(visited); // Liberta mem�ria
}

// Fun��o para procura em largua (BFS)
void bfs(Graph* g, int startIndex) {
    if (startIndex < 0 || startIndex >= g->size) {
        printf("Indice invalido para procura em largura.\n");
        return;
    }

    // Vetor de marcadores de visitado
    int* visited = calloc(g->size, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Erro a alocar memoria para procura em largura.\n");
        return;
    }

    // Fila para armazenar os v�rtices a alcan�ar
    int* q = malloc(sizeof(int) * g->size);
    if (!q) {
        fprintf(stderr, "Erro a alocar fila para procura em largura.\n");
        free(visited);
        return;
    }

    int front = 0; // In�cio da queue
    int rear = 0;  // Fim da queue

    visited[startIndex] = 1;       // Marca o inicial como visitado
    q[rear++] = startIndex;    // Poe o v�rtice inicial na fila

    char freq = g->vertices[startIndex].antenna.freq;

    printf("\n--- procura em largura a partir da antena (%d, %d) [%c] ---\n",
        g->vertices[startIndex].antenna.y,
        g->vertices[startIndex].antenna.x,
        freq);

    // Enquanto houver elementos na fila
    while (front < rear) {
        int current = q[front++]; // Tira da fila
        Antenna a = g->vertices[current].antenna;
        printf("Visitada: (%d, %d) [%c]\n", a.y, a.x, a.freq);

        Node* adj = g->vertices[current].adjList; // Vizinhos
        while (adj) {
            // Encontra o �ndice do vizinho
            int next = findVertexIndex(g, adj->data.freq, adj->data.y, adj->data.x);

            // Se n�o foi visitado e � da mesma frequ�ncia, alcan�a
            if (next != -1 && !visited[next] && g->vertices[next].antenna.freq == freq) {
                visited[next] = 1;
                q[rear++] = next; // P�e na fila para alcanar depois
            }

            adj = adj->next; // Pr�ximo vizinho
        }
    }

    free(q);
    free(visited);
}

// Procura em profundidade (DFS) para encontrar todos os caminhos entre dois v�rtices
void dfsAllPaths(Graph* g, int current, int end, int* visited, int* path, int pathLen) {
    visited[current] = 1;       // Marca como visitado
    path[pathLen++] = current;  // Adiciona ao caminho atual

    if (current == end) {
        // Chegou ao destino, imprime o caminho
        printf("Caminho encontrado:\n");
        for (int i = 0; i < pathLen; i++) {
            Antenna a = g->vertices[path[i]].antenna;
            printf("(%d,%d)", a.x, a.y);
            if (i < pathLen - 1) printf(" -> ");
        }
        printf("\n");
    }
    else {
        Node* adj = g->vertices[current].adjList;
        while (adj) {
            int next = findVertexIndex(g, adj->data.freq, adj->data.y, adj->data.x);
            if (next != -1 && !visited[next]) {
                dfsAllPaths(g, next, end, visited, path, pathLen); // Procura recursivamente
            }
            adj = adj->next;
        }
    }

    visited[current] = 0; // Desmarca como visitado (backtracking)
}

// Wrapper que prepara as estruturas auxiliares para encontrar todos os caminhos
void findAllPaths(Graph* g, int startIndex, int endIndex) {
    if (startIndex < 0 || endIndex < 0 || startIndex >= g->size || endIndex >= g->size) {
        printf("Indices invalidos para caminho.\n");
        return;
    }

    char freq1 = g->vertices[startIndex].antenna.freq;
    char freq2 = g->vertices[endIndex].antenna.freq;

    // S� permite caminhos entre antenas com a mesma frequ�ncia
    if (freq1 != freq2) {
        printf("Antenas com frequencias diferentes (%c != %c), caminho ignorado.\n", freq1, freq2);
        return;
    }

    int* visited = calloc(g->size, sizeof(int));  // Vetor de visitados
    int* path = malloc(sizeof(int) * g->size);    // Vetor para armazenar o caminho

    if (!visited || !path) {
        fprintf(stderr, "Erro a alocar memoria para caminhos.\n");
        free(visited); free(path);
        return;
    }

    printf("\n--- Caminhos de (%d,%d) para (%d,%d) [%c] ---\n",
        g->vertices[startIndex].antenna.y, g->vertices[startIndex].antenna.x,
        g->vertices[endIndex].antenna.y, g->vertices[endIndex].antenna.x,
        freq1);

    dfsAllPaths(g, startIndex, endIndex, visited, path, 0); // Inicia a procura

    free(visited);
    free(path);
}

// Verifica se um valor (c) est� entre outros 2 valores (a e b)
int isBetween(int a, int b, int c) {
    return (a <= c && c <= b) || (b <= c && c <= a);
}

// Verifica se um n�mero � inteiro
int isInteger(double val) {
    return ((int)(val + 0.5) == val);
}

// Encontra interse��es (s� coordenadas inteiras) entre segmentos formados por pares de antenas com frequ�ncias distintas
void findIntersections(Graph* g, char freq1, char freq2) {
    printf("\n--- Intersecoes entre frequencias %c e %c ---\n", freq1, freq2);

    // Vetores tempor�rios para armazenar as antenas de cada frequ�ncia
    Antenna* listA = malloc(sizeof(Antenna) * g->size);
    Antenna* listB = malloc(sizeof(Antenna) * g->size);
    int countA = 0, countB = 0;

    // Separa as antenas da frequ�ncia freq1 e freq2
    for (int i = 0; i < g->size; i++) {
        Antenna a = g->vertices[i].antenna;
        if (a.freq == freq1) listA[countA++] = a;
        if (a.freq == freq2) listB[countB++] = a;
    }

    // Percorre todos os pares de antenas de freq1
    for (int i = 0; i < countA; i++) {
        for (int j = i + 1; j < countA; j++) {
            Antenna a1 = listA[i], a2 = listA[j];

            // E para cada par da freq1, percorre todos os pares de freq2
            for (int m = 0; m < countB; m++) {
                for (int n = m + 1; n < countB; n++) {
                    Antenna b1 = listB[m], b2 = listB[n];

                    // Coordenadas dos pontos extremos dos dois segmentos
                    int x1 = a1.x, y1 = a1.y;
                    int x2 = a2.x, y2 = a2.y;
                    int x3 = b1.x, y3 = b1.y;
                    int x4 = b2.x, y4 = b2.y;

                    // Calcula o denominador da equa��o das retas
                    int denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

                    // Se o denominador for 0, as retas s�o paralelas (n�o se cruzam)
                    if (denom == 0) continue;

                    // F�rmulas para calcular o ponto de interse��o entre duas retas determinantes - �lgebra)
                    double px = ((x1 * y2 - y1 * x2) * (x3 - x4)
                        - (x1 - x2) * (x3 * y4 - y3 * x4)) / (double)denom;
                    double py = ((x1 * y2 - y1 * x2) * (y3 - y4)
                        - (y1 - y2) * (x3 * y4 - y3 * x4)) / (double)denom;

                    // Verifica se o ponto (px, py) � um n�mero inteiro
                    if (isInteger(px) && isInteger(py)) {
                        int ix = (int)round(px);
                        int iy = (int)round(py);

                        // Verifica se a interse��o est� dentro dos dois segmentos (n�o s� nas retas)
                        if (isBetween(x1, x2, ix) && isBetween(y1, y2, iy) &&
                            isBetween(x3, x4, ix) && isBetween(y3, y4, iy)) {
                            // Imprime a interse��o v�lida
                            printf("Intersecao em (%d, %d)\n", iy, ix);
                        }
                    }
                }
            }
        }
    }

    // Liberta os vetores tempor�rios
    free(listA);
    free(listB);
}