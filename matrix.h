#ifndef MATRIX_H
#define MATRIX_H
#include "tarjan.h"

typedef struct matrix
{
 int size;
 double **space;
}t_matrix;

t_matrix createMatrix(AdjacencyList adj);
t_matrix createMatrixWith0(int size);
t_matrix CopyMatrix(t_matrix M1, t_matrix M2);
t_matrix MultiplyMatrix(t_matrix M1, t_matrix M2);
double differenceMatrix(t_matrix M1, t_matrix M2);
void displayMatrix(t_matrix M);
t_matrix powerMatrix(t_matrix M, int p);

/**
 * @brief Extracts a submatrix corresponding to a specific
component of a graph partition.
 *
 * @param matrix The original adjacency matrix of the graph.
 * @param part The partition of the graph into strongly
connected components.
 * @param compo_index The index of the component to extract.
 * @return t_matrix The submatrix corresponding to the
specified component.
 */
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
#endif //MATRIX_H
