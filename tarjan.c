#include <stdio.h>
#include "tarjan.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>


//Part 3, Step 1

//This function creates and initializes the Tarjan vertex array
t_tarjan_vertex* arrayVertices(AdjacencyList adj){
    //allocate memory for an array of tarjan vertices (one per graph vertex)
    t_tarjan_vertex* arr = malloc(adj.size*sizeof(t_tarjan_vertex));
    //loop through each vertex index from 0 to adj.size -1
    for (int i = 0; i < adj.size; i++){
        arr[i].identifier = adj.list[i].head->arrival; //set the identifier of vertex i using the arrival field of its adjacency list head
        arr[i].number = -1; //initialize Tarjan number to -1 meaning it has not been visited yet
        arr[i].access = -1; //initialize Tarjan access to -1 (low-link value)
        arr[i].bool = 0;//initialize the boolean flag to 0, meaning that it is not in stack
    }
    return arr; //return the pointer to the allocated tarjan vertex array
}//don't forget to free the memory after using this function !!


//This function initializes the stack by setting its element count to 0
void Initialization_Stack( t_stack* stack) {
    stack->nbElmts=0;
}

//This function returns 1 if the stack is empty, 0 otherwise
int is_Empty_stack( t_stack* stack) {
    return stack->nbElmts==0;
}


//This function pushes a value onto the stack, or prints an error if empty
void push(t_stack* stack, int v) {
    if (stack->nbElmts<100) {  //check there is still space in the fixed-size stack (max 100)
        stack->values[stack->nbElmts]=v; //store the new value at the current top position
        stack->nbElmts++; //increment the number of elements (stack grows by 1)
    }
    else {
        printf("Stack is full\n"); //print an error message if the stack is already full
    }
}


//This function pops and returns the top value of the stack, or prints an error if empty
int pop(t_stack* stack) {
    if (stack->nbElmts>0) { //check that the stack is not empty
        stack->nbElmts--; //decrease the number of elements (top index moves down)
        return stack->values[stack->nbElmts]; //return the value that was at the top of the stack
    }
    else {
        printf("Stack is empty\n"); //print an error message if the stack is empty
        return 0; ///return 0 as a default value
    }
}


//This function initializes the partition structure so its starts empty with no allocated classes
void Initialization_Partition(t_partition* partition) {
    partition->capacity=0; //set the maximum number of classes to 0 (no space reserved yet)
    partition->size=0; //set the current number of classes to 0 as it is empty
    partition->classes = NULL; //set the pointer to the class array to NULL (no memory allocated)
}


//This function adds a new class to the partition, allocating memory for its name and vertices, and returns it
t_class *add_class(t_partition* partition, char* name, int class_capacity) {
    if (partition->capacity == 0){ //if no memory has been allocated for the class array yet
        partition->capacity = 1; //set the capacity to 1 (we can store 1 class)
        partition->classes=malloc(sizeof(t_class)); //allocate memory for one t_class in the partition
    }
    if (partition->size == partition->capacity) { //if the number of classes has reached the capacity
        printf("not enough space to add a new class\n"); //print an error message
        return NULL;
    }
    t_class *next_class =&partition->classes[partition->size]; //get a point to the next free class slot
    next_class->name = malloc(strlen(name)+1); //allocate memory for the class name string (+1 for '\0')
    strcpy(next_class->name, name); //copy the given name into the allocated memory
    next_class->vertices = malloc(class_capacity * sizeof(t_tarjan_vertex)); // allocate an array of vertices for this class
    if (next_class->vertices == NULL) { //check if the memory allocation for vertices failed
        printf("not enough space to allocate vertices\n");
        return NULL;
    }
    next_class->size=0; //initialize the number of vertices in the class to 0
    next_class->capacity = class_capacity; //store the maximum number of vertices allowed in this class
    partition->size++; //increment the number of classes in the partition (we just added one)
    return next_class; //return a pointer to the newly created class
}


//This function adds a Tarjan vertex to a class if there is still enough capacity
void add_vertex_to_class(t_class *class, t_tarjan_vertex vertex) {
    if (class->size >= class->capacity) { //check if the class already contains the maximum number of vertices
        printf("not enough space to add a vertex to class\n"); //print an error message if the class is full
        return; //exit the function without adding the vertex
    }
    class->vertices[class->size]=vertex; //store the given vertex at the current end of the class's vertex array
    class->size++;//increment the number of vertices in the class
}



