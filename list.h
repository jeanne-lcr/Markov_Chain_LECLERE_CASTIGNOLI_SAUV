//
// Created by jeann on 07/11/2025.
//

#ifndef LIST_H
#define LIST_H
#include "cell.h"
//structure representing a list
typedef struct list {
    t_cell *head;//pointer to the head corresponding to the first element
}t_list;

//structure representing an adjacency list for a graph
typedef struct AdjacencyList{
    int size;//number of vertices in adjacency list
    t_list *list;//pointer to an array of lists with one for each vertex
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
