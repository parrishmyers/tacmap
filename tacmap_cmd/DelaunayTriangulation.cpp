#include "DelaunayTriangulation.h"

template<int Size>
void DelaunayTriangulation<Size>::permute()
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

template<int Size>
void DelaunayTriangulation<Size>::init()
{
    omg1.set( 3.0 * M, 0.0);
    omg2.set( 0.0, 3.0 * M);
    omg3.set(-3.0 * M, -3.0 * M);
    Triangle itri = dag.get();
    itri.setVertexI(omg1);
    itri.setVertexJ(omg1);
    itri.setVertexK(omg1);
}

template<int Size>
DelaunayTriangulation<Size>::DelaunayTriangulation() : M(0)
{
}

template<int Size>
void DelaunayTriangulation<Size>::addPt(double x, double y)
{
	Vertex * v = pts.get();
	if (nullptr != v) {
		v->set(x, y);
        
        int max = (x >= y) ? x : y;
        if (M <= max)
            M = max;
	}
	else {
		fprintf(stdout,"WARNING: Pool of points is empty!\n");
	}
}

template<int Size>
Vertex const& DelaunayTriangulation<Size>::getPoint(int i)
{
    return pts[rPerm[i]];
}

template<int Size>
void DelaunayTriangulation<Size>::compute()
{
	if (pts.len() < 3) {
		fprintf(stdout, "WARNING: Not enough points added to Pool\n");
		return;
	}

	init();
	permute();

	for (int i = 0; i < pts.len(); i++) {
        Vertex pr = getPoint(i);
        Triangle * t = dag.find(pr);
        if (nullptr != t) {
            if (onEdge) {
            } else { // On interier
                
            }
        }
	}
}