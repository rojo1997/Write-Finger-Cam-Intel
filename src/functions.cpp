#include "functions.h"

void setMatrix (rs2::depth_frame depth, Matrix & m) {
    if (depth.get_width() != (int) m.getRows() || depth.get_height() != (int) m.getColumns())
        m.setdimension(depth.get_width(), depth.get_height()) ;
    m.initialize(0.0) ;
    for (unsigned int i = 0 ; i < m.getRows() ; i++) {
        for (unsigned int j = 0 ; j < m.getColumns() ; j++){
            float value = depth.get_distance(i, j) ;
            if (value != 0) m[i][j] = 1/value ; 
        }           
    }              
}

// Calculo de la distancia entre dos puntos
float euclidean_distance (const Point & a, const Point & b) {
    return (sqrt( (float) sum((a-b) * (a-b)))) ;
}

// Si un punto es consecutivo conforme al siguiente actualizamos el recorrido
// Si no lo es apliamos el rango de error para el siguiente
// Posible pobremas si el primero tiene ruido
bool isGoodPoint (Point point, set<PointT> & path) {
    // Caso base
    if (point == pair(0, 0) && path.empty()) return (false) ;
    if (point != pair(0, 0) && path.empty()) return (true) ;
    // Caso general
    bool out = euclidean_distance(point, (--path.end())->second) < (MAX_DISTANCE  /** (errors + 1)*/) ;
    if (out) errors = 0 ;
    else errors++ ;
    return (out) ;
}

//Función que determina si nos quedado parados en un tiempo prefijado por 
//la variable TIME_INTERVAL
bool isLongStanding (set<PointT> & path) {
    // Guardamos el tiempo inicial y el punto sobre el que establecemos el area.
    auto initP = *(--path.end()) ;
    // Calculo del end time para cada punto
    if (double((path.begin()->first - initP.first)/CLOCKS_PER_SEC) < TIME_INTERVAL) return (false) ;
    for (auto it = --(--path.end()) ; it != path.begin() ; it--) {
        // Comprobamos si el punto esta en el area de accion
        if (!isInCircleArea(initP.second, it->second)) break ;
        // Comprobamos si seguimos en el intervalo de tiempo
        double interval = double(initP.first - it->first)/CLOCKS_PER_SEC ;
        if (interval > TIME_INTERVAL) {
            path.erase(it, path.end()) ;
            path.insert(initP) ;
            return (true) ;
        }
    }
    return (false) ;
}

//Función para obtener el path de puntos de un gesto
vector<PointT> getGesturePath(PointT init, set<PointT> & path){
    auto it = path.find (init) ;
    vector<PointT> gesturePath ;
    copy (it, path.end(), back_inserter(gesturePath)) ;
    return (gesturePath) ;
}

//Función para detectar si dos puntos se encuentran dentro de un mismo área en forma de cuadrado
bool isInSquareArea (const Point area, Point b) {
    // Transladamos el area al centro para aprovechar abs
    b = make_pair(abs((int) b.first - (int) area.first), abs( (int) b.second - (int) area.second)) ;
    // Comprobamos sobre el primer cuadrante
    return (ERROR_SQUARE.first > b.first && ERROR_SQUARE.second > b.second) ;
}

//Función para detectar si dos puntos se encuentran dentro de un mismo área en forma de círculo
bool isInCircleArea (const Point area, Point b) {
    return (euclidean_distance (area, b) < ERROR_CIRCLE) ;
}

//Función para detectar gestos básicos
unsigned int detectGesture (const vector<PointT> & gesture) {
    auto right = isRight(gesture) ;
    auto left = isLeft(gesture) ;
    auto top = isTop(gesture) ;
    auto above = isAbove(gesture) ;

    cout << "\tright: " << right.first << endl ;
    cout << "\tleft: " << left.first << endl ;
    cout << "\ttop: " << top.first << endl ;
    cout << "\tabove: " << above.first << endl ;

    vector<float> prob = {right.first, left.first, top.first, above.first} ;

    unsigned int out = 0 ;

    if (right.first == *max_element(prob.begin(), prob.end())) {
        if (right.first > PERCENTAGE) {
            out = 1 ;
        }
    }
    if (left.first == *max_element(prob.begin(), prob.end())) {
        if (left.first > PERCENTAGE) {
            out = 2 ;
        }
    }
    if (top.first == *max_element(prob.begin(), prob.end())) {
        if (top.first > PERCENTAGE) {
            out = 3 ;
        }
    }
    if (above.first == *max_element(prob.begin(), prob.end())) {
        if (above.first > PERCENTAGE) {
            out = 4 ;
        }
    }

    if ((right.first + top.first) / 2 > PERCENTAGE) {
        if (right.second / top.second < 2 && right.second / top.second > 0.5) {
            out = 5 ;
        }
        
    }
    if ((left.first + top.first) / 2 > PERCENTAGE) {
        if (left.second / top.second < 2 && left.second / top.second > 0.5) {
            out = 6 ;
        }
    }
    if ((right.first + above.first) / 2 > PERCENTAGE) {
        if (right.second / above.second < 2 && right.second / above.second > 0.5) {
            out = 7 ;
        }
    }
    if ((left.first + above.first) / 2 > PERCENTAGE) {
        if (left.second / above.second < 2 && left.second / above.second > 0.5) {
            out = 8 ;
        }
    }

    return (out) ;
}

