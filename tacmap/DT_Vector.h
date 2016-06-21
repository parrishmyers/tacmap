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
#include "DT_Constants.h"
#include "DT_Vertex.h"

#include "json.hpp"

using json = nlohmann::json;

class Vector {
private:
    Vertex data[2];
public:
    Vector() {
        
    }
    
    Vector(Vertex * a,Vertex * b) {
        set(a,b);
    }
    
    void set(Vertex * a,Vertex * b) {
        data[0].set(a);
        data[1].set(b);
    }
    
    Vertex * getVertices() {
        return data;
    }
    
    json to_json() {
        json j;
        char addr[66];
        snprintf(addr,66,"0x%0lx",(unsigned long)this);
        j["addr"] = addr;
        j["data"].push_back(data[0].to_json());
        j["data"].push_back(data[1].to_json());
        return j;
    }
    
    bool onEdge(Vertex * pr) {
        double minx = (data[0].getX() <= data[1].getX())? data[0].getX() : data[1].getX();
        double maxx = (data[0].getX() >= data[1].getX())? data[0].getX() : data[1].getX();
        
        double miny = (data[0].getY() <= data[1].getY())? data[0].getY() : data[1].getY();
        double maxy = (data[0].getY() >= data[1].getY())? data[0].getY() : data[1].getY();
        
        if (pr->getX() < minx)
            return false;
        else if (pr->getX() > maxx)
            return false;
        else if (pr->getY() < miny)
            return false;
        else if (pr->getY() > maxy)
            return false;
        else
            return distance2Point(pr) < Constants::DIST_THRESH;
    }
    
    double distance2Point(Vertex * pr) {
        double nx = data[1].getX() - data[0].getX();
        double ny = data[1].getY() - data[0].getY();
        
        double nmag = sqrt(nx * nx + ny * ny);
        
        double px = pr->getX() - data[0].getX();
        double py = pr->getY() - data[0].getY();
        
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
