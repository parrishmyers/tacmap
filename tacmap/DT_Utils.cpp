//
//  DT_Utils.cpp
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/10/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#include <cmath>

#include "DT_Utils.h"

//
// Friend functions of Triangle & Vertex
//
bool isContained(Triangle * t, Vertex * p) {
    double x = p->getX();
    double y = p->getY();
    
    double x1 = t->data[0].getX();
    double y1 = t->data[0].getY();
    
    double x2 = t->data[1].getX();
    double y2 = t->data[1].getY();
    
    double x3 = t->data[2].getX();
    double y3 = t->data[2].getY();
    
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

Vertex pointAlongLine2D(Vertex * a, Vertex * b, double tn = 0.5)
{
    double nx = a->getX() + tn * (b->getX() - a->getX());
    double ny = a->getY() + tn * (b->getY() - a->getY());
    return Vertex(nx,ny);
}
