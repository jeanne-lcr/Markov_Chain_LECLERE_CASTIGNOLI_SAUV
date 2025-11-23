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
    //PART 1
    printf(" PART 1\n");
    AdjacencyList adj;
    adj = readGraph("../data/exemple_valid_step3.txt");
    checkMarkovGraph(adj);
    displayAdjacencyList(adj);
    printf("\n");
    convertForMermaid(adj, "mermaid2.txt" );
    printf("\n");

    //PART 2 STEP 2
    printf("\n\n\n\n PART 2 STEP 2\n");
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
    printf("\n\n\n\n PART 2 STEP 3\n");
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
    printf("\n\n\n\n PART 3 STEP 1\n");
    AdjacencyList adj2 = readGraph("../data/exemple_meteo.txt");
    t_matrix M= createMatrix(adj2);
    displayMatrix(M);
    printf("\n");
    t_matrix M7 = powerMatrix(M, 7);
    displayMatrix(M7);

    //PART 3 STEP 2
    printf("\n\n\n\n PART 3 STEP 2\n");
    t_partition part;
    tarjan(adj2, &part);
    printf("Matrix :\n");
    displayMatrix(M);
    printf("\nNumber of classes (SCCs) : %d\n", part.size );
    for (int i = 0; i < part.size; i++)
    {
        printf("---Class %d---\n", i);
        t_matrix B= subMatrix(M,part, i);
        printf("Submatrix:\n");
        displayMatrix(B);
        t_matrix B_power= powerMatrix(B, 100);
        printf("PowerMatrix:\n");
        displayMatrix(B_power);
        printf("Stationary distribution of class %d:\n[", i);

        for (int j = 0; j < B.size; j++)
        {
            printf("%.2lf ",B_power.space[0][j]);

        }
        printf("]\n\n");

    }

    //PART 3 STEP 3
    printf("\n\n\n\n PART 3 STEP 3\n");
    for (int i = 0; i < part.size; i++)      // Loop through all SCC classes
    {
        printf("Class %d (Step 3)\n", i);     // Print the class index we are processing

        // Rebuild submatrix for this class
        // 'subMatrix' extracts the transition matrix restricted to the vertices
        // belonging to the SCC class 'i'.
        t_matrix B = subMatrix(M, part, i);

        printf("Submatrix:\n");
        displayMatrix(B);                     // Show the submatrix on screen

        // Compute the period of this class
        // getPeriod(B) computes the GCD of all return times n such that (B^n)[k][k] > 0.
        int period = getPeriod(B);
        printf("Period of class %d: %d\n", i, period);   // Display the period

        // If the period is 1 → the class is aperiodic (has a unique stationary distribution)
        if (period == 1) {
            printf(" -> Class %d is aperiodic.\n", i);

            // Compute a large power of B to approximate the stationary distribution
            int p = 100;                                  // exponent (large enough for convergence)
            t_matrix B_power = powerMatrix(B, p);         // B_power ≈ B^p

            printf("Associated stationary distribution for class %d (row 0 of B^%d):\n[", i, p);
            for (int j = 0; j < B_power.size; j++) {
                printf(" %.4lf", B_power.space[0][j]);    // first row of B^p
            }
            printf(" ]\n");

            // Free B_power (each row then the pointer array)
            for (int r = 0; r < B_power.size; r++)
                free(B_power.space[r]);
            free(B_power.space);
        }
        // Otherwise, period > 1 → the class is periodic
        else {
            printf(" -> Class %d is periodic with period %d.\n", i, period);
            printf("    (no unique stationary distribution given by the limit of B^n)\n");
        }

        //  we free each row and then the row-pointer array for B.
        for (int r = 0; r < B.size; r++)
            free(B.space[r]);                 // Free each row of B
        free(B.space);                        // Free the pointer array

        printf("\n");                         // Extra line for readability
    }





    return 0;
}
