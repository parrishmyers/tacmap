#pragma once

#include <cstdlib>
#include <tuple>

#include "DT_Constants.h"
#include "DT_Vertex.h"
#include "DT_Vector.h"
#include "DT_Triangle.h"
#include "DT_Circle.h"
#include "DT_Pool.h"

class DAG {
private:
    Pool<Triangle, Constants::DAG_SIZE> tri;
    Triangle * splitList[4];
public:
    DAG() {};
    ~DAG() {};
    int len() {
        return tri.len();
    }
    Triangle * findTriangleContainingPoint(Vertex *p, bool leftRecursive=true);
    Triangle * findAdjacentTriangle(Triangle *a, Vertex *p, bool leftRecursive=true);
	void divideOnInterior(Triangle * a, Vertex * pr);
    void divideOnEdge(Triangle * a, Triangle * b, Vertex * p);
    Triangle ** divide(Triangle *a, Vertex *p);
    void flip(Triangle *a, Triangle *b, Vertex *pr,
              Triangle *n[2]);
    void validEdge(Triangle *a, Vertex *pr);
	Triangle * get();
    void removeTriangleContainingPoint(Vertex * a);
    
    void printTree(const char name[]);
};