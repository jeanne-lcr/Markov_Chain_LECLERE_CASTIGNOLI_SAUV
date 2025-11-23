#ifndef TARJAN_H
#define TARJAN_H
#include "list.h"
typedef struct tarjan {
  int identifier;   // ID of the vertex in the graph (1..n)
  int number;       // discovery index assigned
  int access;       // lowest reachable index (lowlink value)
  int bool;         // boolean: is the vertex currently on the Tarjan stack?
} t_tarjan_vertex;

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

typedef struct stack {
  int values[100];   // container for stack elements (max capacity = 100)
  int nbElmts;       // number of elements currently in the stack
} t_stack;

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


#endif //TARJAN_H
