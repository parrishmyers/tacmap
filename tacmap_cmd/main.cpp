//
//  main.cpp
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/7/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#include <iostream>

#include "DelaunayTriangulation.h"

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
    
    return 0;
}
