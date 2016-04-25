#pragma once

#include <cstdlib>

#include "DT_Vertex.h"
#include "DT_Vector.h"
#include "DT_Triangle.h"
#include "DT_Circle.h"
#include "DT_Pool.h"

template<int Size>
class DAG {
private:
	Pool<Triangle, Size> tri;
public:
    DAG();
    ~DAG();
	Triangle * find(Vertex & p);
    Triangle * findAdjacent(Vector & e);
	void divide(Triangle &a, Vertex &pr);
    void divideOnEdge(Triangle &a, Vector &e, Vertex &pr);
	Triangle * get();
    void removeTriangleContainingPoint(Vertex & a);
};