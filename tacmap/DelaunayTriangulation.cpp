
#include <fstream>

#include <boost/format.hpp>

#include "DelaunayTriangulation.h"
#include "DT_Circle.h"

bool isContained(Triangle * t, Vertex * p);
bool inCircle(Triangle *a, Vertex * pr);
bool inCircle(Vertex * a, Vertex * b, Vertex * c, Vertex * d);
Circle circleForPoints(Vertex * a, Vertex * b, Vertex * c);
Vertex pointAlongLine2D(Vertex * a, Vertex * b, double tn = 0.5);
bool onEdge(Vertex * a, Vertex * b, Vertex * pr);
double distance2Point(Vertex * a, Vertex * b, Vertex * pr);


//
// Friend functions of Triangle & Vertex
//
bool isContained(Triangle * t, Vertex * p) {
    double x = p->getX();
    double y = p->getY();
    
    double x1 = t->data[0]->getX();
    double y1 = t->data[0]->getY();
    
    double x2 = t->data[1]->getX();
    double y2 = t->data[1]->getY();
    
    double x3 = t->data[2]->getX();
    double y3 = t->data[2]->getY();
    
    double denom = 1.0 / (x1*(y2 - y3) + y1*(x3 - x2) + x2*y3 - y2*x3);
    
    double t1 = (x*(y3 - y1) + y*(x1 - x3) - x1*y3 + y1*x3) *  denom;
    double t2 = (x*(y2 - y1) + y*(x1 - x2) - x1*y2 + y1*x2) * -denom;
    
    if (0.0 <= t1 && t1 <= 1.0 &&
        0.0 <= t2 && t2 <= 1.0 &&
        t1 + t2 <= 1.0)
        return true;
    else
        return false;
}

bool inCircle(Triangle *a, Vertex * pr)
{
    Vertex ** p = a->getVertices();
    return inCircle(p[0],p[1],p[2],pr);
}

bool inCircle(Vertex * a, Vertex * b, Vertex * c, Vertex * d)
{
    double xa = a->getX();
    double xb = b->getX();
    double xc = c->getX();
    double xd = d->getX();
    
    double ya = a->getY();
    double yb = b->getY();
    double yc = c->getY();
    double yd = d->getY();
    
    double sa = xa * xa + ya * ya;
    double sb = xb * xb + yb * yb;
    double sc = xc * xc + yc * yc;
    double sd = xd * xd + yd * yd;
    
    double det = 0.0;
    det  = xa * ( yb * (sc - sd) - sb * (yc - yd) + (yc * sd - yd * sc) );
    det -= ya * ( xb * (sc - sd) - sb * (xc - xd) + (xc * sd - xd * sc) );
    det += sa * ( xb * (yc - yd) - yb * (xc - xd) + (xc * yd - xd * yc) );
    det -=      ( xb * (yc * sd - yd * sc) - yb * (xc * sd - xd * sc) + sb * (xc * yd - xd * yc) );
    
    if (det > 0.0)
        return true;
    else
        return false;
}

Circle circleForPoints(Vertex * a, Vertex * b, Vertex * c)
{
    double x1 = a->getX();
    double x2 = b->getX();
    double x3 = c->getX();
    
    double y1 = a->getY();
    double y2 = b->getY();
    double y3 = c->getY();
    
    double X2 = (double)(x2-x1);
    double X3 = (double)(x3-x1);
    double Y2 = (double)(y2-y1);
    double Y3 = (double)(y3-y1);
    
    double alpha = X3 / X2;
    
    double bx2 = (x2+x1) * X2;
    double bx3 = (x3+x1) * X3;
    double by2 = (y2+y1) * Y2;
    double by3 = (y3+y1) * Y3;
    
    double h = 0.0;
    double k = 0.0;
    double r = 0.0;
    
    k = bx3 + by3 - alpha * (bx2 + by2);
    k /= 2 * (Y3 - alpha * Y2);
    
    h = bx2 + by2 - 2 * k * Y2;
    h /= 2 * X2;
    
    r = sqrt( (x1 - h)*(x1 - h) + (y1 - k)*(y1 - k) );
    
    return Circle(h,k,r);
}

Vertex pointAlongLine2D(Vertex * a, Vertex * b, double tn)
{
    double nx = a->getX() + tn * (b->getX() - a->getX());
    double ny = a->getY() + tn * (b->getY() - a->getY());
    return Vertex(nx,ny);
}

bool onEdge(Vertex * a, Vertex * b, Vertex * pr) {
    double minx = (a->getX() <= b->getX())? a->getX() : b->getX();
    double maxx = (a->getX() >= b->getX())? a->getX() : b->getX();
    
    double miny = (a->getY() <= b->getY())? a->getY() : b->getY();
    double maxy = (a->getY() >= b->getY())? a->getY() : b->getY();
    
    if (pr->getX() < minx)
        return false;
    else if (pr->getX() > maxx)
        return false;
    else if (pr->getY() < miny)
        return false;
    else if (pr->getY() > maxy)
        return false;
    else
        return distance2Point(a,b,pr) < Constants::DIST_THRESH;
}

double distance2Point(Vertex * a, Vertex * b, Vertex * pr) {
    double nx = b->getX() - a->getX();
    double ny = b->getY() - a->getY();
    
    double nmag = sqrt(nx * nx + ny * ny);
    
    double px = pr->getX() - a->getX();
    double py = pr->getY() - a->getY();
    
    //double pmag = sqrt(px * px + py * py);
    
    double dnp = nx * px + ny * py; // dot(n,p)
    
    double dnpx = nx * dnp / (nmag * nmag);
    double dnpy = ny * dnp / (nmag * nmag);
    
    double perx = dnpx - px;
    double pery = dnpy - py;
    
    return sqrt(perx * perx + pery * pery);
}


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
    int rPerm_[10] = {2, 9, 7, 8, 4, 1, 3, 0, 6, 5};
    return pts[rPerm_[i]];
    //return pts[rPerm[i]];
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