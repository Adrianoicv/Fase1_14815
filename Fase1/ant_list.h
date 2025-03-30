#pragma once

// Estrutura de um node da lista ligada (uma antena)
typedef struct Antenna {
    char freq;              // Char (frequencia) da antena
    int line;               // Linha da matriz
    int col;                // Coluna da matriz
    struct Antenna* next;   // Apontador para o próximo node (antena) na lista
} Antenna;

Antenna* insert_ant(Antenna* head, char freq, int line, int col);
Antenna* load_ant();
Antenna* remove_ant(Antenna* head, int line, int col);
void list_ant(const Antenna* head);