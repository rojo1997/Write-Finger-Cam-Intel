// Biblioteca de RealSense
#include <librealsense2/rs.hpp>

#include <iostream>             // cin cout
// DATA STRUCTURES
#include <map>
#include <string>
#include <vector>
#include <list>

#include <algorithm>            // max_element()
#include <cmath>                // abs() sqrt()
// SYNCHRONIZE FUNCTIONS
#include <thread>
#include <atomic>
#include <sstream>
#include <chrono>               // high_resolution_clock::now()
#include <ctime>

#include "matrix.h"
#include "point.h"
#include "pointT.h"
#include "functions.h"

using namespace std ;
using namespace std::chrono ;
using namespace std::this_thread ;
using namespace rs2 ;
//using namespace cv ;

// CONSTANTES
// Numero de puntos que recordamos
const unsigned int MAX_POINT = 1000 ;
// Umbral para distancia Euclidea
const float MAX_DISTANCE = 200 ;
// Intervalo de tiempo de deteccion
const double TIME_INTERVAL = 1.2 ;
// ERROR DE MOVIMIENTO Cuadrado
const Point ERROR_SQUARE = make_pair(75, 75) ;
// ERROR DE MOVIMIENTO Circulo
const float ERROR_CIRCLE = 45.0 ;
// PORCENTAGE DE FIABILIDAD DEL GESTO
const float PERCENTAGE = 0.725 ;

// VARIABLES GLOBAL
unsigned int errors = 0 ;



int main (/*int argc, char** argv*/) {
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p ;

    // Configure and start the pipeline
    p.start() ;

    // Matriz de profundidades
    Matrix matrix ;

    // Recorrido de objeto detectado en el tiempo
    set<PointT> path ;

    // gesture memory
    string memory = "" ;

    PointT init = pair(0, pair(0,0)), end ;

    while (true) {
        // Block program until frames arrive
        frameset frames = p.wait_for_frames();

        // Extraemos la profundidad de la imagen 
        depth_frame depth = frames.get_depth_frame() ;

        // Extraemos el color de la imagen
        video_frame video = frames.get_color_frame() ;

        // Generamos la matriz
        setMatrix (depth, matrix) ;

        // Posible en caso de usar la profundida como una coordenada mas
        //matrix = (matrix - matrix.averageNot0()) ;
        //matrix.relu() ;

        //matrix.print() ;

        // Extraemos los puntos clave
        auto candidate = matrix.max() ;
        // auto candidates = matrix.max_sum_normalize_submatrix () ;

        
        if (isGoodPoint(candidate, path)) {
            path.insert(pair(clock(), candidate)) ;
            
            if (isLongStanding(path)) {

                init = *(path.begin()) ;
                end = *(--path.end()) ;

                vector<PointT> gesturePath ;
                copy (path.begin(), path.end(), back_inserter(gesturePath)) ;

                unsigned int easy_gest = detectGesture (gesturePath) ;

                cout << "EASY_GEST: " << easy_gest << endl ;

                if (easy_gest != 0) memory += to_string(easy_gest) ;

                unsigned int complex_gest = detectComplexGest (memory) ;

                cout << "COMPLEX_GEST: " << complex_gest << endl ;


                path.clear() ; path.insert(end) ;
            }
            //print(*(--path.end())) ; cout << endl ;
        }
        if (path.size() == MAX_POINT) path.erase(path.begin()) ;
    }

    return (0) ;
}