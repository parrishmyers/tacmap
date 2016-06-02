#include <cmath>
#include "DT_DAG.h"

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
            t->containsPoint(p[0]) &&
            t->containsPoint(p[1])) {
            return t;
        }
    }
    return nullptr;
}


void DAG::divide(Triangle * a, Vertex * pr)
{
    // simple case, pr lies on interior
    // split a into a1,a2,a3
    Vertex * v = a->getVertices();
    
    Triangle * a1 = tri.get();
    Triangle * a2 = tri.get();
    Triangle * a3 = tri.get();

    a1->setVertices(&v[0], &v[1], pr);
    a2->setVertices(&v[1], &v[2], pr);
    a3->setVertices(&v[2], &v[0], pr);
    
    a->addChild(a1);
    a->addChild(a2);
    a->addChild(a3);
    a->setValid(false);
}


void DAG::divideOnEdge(Triangle * a, Vector * e, Vertex * pr)
{
    // pr is on edge e
    // need to find triange b that is adjacent to edge e
    
    Triangle * t = nullptr;
    for (int i = 0; i < tri.len(); i++) {
        t = tri[i];
        if (t != nullptr && t->isValid() && t->containsEdge(e)) {
            break;
        }
    }
    // found triange t
    assert(t != nullptr);
    Vertex * tv = t->getVertices();
    
    // split triange and return 4 triangles
    Vertex * av = a->getVertices();
    Vertex * v[5];
    v[0] = pr;
    v[1] = &av[0];
    v[2] = &av[1];
    v[3] = &av[2];
    v[4] = nullptr;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j <= 3; j++) {
            bool found = false;
            if (tv[i] == *v[j]) {
                found = true;
            }
            if (found == false) {
                v[4] = &tv[i];
                break;
            }
        }
    }
    assert(v[4] != nullptr);
    
    //
    // TODO: find the 5 points and assign the new vertices
    //
    
    Triangle * a1 = tri.get();
    Triangle * a2 = tri.get();
    Triangle * a3 = tri.get();
    Triangle * a4 = tri.get();
    
    a1->setVertices(v[0], v[1], v[2]);
    a2->setVertices(v[0], v[1], v[2]);
    a3->setVertices(v[0], v[1], v[2]);
    a4->setVertices(v[0], v[1], v[2]);
    
    a->addChild(a1);
    a->addChild(a2);
    a->addChild(a3);
    a->addChild(a4);
    
    t->addChild(a1);
    t->addChild(a2);
    t->addChild(a3);
    t->addChild(a4);
    
    a->setValid(false);
    t->setValid(false);
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
