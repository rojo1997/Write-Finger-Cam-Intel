#include <iomanip>
#include <omp.h>

#include "matrix.h"

// CONSTRUCTORES

Matrix::Matrix (void) {}

Matrix::Matrix (unsigned int d) {
  setdimension (d, d) ;
}

Matrix::Matrix (unsigned int d, float v) {
  setdimension (d, d) ;
  initialize (v) ;
}

Matrix::Matrix (unsigned int r, unsigned int c) {
  setdimension (r, c) ;
}

Matrix::Matrix (unsigned int r, unsigned int c, float v) {
  setdimension (r, c) ;
  initialize (v) ;
}

// MÉTODOS INTERNOS

void Matrix::setdimension (unsigned int r, unsigned int c) {
  m.clear() ; m.resize(r) ;
  for (unsigned int i = 0 ; i != r ; i++) {
    m[i].resize(c) ;
  }
  rows = r ; columns = c ;
}

void Matrix::initialize (float v) {
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      m[i][j] = v ;
    }
  }
}

// SOBRECARGA DE OPERADORES

Matrix Matrix::operator+ (const Matrix & x) const {
  if (rows != x.rows) exit(1) ;
  if (columns != x.columns) exit(1) ;

  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[i][j] + x.m[i][j] ;
    }
  }

  return (y) ;
}

Matrix Matrix::operator- (const Matrix & x) const {
  if (rows != x.rows) exit(1) ;
  if (columns != x.columns) exit(1) ;

  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[i][j] - x.m[i][j] ;
    }
  }

  return (y) ;
}

Matrix Matrix::operator* (const Matrix & x) const {
  if (columns != x.rows) exit(1) ;

  Matrix y (rows, x.columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; ++i) {
    for (unsigned int j = 0 ; j < x.columns ; ++j) {
      for (unsigned int k = 0 ; k < x.rows ; ++k) {
        y.m[i][j] += m[i][k] * x.m[k][j];
      }
    }
  }

  return (y) ;
}

/*Matrix Matrix::operator* (const Matrix & x) const {
  if (columns != x.rows) exit(1) ;

  Matrix y (rows, x.columns, 0) ;

  #pragma omp parallel for
  for (int i = 0 ; i < rows ; ++i) {
    for (int j = 0 ; j < x.columns ; ++j) {
      for (int k = 0 ; k < x.rows ; ++k) {
        y.m[i][j] += m[i][k] * x.m[k][j];
      }
    }
  }
  return (y) ;
}*/

Matrix Matrix::operator+ (const int scalar) const {
  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[i][j] + scalar ;
    }
  }
  return (y) ;
}

Matrix Matrix::operator- (const int scalar) const {
  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[i][j] - scalar ;
    }
  }

  return (y) ;
}

Matrix Matrix::operator* (const int scalar) const {
  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[i][j] * scalar ;
    }
  }

  return (y) ;
}

// Operador =
Matrix & Matrix::operator= (const Matrix & a) {
  if (this != &a) {
    m = a.m ;
    rows = a.rows ;
    columns = a.columns ;
  }
  return (*this) ;
}

// Media de los valores
float Matrix::average (void) const {
  return (sum()/(rows * columns)) ;
}

void Matrix::relu (void) {
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      if (m[i][j] < 0) m[i][j] = 0 ;
    }
  }
}

float Matrix::averageNot0 (void) {
  float sum = 0 ;
  unsigned int n = 0 ;
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      if (m[i][j] > 0) {
        sum += m[i][j] ;
        n++ ;
      }
    }
  }
  return (sum / n) ;
}

// Submatrix
Matrix Matrix::submatrix (unsigned int a, unsigned int b, unsigned int c, unsigned int d) const {
  Matrix y (a - b, c -d) ;
  for (unsigned int i = b ; i < a ; i++) {
    for (unsigned int j = d ; j < c ; j++) {
      y[i-b][j-d] = m[i][j] ;
    }
  }
  return (y) ;
}

// Sumaria
float Matrix::sum (void) const {
  float sum = 0 ;
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      sum += m[i][j] ;
    }
  }
  return (sum) ;
}

// Maximo
pair<unsigned int,unsigned int> Matrix::max (void) const {
  pair<unsigned int, unsigned int> coord (0,0) ;
  float max = 0.0 ;
  for (unsigned int i = 0 ; i < rows ; i++) {
    auto it = (max_element(m[i].begin(), m[i].end())) ;
    float candidate = (*it) ; unsigned int j ;
    if (candidate > max ) {
      max = candidate ;
      j = distance(m[i].begin(), it) ;
      coord = make_pair(i,j) ;
    }
  }
  return (coord) ;
}

list<pair<unsigned int,unsigned int>> Matrix::max_sum_normalize_submatrix (void) const {
  list<pair<unsigned int,unsigned int>> candidates ;
  // Mascara de la matrix
  for(unsigned int i = 0 ; i < rows ; i++) {
    for(unsigned int j = 0; j < columns ; j++) {
      //submatrix(pair(i,j), rows/2).sum() ;
    }
  }
  return (candidates) ;
}

vector<float> & Matrix::operator[] (unsigned int i) {
  return (m[i]) ;
}

vector<float> Matrix::operator[] (unsigned int i) const {
  return (m[i]) ;
}

Matrix Matrix::transpose (void) const {
  Matrix y (rows, columns, 0) ;

  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      y.m[i][j] = m[j][i] ;
    }
  }

  return (y) ;
}

float Matrix::mul_sum (const Matrix & x) const {
  float result = 0 ;
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      result += m[i][j] * x.m[i][j] ;
    }
  }
  return (result) ;
}

float Matrix::operator() (unsigned int i) const {
  float result = 0 ;
  for (unsigned int j = 0 ; j < columns ; j++) {
    result += m[i][j] ;
  }
  return (result) ;
}

void Matrix::print (void) const {
  for (unsigned int i = 0 ; i < rows ; i++) {
    for (unsigned int j = 0 ; j < columns ; j++) {
      cout << setw(5) << m[i][j] ;
    }
    cout << endl ;
  }
}

unsigned int Matrix::getRows (void) {
  return (rows) ;
}

unsigned int Matrix::getColumns (void) {
  return (columns) ;
}