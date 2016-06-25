#pragma once

#include <cstdlib>

#include "DT_Constants.h"
#include "DT_Utils.h"
#include "DT_DAG.h"

class DelaunayTriangulation {
private:
	DAG dag;
    Pool<Vertex, Constants::POOL_SIZE> pts;
    
    Vertex omg1, omg2, omg3;
    double M;
    
	int rPerm[Constants::POOL_SIZE];
	void permute();
	void init();
public:
	DelaunayTriangulation();
	~DelaunayTriangulation();
	void addPt(double x, double y, double z);
    Vertex * getPoint(int i);
    void validEdge(Triangle *a, Vertex *pr);
	void compute();
    
    void logStep(int loop, Vertex * p);
    void logPoints();

};
