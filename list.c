#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "list.h"
#include "utils.h"

t_list createEmptyList(){
  t_list newList;
  newList.head = NULL;
  return newList;
 }

void displayList(t_list l)
{
    t_cell* curr;
    curr = l.head;
    while (curr != NULL)
    {
        printf("%f", curr->probability);
        curr = curr->next;
    }
}

AdjacencyList createEmptyAdjacencyList(int size) {
    AdjacencyList adj;
    adj.size = size;
    adj.list = (t_list *) malloc(size * sizeof(t_list));
    for (int i = 0; i < size; i++) {
        adj.list[i].head = NULL;
    }

    return adj;
}

void addCellToList(t_list *l, int arrival, float probability) {
  t_cell *newCell = createCell(arrival, probability);
  newCell->next = l->head;
  l->head = newCell;
}

void displayAdjacencyList(AdjacencyList adj) {
  for (int i = 0; i < adj.size; i++) {
    t_cell* curr = adj.list->head;
    while (curr != NULL) {
      printf("|%d|-->", curr->arrival);
      curr = curr->next;
    }
  }
}

AdjacencyList readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, start, end;
    float proba;
    if (file == NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }
    AdjacencyList list = createEmptyAdjacencyList(nbvert);
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3){
        addCellToList(&(list.list[start - 1]), end, proba);
    }
    fclose(file);
    return list;
}

void checkMarkovGraph(AdjacencyList adj) {
    float count = 0;
    t_cell* curr = adj.list->head;
    while (curr != NULL) {
        count += curr -> probability;
        curr = curr->next;
    }
    if (0.99<= count <= 1){
        printf("The graph is a Markov graph\n");
        printf("The sum of the probabilities of vertex %d is %f\n", adj.list->head->arrival, count);
    }else {
        printf("The graph is not a Markov grap");
        printf("The sum of the probabilities of vertex %d is %f\n", adj.list->head->arrival, count);
    }
}

void convertForMermaid(AdjacencyList adj, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Could not open file for writing");
        return;
    }
    fprintf(f,"---\n");
    fprintf(f,"config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");
    fprintf(f,"flowchart LR\n");
    for (int i = 1; i < adj.size +1 ; i++)
    {
        fprintf(f, "%s((%d))\n", getID(i), i);
    }
    fprintf(f,"\n");

    for (int i = 0; i < adj.size  ; i++)
    {
        t_cell* curr = adj.list[i].head;
        while(curr != NULL)
        {
            fprintf(f, "%s -->|%f|", getID(i+1), curr->probability);
            fprintf(f, "%s\n", getID(curr->arrival));
            curr = curr->next;
        }
    }
    fclose(f);
    printf("The mermaid file : %s\n", filename);
}