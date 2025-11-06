//
// Created by jeann on 07/11/2025.
//

#ifndef CELL_H
#define CELL_H
typedef struct cell {
    int arrival;
    float probability;
    struct cell *next;
} t_cell;
t_cell *createCell(int arrival, float probability);
#endif //CELL_H
