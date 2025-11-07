#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "list.h"

//Function to create a new cell with given arrival and probability
t_cell *createCell(int arrival, float probability) {
    t_cell *newCell = malloc(sizeof(t_cell));//allocate memory for a new cell
    newCell->arrival = arrival; // set the arrival vertex for this cell
    newCell->probability = probability; // set the probability value
    newCell->next = NULL; // initialize the next pointer to NULL
    return newCell;
}
