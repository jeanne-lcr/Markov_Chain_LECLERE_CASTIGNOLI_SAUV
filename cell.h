//
// Created by jeann on 07/11/2025.
//

#ifndef CELL_H
#define CELL_H
// structure to representing a cell
typedef struct cell {
    int arrival;//arrival vertex which correspond to the edge destination
    float probability; //probability associated with the edge
    struct cell *next; //pointer to the next cell in the list
} t_cell;
t_cell *createCell(int arrival, float probability);
#endif //CELL_H
