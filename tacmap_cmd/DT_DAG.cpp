#include <cmath>
#include "DT_DAG.h"



//
// DAG member functions
//
template<int Size>
Triangle * DAG<Size>::find(Vertex & p)
{
	for (int i = 0; i < tri.len(); i++) {
		Triangle * t = tri[i];
		if (t != nullptr && t->isValid() && isContained(*t, p)) {
			return t;
		}
	}
	return nullptr;
}

template<int Size>
Triangle * DAG<Size>::findAdjacent(Vector & e)
{
    Vertex p1 = e.getVertex1();
    Vertex p2 = e.getVertex2();
    
    for (int i = 0; i < tri.len(); i++) {
        Triangle * t = tri[i];
        if (t != nullptr &&
            t->isValid() &&
            t->containsPoint(p1) &&
            t->containsPoint(p2)) {
            return t;
        }
    }
    return nullptr;
}

template<int Size>
void DAG<Size>::divide(Triangle &a, Vertex &pr)
{
    // simple case, pr lies on interior
    // split a into a1,a2,a3
    Vertex & vi = a.getVertexI();
    Vertex & vj = a.getVertexJ();
    Vertex & vk = a.getVertexK();
    
    Triangle * a1 = tri.get();
    Triangle * a2 = tri.get();
    Triangle * a3 = tri.get();

    a1->setVertices(vi, vj, pr);
    a2->setVertices(vj, vk, pr);
    a3->setVertices(vk, vi, pr);
    
    a.addChild(a1);
    a.addChild(a2);
    a.addChild(a3);
    a.setValid(false);
}

template<int Size>
void DAG<Size>::divideOnEdge(Triangle &a, Vector &e, Vertex &pr)
{
    // pr is on edge e
    // need to find triange b that is adjacent to edge e
    // and return 4 triangles
    Vertex &e1 = e.getVertex1();
    Vertex &e2 = e.getVertex2();
    
    
}

template<int Size>
Triangle * DAG<Size>::get()
{
    Triangle *t = tri.get();
    if (t == nullptr)
        fprintf(stdout, "WARNING: DAG::get() pool is empty!\n");

    return t;
}

template<int Size>
void DAG<Size>::removeTriangleContainingPoint(Vertex & a)
{
    Triangle * t = find(a);
    while(t != nullptr) {
        t->setValid(false);
    }
}
