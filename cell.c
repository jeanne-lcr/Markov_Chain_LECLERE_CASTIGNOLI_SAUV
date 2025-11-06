#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "list.h"


t_cell *createCell(int arrival, float probability) {
    t_cell *newCell = malloc(sizeof(t_cell));
    newCell->arrival = arrival;
    newCell->probability = probability;
    newCell->next = NULL;
    return newCell;
}
