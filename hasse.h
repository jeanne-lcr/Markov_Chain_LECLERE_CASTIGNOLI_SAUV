#ifndef __HASSE_H__
#define __HASSE_H__
#include "list.h"
#include "tarjan.h"
typedef struct link {
    int start_class;
    int end_class;
}t_link;

typedef struct {
    t_link *link;
    int size;
    int capacity;
}t_link_array;
int* array_vertex_to_class(AdjacencyList adj, t_partition *partition);
int is_link(t_link_array *array, int start, int end);
void add_link(t_link_array *array, int start, int end) ;
void list_links(AdjacencyList adj, t_partition *partition, t_link_array *link_array) ;
int is_transitory(int class_index, t_link_array *links);
int is_persistent(int class_index, t_link_array *links);
int is_absorbing_state(int class_index, t_partition *partition, t_link_array *link_array);
int is_irreducible(t_partition *partition);
void graph_characteristics(t_partition *partition, t_link_array *link_array) ;
void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif