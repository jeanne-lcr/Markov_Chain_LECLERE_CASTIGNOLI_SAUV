//
// Created by jeann on 07/11/2025.
//

#ifndef LIST_H
#define LIST_H
#include "cell.h"
typedef struct list {
    t_cell *head;
}t_list;


typedef struct AdjacencyList{
    int size;
    t_list *list;
}AdjacencyList;

t_list createEmptyList();
void displayList(t_list l);
AdjacencyList createEmptyAdjacencyList(int size);
void addCellToList(t_list *l, int arrival, float probability);
void displayAdjacencyList(AdjacencyList adj);
AdjacencyList readGraph(const char *filename);
void checkMarkovGraph(AdjacencyList adj);
void convertForMermaid(AdjacencyList adj, const char *filename);
#endif //LIST_H
