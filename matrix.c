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
  t_class component = part.classes[compo_index]; //get the component (class) at index compo_index
  t_matrix sub_matrix = createMatrixWith0(component.size); //create a submatrix intialized with 0.0 with dimension = number of vertices in component
  int n = component.size; //size of the submatrix

  //for every pair of vertices inside this component
  for (int i = 0; i < n; i++)
  {
    int k = component.vertices[i].identifier -1;//global index k of vertex i in the original matrix, convert 1 based to 0-based
    for (int j = 0; j < n; j++)
    {
      int l = component.vertices[j].identifier - 1; //global index l of vertex j in the original matrix
      sub_matrix.space[i][j] = matrix.space[k][l];
    }

  }
  return sub_matrix;//return the submatrix
}


//Part 3 Step 3


// Compute the Greatest Common Divisor (GCD) of a list of integers.
// This is used to compute the period of a Markov class from all
// the return times we have collected.
int gcd(int *vals, int nbvals) {

  // If there are no values, there is no meaningful GCD.
  // We return 0 as a neutral / undefined value.
  if (nbvals == 0) return 0;

  // Start the GCD computation with the first value of the array.
  int result = vals[0];

  // Process each remaining value and update the running GCD.
  for (int i = 1; i < nbvals; i++) {

    // 'a' is the current GCD, 'b' is the next value to combine with it.
    int a = result;
    int b = vals[i];

    // Euclid's algorithm: repeatedly apply (a, b) ← (b, a % b)
    // until b becomes 0. At that point, 'a' is gcd(previous_result, vals[i]).
    while (b != 0) {
      int tmp = b;   // temporarily store b
      b = a % b;     // remainder of a divided by b
      a = tmp;       // shift b into a
    }

    // Update the running GCD with the result of this step.
    result = a;
  }

  // Final GCD of all numbers in the array.
  return result;
}


// Determines the period of a strongly connected Markov class.
// The class is represented by its transition submatrix `sub_matrix`.
//
// The period is defined as the GCD of all positive integers n
// such that it is possible to return to a state (i.e., some diagonal entry of M^n is > 0).
// We compute powers M^1, M^2, ..., M^n and check their diagonals.
// This is mathematically sufficient because in an n-state SCC,
// all return times occur within the first n multiples.
int getPeriod(t_matrix sub_matrix){
  // Dimension of the submatrix (number of states in the class).
  int n = sub_matrix.size;

  // Array that will store all exponents 'cpt'
  // for which a return to some state occurs (diagonal > 0).
  int *periods = malloc(n * sizeof(int));
  int period_count = 0;               // how many entries we actually store

  // First power: P = sub_matrix^1
  // Create an n×n zero matrix and then copy sub_matrix into it.
  t_matrix P = createMatrixWith0(n);  // allocate P with zeros
  P = CopyMatrix(P, sub_matrix);      // now P = sub_matrix (M^1)

  // 'next' will be used to hold the result of P * sub_matrix.
  t_matrix next;

  // We check powers from 1 up to n (this is enough for SCC periodicity).
  for (int cpt = 1; cpt <= n; cpt++) {

    int diag_nonzero = 0;   // flag: does M^cpt have any positive diagonal entry?

    // Scan the diagonal of the current power matrix P (which is M^cpt).
    for (int i = 0; i < n; i++) {
      // If P[i][i] > 0, there is a path of length cpt from i back to i.
      if (P.space[i][i] > 0.0) {
        diag_nonzero = 1;
      }
    }

    // If at least one diagonal entry is positive, record this exponent.
    if (diag_nonzero) {
      periods[period_count] = cpt;  // store current exponent as a "return time"
      period_count++;               // increment the number of stored periods
    }

    // If we've reached the last needed power, do not compute the next one.
    if (cpt == n) {
      break;
    }

    // Compute next power: next = P * sub_matrix = M^(cpt + 1).
    next = MultiplyMatrix(P, sub_matrix);

    // Free the current matrix P, because we don't need this power anymore.
    // (We must manually free each row, then the row pointer array.)
    for (int i = 0; i < n; i++) {
      free(P.space[i]);
    }
    free(P.space);

    // Update P to be the newly computed power for the next iteration.
    P = next;
  }

  // Compute the period as the GCD of all recorded exponents.
  int period = gcd(periods, period_count);

  // Free the array of stored exponents.
  free(periods);

  // Free the last power matrix P that remains allocated.
  for (int i = 0; i < n; i++) {
    free(P.space[i]);
  }
  free(P.space);

  // Return the computed period of this class.
  return period;
}









