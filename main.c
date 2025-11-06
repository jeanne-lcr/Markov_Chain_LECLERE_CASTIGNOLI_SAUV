#include <stdio.h>
#include "list.h"
#include "cell.h"


int main() {
    AdjacencyList adj;
    adj = readGraph("../data/exemple4_2check.txt");
    checkMarkovGraph(adj);
    return 0;
}
