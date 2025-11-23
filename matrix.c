#include <stdlib.h>
#include "list.h"
#include "matrix.h"
#include <stdio.h>
#include "math.h"


//This function builds the transition matrix from the adjacency matrix
t_matrix createMatrix(AdjacencyList adj){
    int n = adj.size; //number of vertices states
    t_matrix M; //initialize the matrix
    M.size = n; //initialize the size of the matrix
    M.space = (double **) malloc(n * sizeof(double *)); //allocate memory for the lines

    for (int i = 0; i < n; i++) {
      M.space[i] = calloc(n, sizeof(double)); //allocate memory for columns, initialize them to 0.0
    }
    //Fill the matrix using the adjacency list
    for (int i = 0; i < n; i++) {
      t_cell* curr = adj.list[i].head; //head of the adjacency list for vertex i
      while (curr != NULL) {
        M.space[i][(curr->arrival)-1] = (double)curr->probability; //arrival is stored as 1...n, convert to 0...n-1 for array index
        curr = curr->next; //move to the next edge in the list
      }
    }
    return M; //return the matrix created
}//don't forget to free the space allocated


//This function creates a matrix whose entries are all 0.0
t_matrix createMatrixWith0(int size){
  t_matrix M; //initialize the matrix
  M.size = size; //dimension of the matrix
  M.space = (double **) malloc(size * sizeof(double *)); //allocate array of row pointers
  //allocate each row
  for (int i = 0; i < size; i++) {
    M.space[i] = (double *) malloc(size * sizeof(double));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      M.space[i][j] = 0.0; //set all entries to 0.0, we could also use calloc instead of malloc
    }
  }
  return M; //return the matrix created
}


//This function copies the contents of matrix M2 into matrix M1
t_matrix CopyMatrix(t_matrix M1, t_matrix M2){
  for (int i = 0; i < M2.size; i++) {//loop over rows
    for (int j = 0; j < M2.size; j++) {//loop over columns
      M1.space[i][j] = M2.space[i][j]; //copy the value
    }
  }
  return M1; //return the matrix with copied values
}


//This function multiplies two square matrices M1 and M2 of the same size
t_matrix MultiplyMatrix(t_matrix M1, t_matrix M2){
  t_matrix invalid = {0, NULL}; //as we can't return NULL, we define an invalid value to return to signal an error
  t_matrix M; //initialize the new matrix
  M.size = M1.size; //same size as both previous matrix
  M.space = (double **) malloc(M1.size * sizeof(double *)); //allocate array of row pointers for result matrix
  if (M.space == NULL) //check if the memory is well allocated
  {
    printf("Malloc failed\n");//display error message
    return invalid; //return invalid matrix
  }
  //allocate each row and initialize entries to 0.0 using calloc
  for (int i = 0; i < M1.size; i++) {
    M.space[i] = calloc(M1.size, sizeof(double));
    if (M.space[i] == NULL)
    {
      printf("Malloc failed\n"); //report failure of the allocation
      for (int h=0; h<M1.size; h++) //free all rows allocated if the allocation failed
      {
        free(M.space[h]);
      }
      free(M.space);//free row pointer array
      return invalid;//return invalid matrix
    }
  }
  //Triple nested loop for matrix multiplication
  for (int i = 0; i < M1.size; i++) { //row index of result
    for (int j = 0; j < M1.size; j++) { //column index of result
      for (int k = 0; k < M1.size; k++) { //index for the sum
        M.space[i][j] += M1.space[i][k] * M2.space[k][j];
      }
    }
  }
  return M; //return result matrix
}


//This function computes the sum of absolute differences between two matrices
double differenceMatrix(t_matrix M1, t_matrix M2){
  double difference = 0.0; //accumulator for the sum
  for (int i = 0; i < M1.size; i++) { //loop over rows
    for (int j = 0; j < M1.size; j++) { //loop over columns
      difference = difference + fabs(M1.space[i][j] - M2.space[i][j]); // fabs = absolut value of float values
    }
  }
  return difference; //return final sum
}

//This function displays the matrix
void displayMatrix(t_matrix M) {
  int n = M.size; //matrix dimension
  if (M.space == NULL) {
    printf("Matrix is NULL\n");
    return;
  }

  printf("[");  // start outer bracket

  for (int i = 0; i < n; i++) {//loop over rows
    printf("[");
    for (int j = 0; j < n; j++) { //loop over columns
      printf("%.2lf", M.space[i][j]);//print element with 2 decimals
      if (j < n - 1) printf(" ");
    }
    printf("]");
    if (i < n - 1) printf("\n "); // newline + space before next row
  }

  printf("]\n");  // end outer bracket
}


//This function computes the p-th power of a matrix
t_matrix powerMatrix(t_matrix M, int p)
{
  t_matrix invalid = {0, NULL};//invalid matrix used in case of an error
  int n = M.size; //matrix dimension
  t_matrix R;
  R.size = n;
  //allocate row pointer array for R
  R.space = malloc(n * sizeof(double *));
  if (!R.space) return invalid; //error if allocation failed
  for (int i = 0; i < n; i++) {
    R.space[i] = malloc(n * sizeof(double)); //allocation of columns
    if (!R.space[i])
    {
      //free previously allocated rows if error
      for (int k = 0; k < i; k++)
      {
        free(R.space[k]);
      }
      free(R.space); //free row pointer array
      return invalid;//return invalid matrix
    }

      for (int j = 0; j < n; j++) {
        R.space[i][j] = M.space[i][j]; //copy row i from M into R
      }
    }
    //we already have R=M^1
    //multiply repeatedly to reach M to the power p
    for (int e = 1; e < p; e++) {      // already M^1, do p-1 more multiplies
      t_matrix next = MultiplyMatrix(R, M); //next matrix is matrix R times the original one M
      // free old R to avoid memory leaks
      for (int i = 0; i < n; i++)
      {
        free(R.space[i]);
      }
      free(R.space);
      R = next;

    }

    return R;//return final result matrix
}


t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index){
  t_class component = part.classes[compo_index];
  t_matrix sub_matrix = createMatrixWith0(component.size);
  int n = component.size;
  for (int i = 0; i < n; i++)
  {
    int k = component.vertices[i].identifier -1;
    for (int j = 0; j < n; j++)
    {
      int l = component.vertices[j].identifier - 1;
      sub_matrix.space[i][j] = matrix.space[k][l];
    }

  }
  return sub_matrix;
}








