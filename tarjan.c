#include <stdio.h>
#include "tarjan.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

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


//Part 2 Hasse diagram
//How to link the lists
int* array_vertex_to_class(AdjacencyList adj, t_partition *partition) {
    int *array_vertex=malloc(sizeof(int)*adj.size);
    for (int i = 0; i < partition->size; i++) {
        t_class *C = &partition->classes[i];
        for (int j = 0; j < C->size; j++) {
            int vertex_id= C->vertices[j].identifier;
            int index=vertex_id-1;
            array_vertex[index]=i;

        }
    }
    return array_vertex;
}


int is_link(t_link_array *array, int start, int end) {
    for (int i=0; i<array->size; i++) {
        if (array->link[i].start_class==start && array->link[i].end_class==end) {
            return 1;
        }
    }
    return 0;
}

void add_link(t_link_array *array, int start, int end) {
    if (array->size == array->capacity) {
        printf("not enough space to add links\n");
        return;
    }
    if (is_link(array, start, end)) {
        return;
    }
    array->link[array->size].start_class=start;
    array->link[array->size].end_class=end;
    array->size++;
}

void list_links(AdjacencyList adj, t_partition *partition, t_link_array *link_array) {
    int *array_vertex = array_vertex_to_class(adj, partition);
    if (array_vertex==NULL) {
        return;
    }
    for ( int i=0; i<adj.size; i++) {
        int Ci=array_vertex[i];
        t_cell *edge=adj.list[i].head;
        while (edge != NULL) {
            int j=edge->arrival-1;
            int Cj=array_vertex[j];
            if (Ci!=Cj) {
                add_link(link_array, Ci, Cj);
            }
            edge=edge->next;
        }
    }
    free(array_vertex);
}


//How to remove redundancies (optional)
void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;

    while (i < p_link_array->size)
    {
        t_link link1 = p_link_array->link[i];
        int j = 0;
        int to_remove = 0;

        while (j < p_link_array->size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->link[j];


                if (link1.start_class == link2.start_class)
                {

                    int k = 0;
                    while (k < p_link_array->size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->link[k];

                            if (link3.start_class == link2.end_class &&
                                link3.end_class   == link1.end_class)
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }

        if (to_remove)
        {
            p_link_array->link[i] = p_link_array->link[p_link_array->size - 1];
            p_link_array->size--;
        }
        else
        {
            i++;
        }
    }
}

//Part 3

int is_transitory(int class_index, t_link_array *links) {
    for (int i = 0; i < links->size; i++) {
        if (links->link[i].start_class==class_index) {
            return 1;
        }
    }
    return 0;
}

int is_persistent(int class_index, t_link_array *links) {
    for (int i = 0; i < links->size; i++) {
        if (links->link[i].start_class==class_index) {
            return 0;
        }
    }
    return 1;
}

int is_absorbing_state(int class_index, t_partition *partition, t_link_array *link_array) {
    t_class *C = &partition->classes[class_index];
    if (!is_persistent(class_index, link_array)) {
        return 0;
    }
    if (C->size==1) {
        return 1;
    }
    return 0;
}

int is_irreducible(t_partition *partition) {
    if (partition->size==1) {
        return 1;
    }
    return 0;
}

void graph_characteristics(t_partition *partition, t_link_array *link_array) {
    printf("Transient classes :");
    for (int i = 0; i < partition->size; i++) {
        if (is_transitory(i,link_array)) {
            t_class *C = &partition->classes[i];
            printf("  %s : {", C->name);
            for (int j = 0; j < C->size; j++) {
                printf("%d", C->vertices[j].identifier);
                if (j != C->size-1) {
                    printf(", ");
                }

            }
            printf("}");
        }
    }
    printf("\n");

    printf("\nPersistent classes :");
    for (int i = 0; i < partition->size; i++) {
        if (is_persistent(i,link_array)) {
            t_class *C = &partition->classes[i];
            printf("  %s:{", C->name);
            for (int j = 0; j < C->size; j++) {
                printf("%d", C->vertices[j].identifier);
                if (j != C->size-1) {
                    printf(", ");
                }

            }
            printf("}");
        }

    }
    printf("\n");


    printf("\nAbsorbing states:");
    for (int i = 0; i < partition->size; i++) {
        if (is_absorbing_state(i, partition,link_array)) {
            t_class *C = &partition->classes[i];
            int state = C->vertices[0].identifier;
            printf("   state %d from class %s \n", state, C->name);
        }
    }

    if (is_irreducible(partition)) {
        printf(" The Markov Graph is irreducible");
    }
    else {
        printf(" The Markov Graph is not irreducible");
    }


}