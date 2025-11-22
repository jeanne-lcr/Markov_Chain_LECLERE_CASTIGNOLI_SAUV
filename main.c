#include <stdio.h>
#include "list.h"
#include "cell.h"
#include "matrix.h"
#include "utils.h"
#include "matrix.h"


int main() {
    //AdjacencyList adj;
    //adj = readGraph("../data/exemple_valid_step3.txt");
    //checkMarkovGraph(adj);
    //displayAdjacencyList(adj);
    //convertForMermaid(adj, "mermaid2.txt" );

    AdjacencyList adj2 = readGraph("../data/exemple_meteo.txt");
    t_matrix M= createMatrix(adj2);
    displayMatrix(M);
    printf("\n");
    t_matrix M7 = powerMatrix(M, 7);
    displayMatrix(M7);
    return 0;
}
