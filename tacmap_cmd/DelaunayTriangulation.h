#pragma once

#include <cstdlib>

#include "DT_Utils.h"
#include "DT_DAG.h"

#define INITIAL_SIZE 1000

template<int Size = INITIAL_SIZE>
class DelaunayTriangulation {
private:
	DAG<Size> dag;
	Pool<Vertex, Size> pts;
    
    Vertex omg1, omg2, omg3;
    double M;
    
	int rPerm[Size];
	void permute();
	void init();
public:
	DelaunayTriangulation();
	~DelaunayTriangulation();
	void addPt(double x, double y);
    Vertex const& getPoint(int i);
	void compute();
};
