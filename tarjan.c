#include "tarjan.h"
#include "list.h"
#include <stdlib.h>

t_tarjan_vertex* arrayVertices(AdjacencyList adj){
    t_tarjan_vertex* arr = malloc(adj.size*sizeof(t_tarjan_vertex));
    for (int i = 0; i < adj.size; i++){
        arr[i].identifier = adj.list[i].head->arrival;
        arr[i].number = -1;
        arr[i].access = -1;
        arr[i].bool = 0;
    }
    return arr;
}//don't forget to free the memory after using this function !!

void parcours (AdjacencyList adj, t_tarjan_vertex vertex, t_partition *partition,t_stack *stack, t_class *class) {
    vertex=arrayVertices(adj);
    stack->nbElmts=0;
    partition->classes=NULL;
    partition->size;
    partition->capacity=0;
    int index = 0;
    int classCounter = 0;