#include <stdio.h>
#include "ant_list.h"

int main() {
    // Carrega a lista de antenas a partir do ficheiro
    Antenna* head = load_ant();

    // Verifica se a lista foi carregada
    if (!head) {
        printf("Lista vazia ou erro ao carregar.\n");
        return 1;
    }

    // Inserir antena manualmente
    head = insert_ant(head, 'B', 6, 2);

    // Remover antena manualmente
    //head = remove_ant(list, 2, 2);  // Remove antena que estava em (1, 1)

    // Lista as antenas carregadas no formato tabular
    list_ant(head);

    return 0;
}
