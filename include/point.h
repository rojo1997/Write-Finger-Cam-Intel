#ifndef POINT
#define POINT

#include <utility>
#include <cmath>
#include <iostream>

#include "point.h"

using namespace std ;

// Definicion de punto2D
typedef pair<int,int> Point ;

// Operador suma
Point operator+ (const Point &,const Point &) ;

// Operador resta
Point operator- (const Point &,const Point &) ;

// Operador multiplicacion
Point operator* (const Point &,const Point &) ;

// Sum
int sum (const Point &) ;

void print (const Point &) ;


#endif