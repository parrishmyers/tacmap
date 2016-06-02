#pragma once

#include <string>
#include "DT_Constants.h"


class Triangle;

class Vertex {
public:
	enum CarC {
		X = 0,
		Y = 1,
        Z = 2,
	};
private:
	double data[3];
public:
    Vertex(double x = 0.0, double y = 0.0, double z = 0.0) {
        data[X] = x;
        data[Y] = y;
        data[Z] = z;
    }
    
    std::string str() {
        char tmp[1000];
        sprintf(tmp, "Vertex[%9.3lf,%9.3lf,%9.3lf]",
                data[X],data[Y],data[Z]);
        return std::string(tmp);
    }
    
    double getX() { return data[X]; };
	double getY() { return data[Y]; };
    double getZ() { return data[Z]; };
    
	void set(double x, double y, double z = 0.0) {
		data[X] = x;
		data[Y] = y;
	}
    
    void set(Vertex * pr) {
        data[X] = pr->getX();
        data[Y] = pr->getY();
    }
    
    bool operator==(Vertex & b) {
        double bx = b.getX();
        double by = b.getY();
        double bz = b.getZ();
        
        double ax = data[X];
        double ay = data[Y];
        double az = data[Z];
        
        bool equal = true;
        if (bx > ax + Constants::VECT_THRESH || bx < ax - Constants::VECT_THRESH) equal = false;
        if (by > ay + Constants::VECT_THRESH || by < ay - Constants::VECT_THRESH) equal = false;
        if (bz > az + Constants::VECT_THRESH || bz < az - Constants::VECT_THRESH) equal = false;
        return equal;
    }
};

