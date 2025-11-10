#ifndef TARJAN_H
#define TARJAN_H

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

#endif //TARJAN_H
