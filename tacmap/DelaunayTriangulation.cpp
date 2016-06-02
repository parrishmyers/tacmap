#include "DelaunayTriangulation.h"

void DelaunayTriangulation::permute()
{
    std::srand(pts.len());
    for (int i = 0; i < pts.len(); i++) {
        rPerm[i] = i;
    }
    for (int i = pts.len() - 1; i >= 0; i--) {
        int j = std::rand() % (i + 1);
        int temp = rPerm[i];
        rPerm[i] = rPerm[j];
        rPerm[j] = temp;
    }
}

void DelaunayTriangulation::init()
{
    omg1.set( 3.0 * M, 0.0);
    omg2.set( 0.0, 3.0 * M);
    omg3.set(-3.0 * M, -3.0 * M);
    Triangle * itri = dag.get();
    itri->setVertices(&omg1, &omg2, &omg3);
}

DelaunayTriangulation::DelaunayTriangulation() : M(0)
{
}

DelaunayTriangulation::~DelaunayTriangulation()
{
}

void DelaunayTriangulation::addPt(double x, double y, double z = 0.0)
{
	Vertex * v = pts.get();
	if (nullptr != v) {
		v->set(x, y, z);
        
        int max = (x >= y) ? x : y;
        if (M <= max)
            M = max;
	}
	else {
		fprintf(stdout,"WARNING: Pool of points is empty!\n");
	}
}

Vertex * DelaunayTriangulation::getPoint(int i)
{
    return pts[rPerm[i]];
}

void DelaunayTriangulation::compute()
{
	if (pts.len() < 3) {
		fprintf(stdout, "WARNING: Not enough points added to Pool\n");
		return;
	}

	init();
	permute();

	for (int i = 0; i < pts.len(); i++) {
        Vertex * pr = getPoint(i);
        Triangle * t = dag.find(pr);
        if (nullptr != t) {
            Vector ij = t->getEdgeIJ();
            Vector jk = t->getEdgeJK();
            Vector ki = t->getEdgeKI();
            
            bool onEdgeIJ = ij.onEdge(pr);
            bool onEdgeJK = jk.onEdge(pr);
            bool onEdgeKI = ki.onEdge(pr);
            
            if (onEdgeIJ) {
                
            } else if (onEdgeJK) {
                
            } else if (onEdgeKI) {
                
            } else { // On interier
                
            }
        }
	}
}