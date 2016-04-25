#pragma once

#include "DT_Vertex.h"
#include "DT_Vector.h"

class Triangle {
public:
	enum Corner {
		I = 0,
		J = 1,
		K = 2,
	};
private:
	int pts;
	Vertex data[3];

	int nChild;
	Triangle * child[3];

	bool valid;

	void incPts() {
		pts += 1;
		if (pts >= 3) {
			valid = true;
		}
	}

public:
	Triangle() : valid(false), pts(0), nChild(0) {
		child[0] = nullptr;
		child[1] = nullptr;
		child[2] = nullptr;
	}
	Triangle(Vertex &i, Vertex &j, Vertex &k) {
        setVertices(i, j, k);
	}
	~Triangle() {}
    /*
	void setVertexI(Vertex& a) {
		data[I] = a;
		incPts();
	}

	void setVertexJ(Vertex& a) {
		data[J] = a;
		incPts();
	}
	void setVertexK(Vertex& a) {
		data[K] = a;
		incPts();
	}
    */
    void setVertices(Vertex &i, Vertex &j, Vertex &k) {
        data[I] = i;
        data[J] = j;
        data[K] = k;
        pts = 3;
        valid = true;
        child[0] = nullptr;
        child[1] = nullptr;
        child[2] = nullptr;
        nChild = 0;
    }

	Vertex & getVertexI() {
		return data[I];
	}

	Vertex & getVertexJ() {
		return data[J];
	}

	Vertex & getVertexK() {
		return data[K];
	}
    
    Vector  getEdgeIJ() {
        return Vector(data[I],data[J]);
    }

    Vector  getEdgeJK() {
        return Vector(data[J],data[K]);
    }
    
    Vector  getEdgeKI() {
        return Vector(data[K],data[I]);
    }
    
    bool isValid() const {
		if (pts >= 3 && valid == true)
			return true;
		else
			return false;
	}

	void setValid(bool a) {
		if (pts >= 3) {
			valid = a;
		}
		else {
			valid = false;
		}
	}

	void addChild(Triangle * t) {
		if (nChild < 3) {
			child[nChild] = t;
			nChild += 1;
		}
	}

	Triangle * getChild(int idx) {
		if (idx >= 0 && idx < nChild) {
			return child[idx];
		}
		else {
			return nullptr;
		}
	}
    
    bool containsPoint(Vertex &p) {
        Vertex i = getVertexI();
        Vertex j = getVertexJ();
        Vertex k = getVertexK();
        if (p == i || p == j || p == k)
            return true;
        else
            return false;
    }

	friend bool isContained(Triangle &t, Vertex &p);
};
