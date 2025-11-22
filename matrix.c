#include <stdlib.h>
#include "list.h"
#include "matrix.h"
#include <stdio.h>
#include "math.h"

t_matrix createMatrix(AdjacencyList adj){
    int n = adj.size;
    t_matrix M;
    M.size = n;
    M.space = (double **) malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++) {
      M.space[i] = calloc(n, sizeof(double));
    }

    for (int i = 0; i < n; i++) {
      t_cell* curr = adj.list[i].head;
      while (curr != NULL) {
        M.space[i][(curr->arrival)-1] = (double)curr->probability;
        curr = curr->next;
      }
    }
    return M;
}//don't forget to free the space allocated

t_matrix createMatrixWith0(int size){
  t_matrix M;
  M.size = size;
  M.space = (double **) malloc(size * sizeof(double *));
  for (int i = 0; i < size; i++) {
    M.space[i] = (double *) malloc(size * sizeof(double));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      M.space[i][j] = 0.0;
    }
  }
  return M;
}

t_matrix CopyMatrix(t_matrix M1, t_matrix M2){
  for (int i = 0; i < M2.size; i++) {
    for (int j = 0; j < M2.size; j++) {
      M1.space[i][j] = M2.space[i][j];
    }
  }
  return M1;
}

t_matrix MultiplyMatrix(t_matrix M1, t_matrix M2){
  t_matrix invalid = {0, NULL};
  t_matrix M;
  M.size = M1.size;
  M.space = (double **) malloc(M1.size * sizeof(double *));
  if (M.space == NULL)
  {
    printf("Malloc failed\n");
    return invalid;
  }
  for (int i = 0; i < M1.size; i++) {
    M.space[i] = calloc(M1.size, sizeof(double));
    if (M.space[i] == NULL)
    {
      printf("Malloc failed\n");
      for (int h=0; h<M1.size; h++)
      {
        free(M.space[h]);
      }
      free(M.space);
      return invalid;
    }
  }
  for (int i = 0; i < M1.size; i++) {
    for (int j = 0; j < M1.size; j++) {
      for (int k = 0; k < M1.size; k++) {
        M.space[i][j] += M1.space[i][k] * M2.space[k][j];
      }
    }
  }
  return M;
}

double differenceMatrix(t_matrix M1, t_matrix M2){
  double difference = 0.0;
  for (int i = 0; i < M1.size; i++) {
    for (int j = 0; j < M1.size; j++) {
      difference = difference + fabs(M1.space[i][j] - M2.space[i][j]); // fabs = absolut value of float values
    }
  }
  return difference;
}

void displayMatrix(t_matrix M) {
  int n = M.size;
  if (M.space == NULL) {
    printf("Matrix is NULL\n");
    return;
  }

  printf("[");  // start outer bracket

  for (int i = 0; i < n; i++) {
    printf("[");
    for (int j = 0; j < n; j++) {
      printf("%.2lf", M.space[i][j]);
      if (j < n - 1) printf(" ");
    }
    printf("]");
    if (i < n - 1) printf("\n "); // newline + space before next row
  }

  printf("]\n");  // end outer bracket
}

t_matrix powerMatrix(t_matrix M, int p) {
  // make a copy of M as current power
  t_matrix invalid = {0, NULL};
  int n = M.size;
  t_matrix R;
  R.size = n;
  R.space = malloc(n * sizeof(double *));
  if (!R.space) return invalid;
  for (int i = 0; i < n; i++) {
    R.space[i] = malloc(n * sizeof(double));
    if (!R.space[i]) { for (int k = 0; k < i; k++) free(R.space[k]); free(R.space); return invalid; }
    for (int j = 0; j < n; j++) {
      R.space[i][j] = M.space[i][j];
    }
  }

  for (int e = 1; e < p; e++) {      // already M^1, do p-1 more multiplies
    t_matrix next = MultiplyMatrix(R, M);
    // free old R
    for (int i = 0; i < n; i++) free(R.space[i]);
    free(R.space);
    R = next;
  }

  return R;
}

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index)
{

}

