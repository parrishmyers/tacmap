#include <assert.h>
#include <cmath>
#include <fstream>

#include <boost/format.hpp>

#include "DT_DAG.h"

//
// DAG member functions
//

void DAG::dfsearch(Triangle *a, Vertex *p, Vertex *e, bool leftRecursive)
{
    if (nullptr == a || nullptr == p) {
        return;
    } else if (true == a->isValid() && isContained(a, p)) {
        if (nullptr == e) {
             adjList.add(a);
        } else {
            if (isContained(a, e))
                adjList.add(a);
        }
    } else {
        int seq[3] = {0, 1, 2};
        if (!leftRecursive) {
            seq[0] = 2;
            seq[1] = 1;
            seq[2] = 0;
        }
        Triangle * child = nullptr;
        
        child = a->getChild(seq[0]);
        dfsearch(child,p, e, leftRecursive);
        
        child = a->getChild(seq[1]);
        dfsearch(child,p, e, leftRecursive);
        
        child = a->getChild(seq[2]);
        dfsearch(child,p, e, leftRecursive);
    }
}

void DAG::findSharedEdge(Triangle *ta,
                         Triangle *tb,
                         Vertex **va,
                         Vertex **vb,
                         Vertex *ve[2])
{
    // find the edge shared between the 2 triangles
    Vertex ** avt = ta->getVertices();
    Vertex ** bvt = tb->getVertices();
    
    Vertex * ep[2] = {nullptr,nullptr};
    Vertex * ap = nullptr;
    Vertex * bp = nullptr;
    
    int order_a[3] = {-1,-1,-1};
    int order_b[3] = {-1,-1,-1};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (avt[i] == bvt[j]) {
                order_a[i] = j;
                order_b[j] = i;
            }
        }
    }
    
    for (int i = 0, j = 0; i < 3; i++) {
        if (order_a[i] < 0) {
            ap = avt[i];
        } else {
            ep[j] = avt[i];
            j += 1;
        }
        if (order_b[i] < 0)
            bp = bvt[i];
    }
    
    *va = ap;
    *vb = bp;
    ve[0] = ep[0];
    ve[1] = ep[1];
}

void DAG::findTriangleContainingPoint(Vertex *p, bool leftRecursive)
{
    Triangle * head = tri[0];
    adjList.clear();
    dfsearch(head, p, nullptr, leftRecursive);
}

void DAG::findAdjacentTriangle(Triangle *a, Vertex *p, bool leftRecursive)
{
    Vertex * ap[2];
    a->getVerticesNotContainingPoint(p, ap);
    
    Triangle * head = tri[0];
    adjList.clear();
    adjList.add(a);
    dfsearch(head, ap[0], ap[1], leftRecursive);
}

void DAG::divideOnInterior(Triangle * a, Vertex * p)
{
    json j;
    logStep("divide_interior", "pre", p, j, false);
    
    // simple case, pr lies on interior
    // split a into a1,a2,a3
    Vertex ** v = a->getVertices();
    
    Triangle * a1 = tri.get();
    Triangle * a2 = tri.get();
    Triangle * a3 = tri.get();

    a1->setVertices(v[0], v[1], p);
    a2->setVertices(v[1], v[2], p);
    a3->setVertices(v[2], v[0], p);
    
    a->addChild(a1);
    a->addChild(a2);
    a->addChild(a3);
    a->setValid(false);
    
    splitList.clear();
    splitList.add(a1);
    splitList.add(a2);
    splitList.add(a3);

    logStep("divide_interior", "post", p, j, true);
}


void DAG::divideOnEdge(Triangle * a, Triangle * b, Vertex * p)
{
    json j;
    logStep("divide_edge", "pre", p, j, false);
    
    Vertex * ep[2] = {nullptr,nullptr};
    Vertex * ap = nullptr;
    Vertex * bp = nullptr;
    
    findSharedEdge(a, b, &ap, &bp, ep);
    
    Triangle * c1 = tri.get();
    Triangle * c2 = tri.get();
    Triangle * c3 = tri.get();
    Triangle * c4 = tri.get();
    
    c1->setVertices(p, ep[0], ap);
    c2->setVertices(p, ep[1], ap);
    
    a->addChild(c1);
    a->addChild(c2);
    a->setValid(false);
    
    c3->setVertices(p, ep[0], bp);
    c4->setVertices(p, ep[1], bp);
    
    b->addChild(c3);
    b->addChild(c4);
    b->setValid(false);
    
    splitList.clear();
    splitList.add(c1);
    splitList.add(c2);
    splitList.add(c3);
    splitList.add(c4);
    
    logStep("divide_edge", "post", p, j, true);
}

void DAG::flip(Triangle *a, Triangle *b, Vertex *pr,
                      Triangle *n[2])
{
    json j;
    logStep("flip", "pre", pr, j, false);
    
    Vertex * ep[2] = {nullptr,nullptr};
    Vertex * ap = nullptr;
    Vertex * bp = nullptr;
    
    findSharedEdge(a, b, &ap, &bp, ep);
    
    n[0] = tri.get();
    n[1] = tri.get();
    
    n[0]->setVertices(ap, bp, ep[0]);
    n[1]->setVertices(ap, bp, ep[1]);
    
    a->addChild(n[0]);
    a->addChild(n[1]);
    a->setValid(false);

    b->addChild(n[0]);
    b->addChild(n[1]);
    b->setValid(false);
    
    logStep("flip", "post", pr, j, true);
}

Triangle * DAG::get()
{
    Triangle *t = tri.get();
    if (t == nullptr)
        fprintf(stdout, "WARNING: DAG::get() pool is empty!\n");

    return t;
}


void DAG::removeTriangleContainingPoint(Vertex & a)
{
    for (int i = 0; i < tri.len(); i++) {
        if (0 <= tri[i]->containsPoint(a))
            tri[i]->setValid(false);
    }
}

json DAG::to_json() {
    json j;
    for (int i = 0; i < tri.len(); i++) {
        Triangle *a = tri[i];
        j.push_back( a->to_json() );
    }
    return j;
}

void DAG::logStep(const char * name,
                  const char * type,
                  Vertex * p,
                  json & j,
                  bool dump_to_file)
{
    static int len = -1;
    json j1;
    j1["name"] = name;
    j1["type"] = type;
    j1["point"] = p->to_json();
    j1["length"] = tri.len();
    j1["dag"] = to_json();
    
    j.push_back(j1);
    
    if (true == dump_to_file) {
        std::ofstream log;
        log.open( str( boost::format("step_%05d_%s_%s.json") % len % type % name ) );
        log << j.dump();
        log.close();
    } else {
        len = tri.len();
    }
}

void DAG::logError(const char * name,
                   const char * type,
                   Vertex * p)
{
    json j1;
    j1["name"] = name;
    j1["type"] = type;
    j1["point"] = p->to_json();
    j1["length"] = tri.len();
    j1["dag"] = to_json();
    for (int i = 0; i < adjList.len; i++) {
        j1["error"].push_back(adjList[i]->to_json());
    }
    
    std::ofstream log;
    log.open( str( boost::format("step_%05d_%s_%s.json") % tri.len() % type % name ) );
    log << j1.dump();
    log.close();
}
