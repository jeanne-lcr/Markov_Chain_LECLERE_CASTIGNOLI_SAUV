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
    double **M= createMatrix(adj2);
    displayMatrix(M,adj2.size);
    printf("\n");
    double **M7 = powerMatrix(M, adj2.size, 7);
    displayMatrix(M7,adj2.size);
    return 0;
}
