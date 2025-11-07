#include <stdio.h>
#include "list.h"
#include "cell.h"
#include "utils.h"


int main() {
    AdjacencyList adj;
    adj = readGraph("../data/exemple_valid_step3.txt");
    checkMarkovGraph(adj);
    displayAdjacencyList(adj);
    convertForMermaid(adj, "mermaid2.txt" );
    return 0;
}
