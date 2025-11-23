#include <malloc.h>
#include "hasse.h"
#include "list.h"
#include "tarjan.h"
#include <stdio.h>
//Part 3 Step 2 Hasse diagram
//How to link the lists

// Builds an array that maps each vertex index to the index of the class it belongs to.
int* array_vertex_to_class(AdjacencyList adj, t_partition *partition) {
    int *array_vertex=malloc(sizeof(int)*adj.size);// allocate an integer array of length equal to the number of vertices
    for (int i = 0; i < partition->size; i++) {// loop over all classes in the partition
        t_class *C = &partition->classes[i]; // get a pointer to the i-th class
        for (int j = 0; j < C->size; j++) { // loop over all vertices in this class
            int vertex_id= C->vertices[j].identifier; // get the vertex identifier (1-based index in the original graph)
            int index=vertex_id-1; // convert the 1-based identifier to a 0-based array index
            array_vertex[index]=i;  // store that vertex 'index' belongs to class with index i

        }
    }
    return array_vertex; // return the pointer to the completed mapping array
}

// Checks whether a link (start -> end) already exists in the link array.
int is_link(t_link_array *array, int start, int end) {
    for (int i=0; i<array->size; i++) { // loop through all currently stored links
        if (array->link[i].start_class==start && array->link[i].end_class==end) { // if the start_class matches the 'start' parameter and the end_class matches the 'end' parameter
            return 1; // then the link already exists, return 1 (true)
        }
    }
    return 0;// if no matching link was found, return 0 (false)
}


// Adds a new link (start -> end) to the link array if there is room and it does not already exist.
void add_link(t_link_array *array, int start, int end) {
    if (array->size == array->capacity) { // check if we have reached the maximum number of links
        printf("not enough space to add links\n"); // print an error if there is no more space
        return;// exit the function without adding anything
    }
    if (is_link(array, start, end)) { // check if the link (start, end) is already present
        return;// if it exists, do not add a duplicate and simply return
    }
    array->link[array->size].start_class=start; // set the start_class of the new link to 'start'
    array->link[array->size].end_class=end; // set the end_class of the new link to 'end'
    array->size++; // increment the number of stored links by 1
}


// Builds all links between classes based on the original adjacency list of the graph.
void list_links(AdjacencyList adj, t_partition *partition, t_link_array *link_array) {
    int *array_vertex = array_vertex_to_class(adj, partition);// build an array that maps each vertex to its class index
    if (array_vertex==NULL) {// check if the allocation or building of this array failed
        return; // if so, stop the function early
    }
    for ( int i=0; i<adj.size; i++) {// loop over all vertices (indices 0..size-1)
        int Ci=array_vertex[i];// get the class index for vertex i
        t_cell *edge=adj.list[i].head;// get the head of the adjacency list for vertex i
        while (edge != NULL) { // loop through all edges going out of vertex i
            int j=edge->arrival-1; // get the index of the arrival vertex j (convert from 1-based)
            int Cj=array_vertex[j]; // get the class index for vertex j
            if (Ci!=Cj) { // if vertices i and j belong to different classes
                add_link(link_array, Ci, Cj); // then add a link from class Ci to class Cj
            }
            edge=edge->next; // go to the next edge in i's adjacency list
        }
    }
    free(array_vertex); // free the temporary vertex-to-class mapping array
}


// Removes links that are transitively implied by other links, keeping only Hasse diagram edges.
void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;// index for the current link (link1) being tested

    while (i < p_link_array->size)// loop as long as i is within the link array size
    {
        t_link link1 = p_link_array->link[i]; // copy the current link (potentially transitive)

        int j = 0;// index for a second link (link2)
        int to_remove = 0;// flag indicating whether link1 should be removed

        while (j < p_link_array->size && !to_remove)// loop over all links while link1 is not marked for removal
        {
            if (j != i) // avoid comparing link1 with itself
            {
                t_link link2 = p_link_array->link[j]; // copy the second link to compare with link1


                if (link1.start_class == link2.start_class) // check if link1 and link2 share the same starting class
                {

                    int k = 0;// index for a third link (link3)
                    while (k < p_link_array->size && !to_remove)// loop while we have links and link1 not yet marked
                    {
                        if (k != j && k != i)// skip link1 and link2 to avoid trivial checks
                        {
                            t_link link3 = p_link_array->link[k]; // copy a third link from the array

                            if (link3.start_class == link2.end_class &&
                                link3.end_class   == link1.end_class) // check if link3 goes from link2's end class to link1's end class
                            {
                                to_remove = 1; // then link1 is transitive: A->C exists via A->B and B->C
                            }
                        }
                        k++;// go to the next candidate link3
                    }
                }
            }
            j++;// go to the next candidate link2
        }

        if (to_remove)// if link1 has been detected as transitive
        {
            p_link_array->link[i] = p_link_array->link[p_link_array->size - 1]; // overwrite link1 with the last link in the array
            p_link_array->size--;// reduce the logical size of the array by 1
        }
        else// if link1 is not transitive and is kept
        {
            i++; // move to the next link (increment i)
        }
    }
}


//Part 3 Step 3

