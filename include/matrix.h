#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <omp.h>

using namespace std ;

class Matrix {
  public:
    Matrix (void) ;
    Matrix (unsigned int) ;
    Matrix (unsigned int, float) ;
    Matrix (unsigned int, unsigned int) ;
    Matrix (unsigned int, unsigned int, float) ;

    // Sobrecarga de operadores matrix to matrix
    Matrix operator+ (const Matrix &) const ;
    Matrix operator- (const Matrix &) const ;
    Matrix operator* (const Matrix &) const ;

    // Sobrecarga de operadores matrix to scalar
    Matrix operator+ (const int) const ;
    Matrix operator- (const int) const ;
    Matrix operator* (const int) const ;

    // Operador =
    Matrix & operator= (const Matrix &) ;

    // Media de los valores
    float average (void) const ;

    // Submatrix
    Matrix submatrix (unsigned int, unsigned int, unsigned int, unsigned int) const ;
    Matrix submatrix (pair<unsigned int, unsigned int> coord, unsigned int d) const ;

    // Sum
    float sum (void) const ;

    // Relu
    void relu (void) ;

    float averageNot0 (void) ;

    pair<unsigned int,unsigned int> max (void) const ;

    list<pair<unsigned int,unsigned int>> max_sum_normalize_submatrix (void) const ;

    vector<float> & operator[] (unsigned int) ;
    vector<float> operator[] (unsigned int) const ;
    Matrix transpose (void) const ;
    float mul_sum (const Matrix &) const ;
    float operator() (unsigned int i) const ;

    void setdimension (unsigned int r, unsigned int c) ;
    void initialize (float v) ;

    unsigned int getRows (void) ;
    unsigned int getColumns (void) ;

    void print (void) const ;

  private:
    vector<vector<float>> m ;

    unsigned int rows ;
    unsigned int columns ;
};

/*
iniciar la hebras de pragma en en el constructor y eliminarlas en el destructor
*/

#endif
