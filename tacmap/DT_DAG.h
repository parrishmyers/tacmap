#pragma once

#include <cstdlib>

#include "DT_Constants.h"
#include "DT_Vertex.h"
#include "DT_Vector.h"
#include "DT_Triangle.h"
#include "DT_Circle.h"
#include "DT_Pool.h"



class DAG {
private:
    Pool<Triangle, Constants::DAG_SIZE> tri;
public:
    DAG() {};
    ~DAG() {};
    int len() {
        return tri.len();
    }
	Triangle * find(Vertex * p);
    Triangle * findAdjacent(Vector * e);
	void divideOnInterior(Triangle * a, Vertex * pr);
    void divideOnEdge(Triangle * a, Triangle * b, Vector * e, Vertex * p);
    void divide(Triangle *a, Vertex *p);
    void flip(Triangle *a, Triangle *b, Vertex *pr);
    void validEdge(Triangle *a, Vertex *pr);
	Triangle * get();
    void removeTriangleContainingPoint(Vertex * a);
};