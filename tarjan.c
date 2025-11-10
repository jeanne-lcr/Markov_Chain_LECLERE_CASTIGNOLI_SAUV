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