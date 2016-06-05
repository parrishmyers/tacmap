#include <cmath>
#include "DT_DAG.h"
#include "DT_Utils.h"

#include <assert.h>

//
// DAG member functions
//
Triangle * DAG::find(Vertex * p)
{
	for (int i = 0; i < tri.len(); i++) {
		Triangle * t = tri[i];
		if (t != nullptr && t->isValid() && isContained(t, p)) {
			return t;
		}
	}
	return nullptr;
}


Triangle * DAG::findAdjacent(Vector * e)
{
    Vertex * p = e->getVertices();
    
    for (int i = 0; i < tri.len(); i++) {
        Triangle * t = tri[i];
        if (t != nullptr &&
            t->isValid() &&
            t->containsPoint(p[0]) >= 0 &&
            t->containsPoint(p[1]) >= 0) {
            return t;
        }
    }
    return nullptr;
}


void DAG::divideOnInterior(Triangle * a, Vertex * p)
{
    // simple case, pr lies on interior
    // split a into a1,a2,a3
    Vertex * v = a->getVertices();
    
    Triangle * a1 = tri.get();
    Triangle * a2 = tri.get();
    Triangle * a3 = tri.get();

    a1->setVertices(&v[0], &v[1], p);
    a2->setVertices(&v[1], &v[2], p);
    a3->setVertices(&v[2], &v[0], p);
    
    a->addChild(a1);
    a->addChild(a2);
    a->addChild(a3);
    a->setValid(false);
}


void DAG::divideOnEdge(Triangle * a, Triangle * b, Vector * e, Vertex * p)
{
    Vertex * ep = e->getVertices();
    Vertex * ap = a->pointNotOnEdge(e);
    Vertex * bp = b->pointNotOnEdge(e);
    
    Triangle * c1 = tri.get();
    Triangle * c2 = tri.get();
    Triangle * c3 = tri.get();
    Triangle * c4 = tri.get();
    
    c1->setVertices(p, &ep[0], ap);
    c2->setVertices(p, &ep[1], ap);
    
    a->addChild(c1);
    a->addChild(c2);
    a->setValid(false);
    
    c3->setVertices(p, &ep[0], bp);
    c4->setVertices(p, &ep[1], bp);

    b->addChild(c3);
    b->addChild(c4);
    b->setValid(false);
}

void DAG::divide(Triangle *a, Vertex *p)
{
    Vertex nonEdgePoint;
    Vector edgeForPoint;
    if (a->edgeForPoint(p, &edgeForPoint, &nonEdgePoint)) { // on edge
        // find adjacent triangle that shares an edge with first triangle
        Triangle *b = findAdjacent(&edgeForPoint);
        assert(b != nullptr);
        
        divideOnEdge(a, b, &edgeForPoint, p);
    } else { // On interier
        divideOnInterior(a, p);
    }
}

void flip(Triangle *a, Triangle *b, Vertex *pr)
{
    
}

// ValidEdge(∆, pr,D(P))
// Let ∆adj be the triangle opposite to pr and adjacent to ∆
// if InCircle(∆adj , pr) then
//     (* We have to make an edge flip *)
//     flip(∆,∆adj, pr,D(P))
//     Let ∆′ and ∆′′ be the two new triangles, recursively legalize them
//     ValidEdge(∆′, pr,D(P))
//     ValidEdge(∆′′, pr,D(P))
// end if
void DAG::validEdge(Triangle *a, Vertex *pr)
{
    // Let ∆adj be the triangle opposite to pr and adjacent to ∆
    if (inCircle(a, pr)) {
        // make an edge flip
    }
}

Triangle * DAG::get()
{
    Triangle *t = tri.get();
    if (t == nullptr)
        fprintf(stdout, "WARNING: DAG::get() pool is empty!\n");

    return t;
}


void DAG::removeTriangleContainingPoint(Vertex * a)
{
    Triangle * t = find(a);
    while(t != nullptr) {
        t->setValid(false);
    }
}
