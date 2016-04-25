//
//  main.cpp
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/7/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#include <iostream>

#include "DelaunayTriangulation.h"

void testPt(Vector &n, Vertex pr)
{
    fprintf(stdout, "%s is on edge of %s: %d, %lf\n",
            pr.str().c_str(),
            n.str().c_str(),
            n.onEdge(pr),
            n.distance2Point(pr));

}

int main(int argc, const char * argv[]) {
    Vertex a(4.0, 9.0);
    Vertex b(7.0, 5.0);
    Vertex c(10.0, 16.0);
    Vertex p1(7.0, 9.0); // inside
    Vertex p2(5.0, 13.0); // outside
    
    double x = 0.5 * c.getX() + (1.0 - 0.5) * a.getX();
    double y = 0.5 * c.getY() + (1.0 - 0.5) * a.getY();
    Vertex p3(x,y); // along edge a,c
    
    Triangle t(a, b, c);
    
    fprintf(stdout, "isContained(inside): %d\n", isContained(t, p1));
    fprintf(stdout, "isContained(outside): %d\n", isContained(t, p2));
    fprintf(stdout, "isContained(along a,c): %d\n", isContained(t, p3));
    fprintf(stdout, "isContained(coincident with a): %d\n", isContained(t, a));
    fprintf(stdout, "isContained(coincident with b): %d\n", isContained(t, b));
    fprintf(stdout, "isContained(coincident with c): %d\n", isContained(t, c));
    
    Vertex n1(5.0,10.0);
    Vertex n2(10.0,16.0);
    
    Vector n(n1,n2);
    
    testPt(n, Vertex(3.0,17.0));
    testPt(n, Vertex(7.0,17.0));
    testPt(n, Vertex(13.0,17.0));
    testPt(n, Vertex(3.0,12.0));
    testPt(n, Vertex(6.0,12.0));
    testPt(n, Vertex(11.0,12.0));
    testPt(n, Vertex(4.0,9.0));
    testPt(n, Vertex(7.0,9.0));
    testPt(n, Vertex(14.0,9.0));
    testPt(n, Vertex(14.0,9.0));
    
    double tn = 0.5;
    double nx = 0.1 + n1.getX() + tn * (n2.getX() - n1.getX());
    double ny = n1.getY() + tn * (n2.getY() - n1.getY());
    
    testPt(n, Vertex(nx,ny));
    
    return 0;
}
