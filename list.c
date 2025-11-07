#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "list.h"
#include "utils.h"

// Function to create an empty list
t_list createEmptyList(){
  t_list newList;//declare a new list variable
  newList.head = NULL; // initialize the head of the list to NULL
  return newList; //return the new empty list
 }

// Function to display a list
void displayList(t_list l)
{
    t_cell* curr;  // initialize a pointer to iterate through the list
    curr = l.head; // initialize a current variable at the head of the list
    while (curr != NULL) // traverse the list until reaching the end of the list
    {
        printf("%f", curr->probability);// print the probability
        curr = curr->next; // move to the next cell
    }
}

// Function to create an empty adjacency list for a graph
AdjacencyList createEmptyAdjacencyList(int size) {
    AdjacencyList adj; // declare an adjacency list
    adj.size = size; // set the size of the list
    adj.list = (t_list *) malloc(size * sizeof(t_list)); // allocate memory for the list of lists
    for (int i = 0; i < size; i++) {
        adj.list[i].head = NULL; // set head of each list to NULL
    }

    return adj; // return the adjacency list
}

// Function to add a cell to a given list
void addCellToList(t_list *l, int arrival, float probability) {
  t_cell *newCell = createCell(arrival, probability); // create a new cell with given arrival and probability
  newCell->next = l->head; // set the next of the new cell to the current head
  l->head = newCell;  // update the head of the list to be the new cell
}

// Function to display an adjacency list
void displayAdjacencyList(AdjacencyList adj) {
  for (int i = 0; i < adj.size; i++) {// running through each list in the adjacency list
      printf("list for vertex %d: [head@]",i+1);// print for each vertex the list by starting at the head
    t_cell* curr = adj.list->head;// start with the head of the current list
    while (curr != NULL) {// traverse the list until the end
      printf("-->|%d,%2f|@", curr->arrival, curr->probability); // print the arrival and the probability by each list
      curr = curr->next;// move to the next cell
    }
  }
}


// Function to read a graph from a file and create an adjacency list
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


// Function to check if a graph is a Markov graph
void checkMarkovGraph(AdjacencyList adj) {
    float count = 0; // initialize probability sum counter
    t_cell* curr = adj.list->head; // start with the head of the first list
    while (curr != NULL) { // traverse the list
        count += curr -> probability; // accumulate the probabilities
        curr = curr->next; // move to the next cell
    }
    if (0.99<= count <= 1){ // check if the sum is between 0.99 and 1
        printf("The graph is a Markov graph\n");
        printf("The sum of the probabilities of vertex %d is %f\n", adj.list->head->arrival, count);// print probability sum
    }else {
        printf("The graph is not a Markov grap");
        printf("The sum of the probabilities of vertex %d is %f\n", adj.list->head->arrival, count);// print probability sum
    }
}


// Function to convert an adjacency list in a file made to be read by Mermaid, and represent the graph
void convertForMermaid(AdjacencyList adj, const char *filename)
{
    FILE *f = fopen(filename, "w"); //open the new file in write mode
    if (f == NULL) // if f NULL
    {
        printf("Could not open file for writing");
        return;
    }

    //print the head of the file
    fprintf(f,"---\n");
    fprintf(f,"config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");
    fprintf(f,"flowchart LR\n");
    //print the vertices and their corresponding number
    for (int i = 1; i < adj.size +1 ; i++)
    {
        fprintf(f, "%s((%d))\n", getID(i), i);
    }
    fprintf(f,"\n");

    // For each vertex in the graph
    for (int i = 0; i < adj.size  ; i++)
    {
        t_cell* curr = adj.list[i].head;  // Get the head of the linked list for this vertex
        // Traverse the adjacency list of vertex i
        while(curr != NULL)
        {
            // Write an edge line in Mermaid syntax
            fprintf(f, "%s -->|%f|", getID(i+1), curr->probability);
            fprintf(f, "%s\n", getID(curr->arrival));
            curr = curr->next;//Move to the next edge
        }
    }
    fclose(f); // Close the file after writing
    printf("The mermaid file : %s\n", filename);
}