//
//  DT_Vector.h
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/15/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef DT_Vector_h
#define DT_Vector_h

#include <cmath>
#include "DT_Vertex.h"

class Vector {
public:
    const double THRESH = 0.001;
private:
    Vertex data[2];
public:
    Vector(Vertex const& a,Vertex const& b) {
        data[0] = a;
        data[1] = b;
    }
    
    bool onEdge(Vertex const& pr) {
        double minx = (data[0].getX() <= data[1].getX())? data[0].getX() : data[1].getX();
        double maxx = (data[0].getX() >= data[1].getX())? data[0].getX() : data[1].getX();
        
        double miny = (data[0].getY() <= data[1].getY())? data[0].getY() : data[1].getY();
        double maxy = (data[0].getY() >= data[1].getY())? data[0].getY() : data[1].getY();
        
        if (pr.getX() < minx)
            return false;
        else if (pr.getX() > maxx)
            return false;
        else if (pr.getY() < miny)
            return false;
        else if (pr.getY() > maxy)
            return false;
        else
            return distance2Point(pr) < THRESH;
    }
    
    double distance2Point(Vertex const& pr) {
        double ax = data[0].getX();
        double ay = data[0].getY();
        double px = pr.getX();
        double py = pr.getY();
        double nx = data[1].getX() - ax;
        double ny = data[1].getY() - ay;
        
        double apx = ax - px;
        double apy = ay - py;
        
        double dapn = apx * nx + apy * ny;
        
        double norm_n = sqrt(nx * nx + ny * ny);
        double nxhat = nx / norm_n;
        double nyhat = ny / norm_n;
        
        double ppx = apx - dapn * nxhat;
        double ppy = apy - dapn * nyhat;
        
        return sqrt( ppx * ppx + ppy * ppy);
    }
};


#endif /* DT_Vector_h */
