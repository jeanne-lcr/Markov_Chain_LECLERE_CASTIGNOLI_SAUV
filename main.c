#include <stdio.h>
#include "list.h"
#include "cell.h"
#include "matrix.h"
#include "utils.h"
#include "matrix.h"


int main() {
    AdjacencyList adj;
    adj = readGraph("../data/exemple_valid_step3.txt");
    checkMarkovGraph(adj);
    displayAdjacencyList(adj);
    convertForMermaid(adj, "mermaid2.txt" );

    AdjacencyList adj2 = readGraph("../data/exemple_valid_step3.txt");
    double **M= createMatrix(adj2);
    for (int i=0; i< adj2.size; i++)
    {
        for (int j=0; j<adj2.size; j++)
        {
            printf("%f ",M[i][j]);
        }
    }

    return 0;
}
