#include <stdio.h>
#include "tarjan.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>


//Part 3, Step 1
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

void Initialization_Stack( t_stack* stack) {
    stack->nbElmts=0;
}

int is_Empty_stack( t_stack* stack) {
    return stack->nbElmts==0;
}

void push(t_stack* stack, int v) {
    if (stack->nbElmts<100) {
        stack->values[stack->nbElmts]=v;
        stack->nbElmts++;
    }
    else {
        printf("Stack is full\n");
    }
}

int pop(t_stack* stack) {
    if (stack->nbElmts>0) {
        stack->nbElmts--;
        return stack->values[stack->nbElmts];
    }
    else {
        printf("Stack is empty\n");
        return 0;
    }
}

void Initialization_Partition(t_partition* partition) {
    partition->capacity=0;
    partition->size=0;
    partition->classes = NULL;
}

t_class *add_class(t_partition* partition, char* name, int class_capacity) {
    if (partition->capacity == 0){
        partition->capacity = 1;
        partition->classes=malloc(sizeof(t_class));
    }
    if (partition->size == partition->capacity) {
        printf("not enough space to add a new class\n");
        return NULL;
    }
    t_class *next_class =&partition->classes[partition->size];
    next_class->name = malloc(strlen(name)+1);
    strcpy(next_class->name, name);
    next_class->vertices = malloc(class_capacity * sizeof(t_tarjan_vertex));
    if (next_class->vertices == NULL) {
        printf("not enough space to allocate vertices\n");
        return NULL;
    }
    next_class->size=0;
    next_class->capacity = class_capacity;
    partition->size++;
    return next_class;
}

void add_vertex_to_class(t_class *class, t_tarjan_vertex vertex) {
    if (class->size >= class->capacity) {
        printf("not enough space to add a vertex to class\n");
        return;
    }
    class->vertices[class->size]=vertex;
    class->size++;
}


void parcours(int v,AdjacencyList *G,t_tarjan_vertex *V,int *num,t_stack *P,t_partition *partition,int *class_counter,int class_capacity) {
    t_cell *edge;
    int w_index;
    char class_name[10];

    V[v].number= *num;
    V[v].access= *num;
    (*num)++;

    push(P,v);
    V[v].bool= 1;

    edge=G->list[v].head;
    while (edge != NULL) {
        w_index= edge->arrival-1;
        if (V[w_index].number == -1) {
            parcours(w_index, G, V, num, P, partition, class_counter, class_capacity);
            if (V[w_index].access<V[v].access) {
                V[v].access= V[w_index].access;
            }
        }
        else if (V[w_index].bool == 1) {
            if (V[w_index].number < V[v].access) {
                V[v].access= V[w_index].number;
            }
        }
        edge=edge->next;
    }
    if (V[v].access==V[v].number) {
        int w;
        t_class *C;
        (*class_counter)++;
        sprintf(class_name,"C%d",*class_counter);
        C=add_class(partition,class_name,class_capacity);
        w = pop(P);
        V[w].bool = 0;
        add_vertex_to_class(C, V[w]);
        while (w != v) {
            w = pop(P);
            V[w].bool = 0;
            add_vertex_to_class(C, V[w]);
        }

    }
}

void display_partition(t_partition *partition) {
    for (int i = 0; i < partition->size; i++) {
        t_class *C = &partition->classes[i];

        printf("%s : {", C->name);

        for (int j = 0; j < C->size; j++) {
            printf("%d", C->vertices[j].identifier);
            if (j < C->size - 1)
                printf(", ");
        }

        printf("}\n");
    }
}

void tarjan(AdjacencyList G, t_partition *partition) {
    t_tarjan_vertex *V;
    t_stack P;
    int num=0;
    int class_counter=0;
    int v;
    int class_capacity;
    Initialization_Stack(&P);
    Initialization_Partition(partition);
    partition->capacity=G.size;
    partition->size=0;
    partition->classes = malloc(G.size*sizeof(t_class));
    V=arrayVertices(G);
    if (V==NULL) {
        printf("could not allocate Tarjan vertices\n");
        return;
    }
    class_capacity=G.size;
    for (v=0; v<G.size; v++) {
        if (V[v].number==-1) {
            parcours(v,&G,V,&num,&P,partition,&class_counter,class_capacity);
        }
    }
    free(V);
    display_partition(partition);
}



