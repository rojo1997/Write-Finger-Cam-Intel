#ifndef FUNCTIONS
#define FUNCTIONS

#include <list>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <utility>

#include <librealsense2/rs.hpp>

#include "point.h"
#include "pointT.h"
#include "matrix.h"
#include "functions.h"

using namespace std ;
using namespace std::chrono ;
using namespace rs2 ;

extern const float MAX_DISTANCE ;
extern const Point ERROR_SQUARE ;
extern const float ERROR_CIRCLE ;
extern const double TIME_INTERVAL ;
extern const float PERCENTAGE ;

extern unsigned int errors ;

void setMatrix (depth_frame depth, Matrix & m) ;
float euclidean_distance (const Point & a, const Point & b) ;
bool isGoodPoint (Point point, set<PointT> & path) ;
bool isLongStanding (set<PointT> & path) ;
bool isInSquareArea (const Point area, Point b) ;
bool isInCircleArea (const Point area, Point b) ;
vector<PointT> getGesturePath(PointT init, set<PointT> & path) ;
unsigned int detectGesture (const vector<PointT> & gesture) ;
unsigned int detectComplexGest (string & memory) ;
void print (const set<PointT> & l) ;
void print (const vector<PointT> & l) ;

pair<float,float> isRight (const vector<PointT> & gesture) ;
pair<float,float> isLeft (const vector<PointT> & gesture) ;
pair<float,float> isTop (const vector<PointT> & gesture) ;
pair<float,float> isAbove (const vector<PointT> & gesture) ;

bool isPermutation (string one, string generate) ;

#endif