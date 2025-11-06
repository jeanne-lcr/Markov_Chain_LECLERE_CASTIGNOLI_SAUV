#include <stdio.h>
#include "list.h"
#include "cell.h"


int main() {
    AdjacencyList adj;
    adj = readGraph("../data/exemple1.txt");
    checkMarkovGraph(adj);
    return 0;
}
