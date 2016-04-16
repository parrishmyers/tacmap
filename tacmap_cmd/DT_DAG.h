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
	void divide(Triangle &a);
	Triangle * get();
    void removeTriangleContainingPoint(Vertex & a);
};