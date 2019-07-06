#include "pointT.h"

/*PointT operator+ (const PointT & l,const Point & r) {   
    return {l.first+r,l.second} ;                                 
}*/

void print (const PointT & a) {
    cout << a.first << ":" ; print(a.second) ;
}