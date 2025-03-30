#pragma once

#include "ant_list.h" // para usar a ED das antenas

// Estrutura para localizacoes nefastas
typedef struct NefLoc {
    int line;
    int col;
    struct NefLoc* next;
} NefLoc;

// Encontrar localizacoes nefastas
//NefLoc* deduce_nef(const Antenna* head);
//void list_nef(const NefLoc* head);

// Listagem principal
void list_ant(const Antenna* head);