// Converts the class graph (partition + links) into a Mermaid diagram and writes it to a file.
void convert_Classes_Mermaid(t_partition *partition, t_link_array *links, const char *filename){
    FILE *f = fopen(filename, "w");                    // open the output file in write mode ("w")
    if (f == NULL) {                                   // check if fopen failed
        printf("Could not open file %s\n", filename);  // print an error message if file could not be created
        return;                                        // exit the function early
    }
    fprintf(f, "flowchart LR\n\n");                    // Mermaid directive: left-to-right flowchart
    for (int i = 0; i < partition->size; i++) {        // loop through all classes in the partition
        t_class *C = &partition->classes[i];           // get a pointer to the current class
        fprintf(f, "  %s[\"{", C->name);               // begin printing the node: C1["{
        for (int j = 0; j < C->size; j++) {            // loop through all vertices inside this class
            fprintf(f, "%d", C->vertices[j].identifier);// print the vertex identifier (ex: 5, 7, 1)
            if (j < C->size - 1)                       // if not the last element in the class
                fprintf(f, ", ");                      // print a comma separator
        }
        fprintf(f, "}\"]\n");                          // close the Mermaid node definition  --> {5,7,1}"]
    }
    fprintf(f, "\n");                                  // print a blank line for clarity
    for (int i = 0; i < links->size; i++) {            // loop through all class-to-class links
        int from = links->link[i].start_class;         // index of source class in the partition array
        int to   = links->link[i].end_class;           // index of destination class in the partition array

        fprintf(f, "  %s --> %s\n",                    // Mermaid arrow syntax: A --> B
                partition->classes[from].name,         // print source class name (ex: C1)
                partition->classes[to].name);          // print destination class name (ex: C4)
    }


    fclose(f);                                         // close the output file
    printf("Hasse Mermaid file written in: %s\n", filename);
                                                       // notify the user the file was created
}

// Returns 1 if the class with index class_index has at least one outgoing link (transitory), 0 otherwise.
int is_transitory(int class_index, t_link_array *links) {
    for (int i = 0; i < links->size; i++) {// loop through all links
        if (links->link[i].start_class==class_index) { // if any link starts from the given class index
            return 1;// then the class is transitory (has an outgoing link)
        }
    }
    return 0;// no outgoing link found, class is not transitory
}


// Returns 1 if the class with index class_index has no outgoing links (persistent), 0 otherwise
int is_persistent(int class_index, t_link_array *links) {
    for (int i = 0; i < links->size; i++) {// loop through all links
        if (links->link[i].start_class==class_index) {// if we find a link that starts from this class
            return 0;// then the class is not persistent (it has an outgoing link)
        }
    }
    return 1;// no outgoing links found, class is persistent
}


// Returns 1 if the class at class_index is persistent and contains exactly one state (absorbing)
int is_absorbing_state(int class_index, t_partition *partition, t_link_array *link_array) {
    t_class *C = &partition->classes[class_index];// get a pointer to the class at index class_index
    if (!is_persistent(class_index, link_array)) { // check if the class is not persistent
        return 0; // if it's not persistent, it cannot be absorbing
    }
    if (C->size==1) { // if the class contains exactly one state
        return 1; // this unique state is absorbing
    }
    return 0;  // otherwise the class does not represent an absorbing state
}


// Returns 1 if the Markov graph is irreducible (only one class), 0 otherwise
int is_irreducible(t_partition *partition) {
    if (partition->size==1) {// check if there is exactly one class in the partition
        return 1; // if yes, the Markov graph is irreducible
    }
    return 0;// if there is more than one class, it is not irreducible
}


// Prints the transient classes, persistent classes, absorbing states and irreducibility of the Markov graph
void graph_characteristics(t_partition *partition, t_link_array *link_array) {
    printf("Transient classes :"); // print title for the list of transient classes
    for (int i = 0; i < partition->size; i++) { // loop over all classes in the partition
        if (is_transitory(i,link_array)) { // check if the i-th class is transitory
            t_class *C = &partition->classes[i]; // get a pointer to that clas
            printf("  %s : {", C->name); // print the class name followed by an opening brace
            for (int j = 0; j < C->size; j++) { // loop over all vertices in this class
                printf("%d", C->vertices[j].identifier); // print the vertex identifier
                if (j != C->size-1) { // if this is not the last vertex in the class
                    printf(", ");  // print a comma and a space
                }

            }
            printf("}"); // close the brace for this class
        }
    }
    printf("\n");

    printf("\nPersistent classes :"); // print title for the list of persistent classes
    for (int i = 0; i < partition->size; i++) { // loop over all classes again
        if (is_persistent(i,link_array)) {  // check if the i-th class is persistent
            t_class *C = &partition->classes[i]; // get a pointer to that class
            printf("  %s:{", C->name); // print the class name followed by an opening brace
            for (int j = 0; j < C->size; j++) { // loop over all vertices in this class
                printf("%d", C->vertices[j].identifier); // print the vertex identifier
                if (j != C->size-1) { // if this is not the last vertex in the class
                    printf(", "); // print a comma and a space
                }

            }
            printf("}");
        }

    }
    printf("\n");


    printf("\nAbsorbing states:"); // print title for absorbing states
    for (int i = 0; i < partition->size; i++) {  // loop over all classes
        if (is_absorbing_state(i, partition,link_array)) { // check if the class at index i is absorbing
            t_class *C = &partition->classes[i]; // get a pointer to that absorbing class
            int state = C->vertices[0].identifier; // get the identifier of the (unique) state in this class
            printf("   state %d from class %s \n", state, C->name); // print the absorbing state and its class
        }
    }

    if (is_irreducible(partition)) {  // check if the Markov graph is irreducible
        printf(" The Markov Graph is irreducible");
    }
    else {
        printf(" The Markov Graph is not irreducible");
    }


}
