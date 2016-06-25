#pragma once

#include <cstdlib>
#include <string>

#include "DT_Constants.h"
#include "DT_Vertex.h"
#include "DT_Vector.h"
#include "DT_Triangle.h"
#include "DT_TriangleList.h"
#include "DT_Circle.h"
#include "DT_Pool.h"

#include "json.hpp"

using json = nlohmann::json;

class DAG {
private:
    Pool<Triangle, Constants::DAG_SIZE> tri;
    
    void dfsearch(Triangle *a, Vertex *p, Vertex *e = nullptr, bool leftRecursive = true);
    void findSharedEdge(Triangle *ta,
                        Triangle *tb,
                        Vertex **va,
                        Vertex **vb,
                        Vertex *ve[2]);

    void divideOnInterior(Triangle * a, Vertex * pr);
    void divideOnEdge(Triangle * a, Triangle * b, Vertex * p);

public:
    TriangleList<Constants::splitListSize> splitList;
    TriangleList<Constants::adjListSize> adjList;
    
    DAG() {};
    ~DAG() {};
    int len() {
        return tri.len();
    }
    
    void findTriangleContainingPoint(Vertex *p, bool leftRecursive=true);
    void findAdjacentTriangle(Triangle *a, Vertex *p, bool leftRecursive=true);
    void divideOnPoint(Vertex *p);
    void flip(Triangle *a, Triangle *b, Vertex *pr,
              Triangle *n[2]);
	Triangle * get();
    void removeTriangleContainingPoint(Vertex & a);
    
    json to_json();
    void logStep(const char * name,
                 const char * type,
                 Vertex * p,
                 json & j,
                 bool dump_to_file = false);
    void logError(const char * name,
                  const char * type,
                  Vertex * p);

};