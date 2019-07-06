#include <point.h>
                                                 
Point operator+ (const Point & l,const Point & r) {   
    return {l.first+r.first,l.second+r.second} ;                                 
}
                                          
Point operator- (const Point & l,const Point & r) {   
    return {l.first-r.first,l.second-r.second} ;                                 
}

Point operator* (const Point & l,const Point & r) {   
    return {l.first*r.first,l.second*r.second} ;                                 
}

int sum (const Point & a) {
    return (a.first + a.second) ;
}

void print (const Point & a) {
    cout << a.first << ":" << a.second ;
}