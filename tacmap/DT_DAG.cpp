#include <cmath>
#include "DT_DAG.h"
#include "DT_Utils.h"

#include <assert.h>

extern FILE * DebugLog;

//
// DAG member functions
//

Triangle *dfsearch(Triangle *a, Vertex *p, bool leftRecursive)
{
    if (nullptr == a)
        return nullptr;
    
    if (true == a->isValid() && isContained(a, p)) {
        return a;
    } else {
        int seq[3] = {0, 1, 2};
        if (!leftRecursive) {
            seq[0] = 2;
            seq[1] = 1;
            seq[2] = 0;
        }
        Triangle * result = nullptr;
        Triangle * child = nullptr;
        
        child = a->getChild(seq[0]);
        result = dfsearch(child,p, leftRecursive);
        if (nullptr != result) return result;
        
        child = a->getChild(seq[1]);
        result = dfsearch(child,p, leftRecursive);
        if (nullptr != result) return result;
        
        child = a->getChild(seq[2]);
        result = dfsearch(child,p, leftRecursive);
        
        return result;
    }
}

Triangle *dfsearch2(Triangle *a, Vertex *p, Vertex *e, bool leftRecursive)
{
    if (nullptr == a)
        return nullptr;
    
    if (true == a->isValid() && isContained(a, p) && isContained(a, e)) {
        return a;
    } else {
        int seq[3] = {0, 1, 2};
        if (!leftRecursive) {
            seq[0] = 2;
            seq[1] = 1;
            seq[2] = 0;
        }
        Triangle * result = nullptr;
        Triangle * child = nullptr;
        
        child = a->getChild(seq[0]);
        result = dfsearch2(child,p, e, leftRecursive);
        if (nullptr != result) return result;
        
        child = a->getChild(seq[1]);
        result = dfsearch2(child,p, e, leftRecursive);
        if (nullptr != result) return result;
        
        child = a->getChild(seq[2]);
        result = dfsearch2(child,p, e, leftRecursive);
        
        return result;
    }
}

void findSharedEdge(Triangle *ta,
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

Triangle * DAG::findTriangleContainingPoint(Vertex *p, bool leftRecursive)
{
    Triangle * head = tri[0];
    return dfsearch(head, p, leftRecursive);
}

Triangle * DAG::findAdjacentTriangle(Triangle *a, Vertex *p, bool leftRecursive)
{
    Vertex * ap[2];
    a->getVerticesNotContainingPoint(p, ap);
    
    Triangle * head = tri[0];
    return dfsearch2(head, ap[0], ap[1], leftRecursive);
}

void DAG::divideOnInterior(Triangle * a, Vertex * p)
{
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
    
    splitList[0] = a1;
    splitList[1] = a2;
    splitList[2] = a3;
    splitList[3] = nullptr;
}


void DAG::divideOnEdge(Triangle * a, Triangle * b, Vertex * p)
{
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
    
    splitList[0] = c1;
    splitList[1] = c2;
    splitList[2] = c3;
    splitList[3] = c4;
}

Triangle ** DAG::divide(Triangle *a, Vertex *p)
{
    if (a->onEdge(p)) {
        Triangle * b = findTriangleContainingPoint(p,false);
        assert(nullptr != b);
        assert(a != b);
        if (nullptr != DebugLog) {
            fprintf(DebugLog,"{'step': 'divide', 'data': {'type': 'edge', 'orig': [");
            a->print();
            fprintf(DebugLog,", ");
            b->print();
            fprintf(DebugLog,"], ");
        }
        divideOnEdge(a, b, p);
    } else {
        if (nullptr != DebugLog) {
            fprintf(DebugLog,"{'step': 'divide', 'data': {'type': 'interior', 'orig': ");
            a->print();
            fprintf(DebugLog,", ");
        }
        divideOnInterior(a, p);
    }
    
    if (nullptr != DebugLog)
        fprintf(DebugLog,"'new': ["); //'0x%lx', '0x%lx', '0x%lx', '0x%lx']}\n",
    for (int i = 0; i < 4; i++) {
        if (nullptr != splitList[i]) {
            if (nullptr != DebugLog)
                splitList[i]->print();
        } else {
            if (nullptr != DebugLog)
                fprintf(DebugLog,"None");
        }
        if (i < 3) {
            if (nullptr != DebugLog)
                fprintf(DebugLog,", ");
        }
    }
    if (nullptr != DebugLog)
        fprintf(DebugLog,"]}}\n");
    return splitList;
}

void DAG::flip(Triangle *a, Triangle *b, Vertex *pr,
                      Triangle *n[2])
{
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
    
    if (nullptr != DebugLog)
        fprintf(DebugLog,"{'step':'flip', 'data': {'orig': ['0x%0lx', '0x%0lX'], 'new': ['0x%0lX', '0x%0lX']}}\n",
                (unsigned long)a,
                (unsigned long)b,
                (unsigned long)n[0],
                (unsigned long)n[1]);
}

///
// ValidEdge(∆, pr,D(P))
// Let ∆adj be the triangle opposite to pr and adjacent to ∆
// if InCircle(∆adj , pr) then
//     (* We have to make an edge flip *)
//     flip(∆,∆adj, pr,D(P))
//     Let ∆′ and ∆′′ be the two new triangles, recursively legalize them
//     ValidEdge(∆′, pr,D(P))
//     ValidEdge(∆′′, pr,D(P))
// end if
///
void DAG::validEdge(Triangle *a, Vertex *pr)
{
    // Let ∆adj be the triangle opposite to pr and adjacent to ∆
    Triangle *b = findAdjacentTriangle(a, pr, false);
    assert( nullptr != b);
    if (a != b) return;
    if (inCircle(a, pr)) {
        Triangle *n[2];
        flip(a,b,pr,n);
        validEdge(n[0], pr);
        validEdge(n[1], pr);
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
    Triangle * t = findTriangleContainingPoint(a);
    while(t != nullptr) {
        t->setValid(false);
    }
}

void DAG::printTree(const char name[])
{
    fprintf(DebugLog,"{'step': 'dag', 'name': '%s', 'tree': [",name);
    long len = tri.len();
    for (int i = 0; i < len; i++) {
        Triangle *a = tri[i];
        a->print();
        if (i < len-1)
            fprintf(DebugLog,", ");
    }
    fprintf(DebugLog,"]}\n");
}

