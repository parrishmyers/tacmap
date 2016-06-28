//
//  Circle.h
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/10/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef Circle_h
#define Circle_h

#include <stdio.h>
#include "json.hpp"

using json = nlohmann::json;

class Circle {
public:
    enum CarC {
        X = 0,
        Y = 1,
        R = 2,
    };
private:
    double data[3];
public:
    Circle(double x = 0.0, double y = 0.0, double r = 0.0) {
        set(x,y,r);
    }
    
    void set(double x = 0.0, double y = 0.0, double r = 0.0) {
        data[X] = x;
        data[Y] = y;
        data[R] = r;
    }
    
    double getX() const { return data[X]; }
    double getY() const { return data[Y]; }
    double getR() const { return data[R]; }
    
    void printIt(FILE *fh) {
        fprintf(fh,"%f, %f, %f", data[X], data[Y], data[R]);
    }
    
    bool pointInside(double x, double y) {
        double diff_x = x - data[X];
        double diff_y = y - data[Y];
        double dist = std::sqrt( diff_x * diff_x + diff_y * diff_y );
        if (dist <= data[R])
            return true;
        else
            return false;
    }
    
    json to_json() {
        json j;
        j["circle"] = { data[X], data[Y], data[R] };
        return j;
    }
};

#endif /* Circle_h */
