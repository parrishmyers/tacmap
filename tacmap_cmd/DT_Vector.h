//
//  DT_Vector.h
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/15/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#pragma once

#ifndef DT_Vector_h
#define DT_Vector_h

#include <cmath>
#include <string>
#include "DT_Vertex.h"

class Vector {
public:
    const double THRESH;
private:
    Vertex data[2];
public:
    Vector(Vertex const& a,Vertex const& b, double thresh = 0.1) : THRESH(thresh) {
        data[0] = a;
        data[1] = b;
    }
    
    Vertex & getVertex1() {
        return data[0];
    }
    
    Vertex & getVertex2() {
        return data[1];
    }
    
    std::string str() {
        std::string s = "Vector[";
        s += data[0].str() + " => ";
        s += data[1].str() + "]";
        return s;
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
        double nx = data[1].getX() - data[0].getX();
        double ny = data[1].getY() - data[0].getY();
        
        double nmag = sqrt(nx * nx + ny * ny);
        
        double px = pr.getX() - data[0].getX();
        double py = pr.getY() - data[0].getY();
        
        //double pmag = sqrt(px * px + py * py);
        
        double dnp = nx * px + ny * py; // dot(n,p)
        
        double dnpx = nx * dnp / (nmag * nmag);
        double dnpy = ny * dnp / (nmag * nmag);
        
        double perx = dnpx - px;
        double pery = dnpy - py;
        
        return sqrt(perx * perx + pery * pery);
    }
};


#endif /* DT_Vector_h */