//Función para detectar gestos complejos
unsigned int detectComplexGest (string & memory) {
    cout << "Memoria: " << memory << endl ;
    unsigned int out = 0 ;
    if (isPermutation(memory, "7865")) {
        cout << "Rombo detectado" << endl ; 
        memory.clear() ;
        out = 1 ;
    }

    if (isPermutation(memory, "4132")) {
        cout << "Cuadrado detectado" << endl ; 
        memory.clear() ;
        out = 2 ;
    }
    if (isPermutation(memory, "527")) {
        cout << "Triangulo detectado" << endl ;
        memory.clear() ;
        out = 3 ;
    }
    if (isPermutation(memory, "53628471")) {
        cout << "Circulo detectado" << endl ;
        memory.clear() ;
        out = 4 ;
    }
    return (out) ;
}

//Detecta si el movimiento es hacia la derecha
pair<float,float> isRight (const vector<PointT> & gesture) {
    float one_point = gesture[0].second.first > gesture[gesture.size()-1].second.first? 1.0 : 0.0 ;

    int count = 0 ;
    for (int i = 0 ; i < ((int)gesture.size() - 2) ; i++) {
        if (gesture[i].second.first >= gesture[i+2].second.first) count++ ;
    }
    float value = (one_point + (float) count / gesture.size()) / 2.0 ;
    float distance = gesture[0].second.first - gesture[gesture.size()-1].second.first ;
    return (pair(value, distance)) ;
}

pair<float,float> isLeft (const vector<PointT> & gesture) {
    float one_point = gesture[0].second.first < gesture[gesture.size()-1].second.first? 1.0 : 0.0 ;

    int count = 0 ;
    for (int i = 0 ; i < ((int)gesture.size() - 2) ; i++) {
        if (gesture[i].second.first <= gesture[i+2].second.first) count++ ;
    }
    float value = (one_point + (float) count / gesture.size()) / 2.0 ;
    float distance =  gesture[gesture.size()-1].second.first - gesture[0].second.first ;
    return (pair(value, distance)) ;
}

pair<float,float> isTop (const vector<PointT> & gesture) {
    float one_point = gesture[0].second.second > gesture[gesture.size()-1].second.second? 1.0 : 0.0 ;

    int count = 0 ;
    for (int i = 0 ; i < ((int)gesture.size() - 2) ; i++) {
        if (gesture[i].second.second >= gesture[i+2].second.second) count++ ;
    }
    float value = (one_point + (float) count / gesture.size()) / 2.0 ;
    float distance = gesture[0].second.second - gesture[gesture.size()-1].second.second ;
    return (pair(value, distance)) ;
}

pair<float,float> isAbove (const vector<PointT> & gesture) {
    float one_point = gesture[0].second.second < gesture[gesture.size()-1].second.second? 1.0 : 0.0 ;

    int count = 0 ;
    for (int i = 0 ; i < ((int)gesture.size() - 2) ; i++) {
        if (gesture[i].second.second <= gesture[i+2].second.second) count++ ;
    }
    float value = (one_point + (float) count / gesture.size()) / 2.0 ;
    float distance = gesture[gesture.size()-1].second.second - gesture[0].second.second ;
    return (pair(value, distance)) ;
}

void print (const set<PointT> & l) {
    cout << "[" ;
    for (auto i : l) {
        print(i) ; cout << ", " ;

    }
    cout << "]" << endl ;
}

void print (const vector<PointT> & l) {
    cout << "[" ;
    for (auto i : l) {
        print(i) ; cout << ", " ;

    }
    cout << "]" << endl ;
}

bool isPermutation (string one, string generate) {
    if (one.size() != generate.size()) return (false) ;
    for (unsigned int i = 0 ; i < generate.size() ; i++){
        if (one == generate) return (true) ;
        reverse (one.begin(), one.end()) ;
        if (one == generate) return (true) ;
        reverse (one.begin(), one.end()) ;
        rotate (one.begin(), one.begin() + 1, one.end()) ;
    }
    return (false) ;
}