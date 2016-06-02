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
	Triangle(Vertex * i, Vertex * j, Vertex * k) {
        setVertices(i, j, k);
	}
	
    ~Triangle() {}
    
    void setVertices(Vertex * i, Vertex * j, Vertex * k) {
        data[I].set(i);
        data[J].set(j);
        data[K].set(k);
        pts = 3;
        valid = true;
        child[0] = nullptr;
        child[1] = nullptr;
        child[2] = nullptr;
        nChild = 0;
    }

    Vertex * getVertices() {
        return data;
    }
    
    Vector  getEdgeIJ() {
        return Vector(&data[I],&data[J]);
    }

    Vector  getEdgeJK() {
        return Vector(&data[J],&data[K]);
    }
    
    Vector  getEdgeKI() {
        return Vector(&data[K],&data[I]);
    }
    
    bool isValid() const {
        return (pts >= 3 && valid == true)? true : false;
	}

	void setValid(bool a) {
        valid = (pts >= 3)? a : false;
	}

	void addChild(Triangle * t) {
		if (nChild < 3) {
			child[nChild] = t;
			nChild += 1;
		}
	}

	Triangle * getChild(int idx) {
        return (idx >= 0 && idx < nChild)? child[idx] : nullptr;
	}
    
    bool containsPoint(Vertex &p) {
        return (p == data[0] || p == data[1] || p == data[2])? true : false;
    }

    bool containsEdge(Vector * v) {
        Vertex * p = v->getVertices();
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == j) continue;
                if (data[i] == p[0] && data[j] == p[1])
                    return true;
                else if (data[i] == p[1] && data[j] == p[0])
                    return true;
            }
        }
        return false;
    }
    
    friend bool isContained(Triangle * t, Vertex * p);
};
