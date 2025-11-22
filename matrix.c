//
// Created by jeann on 21/11/2025.
//
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
      M[i] = (double *) malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; i++) {
      t_cell* curr = adj.list[i].head;
      while (curr != NULL) {
        M[i][(curr->arrival) - 1] = curr->probability;
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
    return 0;
  }
  for (int i = 0; i < size; i++) {
    M[i] = (double *) malloc(size * sizeof(double));
    if (M[i] == NULL)
    {
      printf("Malloc failed\n");
      for (int h=0; h<size; h++)
      {
        free(M[h]);
      }
      free(M);
      return 0;
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
