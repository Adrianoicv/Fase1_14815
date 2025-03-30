#include <stdio.h>
#include <stdlib.h>
#include "ant_list.h"
#define MATRIZ_IN "matriz_in.txt" // nome do ficheiro com a matriz

// Insere nova antena no inicio da lista
Antenna* insert_ant(Antenna* head, char freq, int line, int col) {

    // Aloca memoria para um novo node da lista
    Antenna* new = (Antenna*)malloc(sizeof(Antenna)); 

    // Se der erro a alocar, exit com mnesagem de erro
    if (!new) {
        fprintf(stderr, "Erro a alocar memória.\n");
        exit(1);
    }

    // Preenche os campos do novo node
    new->freq = freq;
    new->line = line;
    new->col = col;
    new->next = head; // Aponta para o inicio atual da lista (insercao no inicio)

    return new; // Return do novo inicio da lista
}

//  le o ficheiro linha a linha e cria a lista ligada com as antenas
Antenna* load_ant() {

    FILE* file = fopen(MATRIZ_IN, "r"); // Abre matriz_in.txt em modo leitura

    // Se erro em abrir, exit com mensagem de erro
    if (!file) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", MATRIZ_IN);
        return NULL;
    }

    Antenna* head = NULL; // Inicializa a lista ligada vazia
    char line[256];       // Buffer para guardar as linhas do ficheiro
    int curr_line = 1;    // Contador para saber a linha atual (comecar a 1)

    // Le o ficheiro linha a linha
    while (fgets(line, sizeof(line), file)) {
        // Percorre cada char da linha
        for (int col = 0; line[col] != '\0' && line[col] != '\n'; col++) {
            char c = line[col];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '#') {
                head = insert_ant(head, c, curr_line, col + 1); // Soma +1 para coemcar na posicao 1 e nao 0
            }
        }
        curr_line++;
    }

    fclose(file);
    return head;
}

// Remove uma antena com base nas coordenadas (line, col)
Antenna* remove_ant(Antenna* head, int line, int col) {
    if (!head) return NULL;  // Se a lista for NULL

    Antenna* curr = head;   // Apontador para o node atual
    Antenna* prev = NULL;   // Apontador para o node anterior (inexistente inicialmente)

    // Percorre a lista ligada
    while (curr) {
        // Verifica se ha correspondencia
        if (!prev) {
            // A antena a remover esta no inicio da lista
            Antenna* aux = curr->next;
            free(curr);
            return aux;
        }
        else {
            // A antena esta no meio ou fim da lista
            prev->next = curr->next;
            free(curr);
            return head;
        }

        // Avanca um node
        prev = curr;
        curr = curr->next;
    }

    // Se nao houve correspondencia
    printf("Antena nao encontrada em (%d, %d)\n", line, col);
    return head; // Devolve a lista original
}

/*// Percorre a lista ligada de antenas e imprime os dados em formato tabular
void list_ant(const Antenna* head) {
    // Print da lista tabular
    printf("\n%-11s | %-11s\n", "Frequencia", "Localizacao");
    printf("--------------------------------\n");

    const Antenna* curr = head;

    // Percorre todos os nodes da lista até ao final
    while (curr != NULL) {
        printf("     %c      |   (%2d,%2d)\n", curr->freq, curr->line, curr->col);
        curr = curr->next;
    }

}
*/