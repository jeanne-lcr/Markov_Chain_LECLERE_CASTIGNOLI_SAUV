#ifndef TARJAN_H
#define TARJAN_H
#include "list.h"
typedef struct tarjan{
  int identifier;
  int number;
  int access;
  int bool;
}t_tarjan_vertex;

typedef struct class {
  char *name; // name of the class, exemple : C1, C2,...
  t_tarjan_vertex *vertices; //array thta points on each vertex of the array
  int size; // logical size
  int capacity; //physical size
}t_class;

typedef struct partition{
  t_class *classes;
  int size;//logical size
  int capacity;//physical size
}t_partition;

typedef struct stack{
  int values[100];
  int nbElmts;
}t_stack;

t_tarjan_vertex* arrayVertices(AdjacencyList adj);
void Initialization_Stack( t_stack* stack);
int is_Empty_stack( t_stack* stack);
void push(t_stack* stack, int v);
int pop(t_stack* stack) ;
void Initialization_Partition(t_partition* partition);
t_class *add_class(t_partition* partition, char* name, int class_capacity);
void add_vertex_to_class(t_class *class, t_tarjan_vertex vertex) ;
void parcours(int v,AdjacencyList *G,t_tarjan_vertex *V,int *num,t_stack *P,t_partition *partition,int *class_counter,int class_capacity);
void display_partition(t_partition *partition);
void tarjan(AdjacencyList G, t_partition *partition) ;
int* array_vertex_to_class(AdjacencyList adj, t_partition *partition);
int is_link(t_link_array *array, int start, int end);
void add_link(t_link_array *array, int start, int end) ;
void list_links(AdjacencyList adj, t_partition *partition, t_link_array *link_array) ;
void removeTransitiveLinks(t_link_array *p_link_array);
int is_transitory(int class_index, t_link_array *links);
int is_persistent(int class_index, t_link_array *links);
int is_absorbing_state(int class_index, t_partition *partition, t_link_array *link_array);
int is_irreducible(t_partition *partition);
void graph_characteristics(t_partition *partition, t_link_array *link_array) ;


#endif //TARJAN_H
