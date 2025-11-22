#include <stdlib.h>
#include "list.h"
#include "matrix.h"
#include <stdio.h>
#include "math.h"

double **createMatrix(AdjacencyList adj){
    int n = adj.size;
    double **M = NULL;
    M = (double **) malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++) {
      M[i] = calloc(n, sizeof(double));
    }

    for (int i = 0; i < n; i++) {
      t_cell* curr = adj.list[i].head;
      while (curr != NULL) {
        M[i][(curr->arrival)-1] = (double)curr->probability;
        curr = curr->next;
      }
    }
    return M;
}//don't forget to free the space allocated

double **createMatrixWith0(int size){
  double **M = (double **) malloc(size * sizeof(double *));
  for (int i = 0; i < size; i++) {
    M[i] = (double *) malloc(size * sizeof(double));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      M[i][j] = 0.0;
    }
  }
  return M;
}

double **CopyMatrix(double **M1, double **M2, int size){
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      M1[i][j] = M2[i][j];
    }
  }
  return M1;
}

double **MultiplyMatrix(double **M1, double **M2, int size){
  double **M = (double **) malloc(size * sizeof(double *));
  if (M == NULL)
  {
    printf("Malloc failed\n");
    return NULL;
  }
  for (int i = 0; i < size; i++) {
    M[i] = calloc(size, sizeof(double));
    if (M[i] == NULL)
    {
      printf("Malloc failed\n");
      for (int h=0; h<size; h++)
      {
        free(M[h]);
      }
      free(M);
      return NULL;
    }
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      for (int k = 0; k < size; k++) {
        M[i][j] += M1[i][k] * M2[k][j];
      }
    }
  }
  return M;
}

double differenceMatrix(double **M1, double **M2, int size){
  double difference = 0.0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      difference = difference + fabs(M1[i][j] - M2[i][j]); // fabs = absolut value of float values
    }
  }
  return difference;
}

void displayMatrix(double **M, int n) {
  if (M == NULL) {
    printf("Matrix is NULL\n");
    return;
  }

  printf("[");  // start outer bracket

  for (int i = 0; i < n; i++) {
    printf("[");
    for (int j = 0; j < n; j++) {
      printf("%.2lf", M[i][j]);
      if (j < n - 1) printf(" ");
    }
    printf("]");
    if (i < n - 1) printf("\n "); // newline + space before next row
  }

  printf("]\n");  // end outer bracket
}

double **powerMatrix(double **M, int n, int p) {
  // make a copy of M as current power
  double **R = malloc(n * sizeof(double *));
  if (!R) return NULL;
  for (int i = 0; i < n; i++) {
    R[i] = malloc(n * sizeof(double));
    if (!R[i]) { for (int k = 0; k < i; k++) free(R[k]); free(R); return NULL; }
    for (int j = 0; j < n; j++) {
      R[i][j] = M[i][j];
    }
  }

  for (int e = 1; e < p; e++) {      // already M^1, do p-1 more multiplies
    double **next = MultiplyMatrix(R, M, n);
    // free old R
    for (int i = 0; i < n; i++) free(R[i]);
    free(R);
    R = next;
  }

  return R;
}
