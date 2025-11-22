//
// Created by jeann on 21/11/2025.
//

#ifndef MATRIX_H
#define MATRIX_H
double **createMatrix(AdjacencyList adj);
double **createMatrixWith0(int size);
double **CopyMatrix(double **M1, double **M2, int size);
double **MultiplyMatrix(double **M1, double **M2, int size);
double differenceMatrix(double **M1, double **M2, int size);
#endif //MATRIX_H