//This function is a recursive Tarjan function that explores vertex v and builds strongly connected components (SCCs)
void parcours(int v,AdjacencyList *G,t_tarjan_vertex *V,int *num,t_stack *P,t_partition *partition,int *class_counter,int class_capacity) {
    t_cell *edge; //pointer used to iterate through adjacency list of vertex v
    int w_index; //index of successor vertex w in the vertex array V
    char class_name[10]; //buffer used to store the name of a new SCC class (ex:'C1')

    V[v].number= *num; //assign the current global index to v.number (discovery order)
    V[v].access= *num;// initialize v.access (lowlink) to the same value as v.number
    (*num)++; //increment the global counter for the next discovered vertex

    push(P,v); // push vertex v onto the stack P
    V[v].bool= 1; // mark v as being currently in the stack (bool = 1)

    edge=G->list[v].head; // get the head of the adjacency list for vertex v
    while (edge != NULL) { // loop through all outgoing edges from vertex v
        w_index= edge->arrival-1; // compute the index of the arrival vertex w (arrival is 1-based)
        if (V[w_index].number == -1) {  // if w has not yet been visited (number == -1)
            parcours(w_index, G, V, num, P, partition, class_counter, class_capacity); // recursively apply Tarjan to the successor vertex w
            if (V[w_index].access<V[v].access) { // after recursion, if the lowlink of w is smaller than that of v
                V[v].access= V[w_index].access; // then update v.access to be the minimum (propagate lowlink)
            }
        }
        else if (V[w_index].bool == 1) { // else if w has already been visited and is still in the stack
            if (V[w_index].number < V[v].access) { // if the discovery number of w is smaller than v.access
                V[v].access= V[w_index].number; // update v.access with w.number (back-edge to an ancestor)
            }
        }
        edge=edge->next; // move to the next edge in v's adjacency list
    }
    if (V[v].access==V[v].number) { // if v is a root of an SCC (its lowlink equals its discovery number)
        int w; // variable used to store vertices popped from the stack
        t_class *C; // pointer to the new class representing this SCC
        (*class_counter)++; // increment the SCC counter to generate a new class name
        sprintf(class_name,"C%d",*class_counter); // build the class name string
        C=add_class(partition,class_name,class_capacity); // create a new class in the partition for this SCC
        w = pop(P); // pop a vertex from the stack
        V[w].bool = 0; // mark w as no longer being in the stack
        add_vertex_to_class(C, V[w]); // add w to the current SCC class
        while (w != v) { // continue popping until we reach the root vertex v
            w = pop(P);// pop the next vertex from the stack
            V[w].bool = 0; // mark this vertex as not in the stack anymore
            add_vertex_to_class(C, V[w]); // add this vertex to the same SCC class
        }

    }
}


//This function displays all classes of the partition and the list of vertex identifiers in each class
void display_partition(t_partition *partition) {
    for (int i = 0; i < partition->size; i++) { // loop over all classes stored in the partition
        t_class *C = &partition->classes[i]; // get a pointer to the current class

        printf("%s : {", C->name);// print the class name followed by an opening brace

        for (int j = 0; j < C->size; j++) { // loop over all vertices in the current class
            printf("%d", C->vertices[j].identifier); // print the identifier (original index) of the vertex
            if (j < C->size - 1) // if this is not the last vertex in the class
                printf(", "); // print a comma and a space to separate the vertices
        }
        }

        printf("}\n"); // close the brace and go to the next line
}


// This function runs Tarjan's algorithm on the graph to compute and display its strongly connected components
void tarjan(AdjacencyList G, t_partition *partition) {
    t_tarjan_vertex *V; // pointer to the array of Tarjan vertex info (number, access, etc.)
    t_stack P; // stack used by Tarjan's algorithm to track current SCC
    int num=0; // global counter for assigning discovery numbers to vertices
    int class_counter=0; // counter used to generate unique class names (C1, C2, ...)
    int v; // loop variable for vertices
    int class_capacity; // maximum capacity in number of vertices for each SCC class
    Initialization_Stack(&P); // initialize the stack P as empty
    Initialization_Partition(partition); // initialize the stack P as empty
    partition->capacity=G.size; // reserve space to store up to G.size classes in the partition
    partition->size=0; // no classes stored yet
    partition->classes = malloc(G.size*sizeof(t_class));// allocate memory for an array of t_class of size G.size
    V=arrayVertices(G); // create and initialize the Tarjan vertex array from the graph
    if (V==NULL) { // check if memory allocation failed
        printf("could not allocate Tarjan vertices\n");
        return; // exit the function as we cannot continue Tarjan
    }
    class_capacity=G.size; // set the maximum size of each class to the number of vertices
    for (v=0; v<G.size; v++) { // loop through all vertices in the graph
        if (V[v].number==-1) { // if vertex v has not been visited yet (number == -1)
            parcours(v,&G,V,&num,&P,partition,&class_counter,class_capacity); // start Tarjan exploration from vertex v
        }
    }
    free(V); // free the memory used by the Tarjan vertex array
    display_partition(partition); // display the resulting partition into strongly connected components
}



