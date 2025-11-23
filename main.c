#include <stdio.h>
#include "list.h"
#include "cell.h"
#include "matrix.h"
#include "utils.h"
#include "matrix.h"
#include "hasse.h"
#include "tarjan.h"
#include <stdlib.h>


int main() {
    AdjacencyList adj;
    adj = readGraph("../data/exemple_valid_step3.txt");
    //checkMarkovGraph(adj);
    //displayAdjacencyList(adj);
    //convertForMermaid(adj, "mermaid2.txt" );

    //PART 2 STEP 2
    t_partition partition;                                   // declare a partition structure to store Tarjan's SCC classes

    tarjan(adj, &partition);                                 // run Tarjan’s algorithm on the adjacency list 'adj'
    t_link_array link_array;                                // declare a structure to store links between classes
    link_array.size = 0;                                    // initialize the current number of links to 0 (no links yet)
    link_array.capacity = partition.size * partition.size;   // maximum possible links = all classes connected to all others
    link_array.link = malloc(link_array.capacity * sizeof(t_link));
                                                            // allocate memory to store all possible links
                                                            // array of t_link structures of size capacity

    if (link_array.link == NULL) {                          // check if malloc failed (no memory available)
        printf("could not allocate memory for links\n");    // print error message
        for (int i = 0; i < partition.size; i++) {          // loop over every class stored in the partition
            free(partition.classes[i].name);                // free allocated memory for the class name
            free(partition.classes[i].vertices);            // free allocated memory for the vertex array
        }
        free(partition.classes);                            // free the class array itself
        return 1;                                           // return error code
    }
    list_links(adj, &partition, &link_array);               // build the links between classes
                                                            // fills link_array.link[] and updates link_array.size

    convert_Classes_Mermaid(&partition, &link_array, "hasse_diagram.mmd");
                                                            // generate a Mermaid file containing ALL links between classes
                                                            // including redundant transitive links

    removeTransitiveLinks(&link_array);                     // remove all redundant transitive links

    convert_Classes_Mermaid(&partition, &link_array, "hasse_diagram_without_red.mmd");
                                                            // generate a second Mermaid file
                                                            // this time containing ONLY the essential edges (Hasse diagram)


    //PART 2 STEP 3
    graph_characteristics(&partition, &link_array); // print transient classes, persistent classes, absorbing states, irreducibility

    // free the array of links between classes
    free(link_array.link);                          // release the memory used by link_array.link

    // free the partition (each class has a name and an array of vertices)
    for (int i = 0; i < partition.size; i++) {      // loop over each class in the partition
        free(partition.classes[i].name);            // free the name string of the class
        free(partition.classes[i].vertices);        // free the Tarjan vertices array for this class
    }
    free(partition.classes);                        // free the array of classes



    //PART 3 STEP 1
    //AdjacencyList adj2 = readGraph("../data/exemple_meteo.txt");
    //t_matrix M= createMatrix(adj2);
    //displayMatrix(M);
    //printf("\n");
    //t_matrix M7 = powerMatrix(M, 7);
    //displayMatrix(M7);
    return 0;
}
