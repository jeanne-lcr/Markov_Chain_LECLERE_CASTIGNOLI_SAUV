#include <malloc.h>
#include "hasse.h"
#include "list.h"
#include "tarjan.h"
#include <stdio.h>
//Part 3 Step 2 Hasse diagram
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


//Part 3 Step 3
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
