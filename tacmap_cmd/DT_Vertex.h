#pragma once

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
    double getX() const { return data[X]; };
	double getY() const { return data[Y]; };
    double getZ() const { return data[Z]; };
    
	void set(double x, double y, double z = 0.0) {
		data[X] = x;
		data[Y] = y;
	}
};

