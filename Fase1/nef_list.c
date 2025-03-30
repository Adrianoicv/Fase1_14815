#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nef_list.h"

// Percorre a lista ligada de antenas e imprime os dados em formato tabular
void list_ant(const Antenna* head) {
    // Print da lista tabular
    printf("\n%-8s | %-11s\n", "Freq", "Loc (l,c)");
    printf("------------------------\n");

    const Antenna* curr = head;

    // Percorre todos os nodes da lista até ao final
    while (curr) {
        printf("    %c    |   (%2d,%2d)\n", curr->freq, curr->line, curr->col);
        curr = curr->next;
    }

}