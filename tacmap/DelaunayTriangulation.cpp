
#include <fstream>

#include <boost/format.hpp>

#include "DelaunayTriangulation.h"
#include "DT_Circle.h"

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
void DelaunayTriangulation::validEdge(Triangle *a, Vertex *pr)
{
    TriangleList<Constants::adjListSize> copy;
    
    // Let ∆adj be the triangle opposite to pr and adjacent to ∆
    dag.findAdjacentTriangle(a, pr, false);
    copy.copy(dag.adjList);
    if (copy.len > 1) {
        
        Vertex ** points = copy[1]->getVertices();
        Circle c = circleForPoints(points[0], points[1], points[2]);
        bool answer1 = c.pointInside(pr->getX(), pr->getY());
        bool answer2 = inCircle(copy[1], pr);
        
        if (answer1 != answer2) {
            fprintf(stdout, "WARNING: inCircle produced wrong answer.\n");
            fprintf(stdout, "\t%s\n", c.to_json().dump().c_str());
            fprintf(stdout, "\t%s\n", a->to_json().dump().c_str());
            fprintf(stdout, "\t%s\n", pr->to_json().dump().c_str());
        }
        
        if (answer1) {
            Triangle *n[2];
            dag.flip(copy[0],copy[1],pr,n);
            validEdge(n[0], pr);
            validEdge(n[1], pr);
        }
    }
}

void DelaunayTriangulation::compute()
{
	if (pts.len() < 3) {
		fprintf(stdout, "WARNING: Not enough points added to Pool\n");
		return;
	}

	init();
	permute();
    
    logPoints();
    json j;
    
	for (int i = 0; i < pts.len(); i++) {
        Vertex * p = getPoint(i);

        logStep(i, p);
        
        dag.findTriangleContainingPoint(p);
        if (2 == dag.adjList.len) { // on edge
            dag.divideOnEdge(dag.adjList[0], dag.adjList[1], p);
        } else { // on interior
            dag.divideOnInterior(dag.adjList[0], p);
        }
        
        
        TriangleList<Constants::splitListSize> copy;
        copy.copy(dag.splitList);
        for (int i = 0; i < copy.len; i++) {
            assert(true == copy[i]->isValid());
            validEdge(copy[i], p);
        }
	}
    
    //
    // removing all triangles with the initial fake points added to DAG.
    //
    dag.removeTriangleContainingPoint(omg1);
    dag.removeTriangleContainingPoint(omg2);
    dag.removeTriangleContainingPoint(omg3);
    json sol = dag.to_json();
    std::ofstream log;
    log.open ("solution.json");
    log << sol.dump();
    log.close();
}

void DelaunayTriangulation::logStep(int loop, Vertex * p)
{
    json j;
    j["loop"] = loop;
    j["point"] = p->to_json();
    j["dag"] = dag.len();

    std::ofstream log;
    log.open (str(boost::format("loop_%05d.json") % dag.len()));
    log << j.dump();
    log.close();
}

void DelaunayTriangulation::logPoints()
{
    std::ofstream log;
    log.open ("points.json");
    json j;
    j.push_back(omg1.to_json());
    j.push_back(omg2.to_json());
    j.push_back(omg3.to_json());
    for (int i = 0; i < pts.len(); i++) {
        j.push_back( pts[i]->to_json() );
    }
    log << j.dump();
    log.close();
}