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
    
    bool edgeForPoint(Vertex * a,
                          Vector * v, Vertex * b) {
        Vector ij = Vector(&data[I],&data[J]);
        Vector jk = Vector(&data[J],&data[K]);
        Vector ki = Vector(&data[K],&data[I]);
        
        bool found = false;
        
        if (ij.onEdge(a)) {
            *b = data[K];
            *v = ij;
            found = true;
        } else if (jk.onEdge(a)) {
            *b = data[I];
            *v = jk;
            found = true;
        } else if (ki.onEdge(a)) {
            *b = data[J];
            *v = ki;
            found = true;
        }
        return found;
    }
    
    Vertex * pointNotOnEdge(Vector *e)
    {
        Vertex *ap = getVertices();
        Vertex *ep = e->getVertices();
        for (int i = 0; i < 3; i++) {
            if (ap[i] != ep[0] && ap[i] != ep[1]) {
                return &ap[i];
            }
        }
        return nullptr;
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
    
    int numChildren() {
        return nChild;
    }
    
    int containsPoint(Vertex & p) {
        if (data[0] == p)
            return 0;
        else if (data[1] == p)
            return 1;
        else if (data[2] == p)
            return 2;
        else
            return -1;
    }
    
    //int containsPoint(Vertex * p) {
    //    Vertex & a = *p;
    //    return containsPoint(a);
    //}
    
    bool containsPoints(Vertex & a, Vertex & b, Vertex & c) {
        bool answer[3] = {false,false,false};
        int npt = -1;
        
        npt = containsPoint(a);
        if (npt >= 0) {
            if (answer[npt] == false)
                answer[npt] = true;
        } else {
            return false;
        }
        
        npt = containsPoint(b);
        if (npt >= 0) {
            if (answer[npt] == false)
                answer[npt] = true;
        } else {
            return false;
        }
        
        npt = containsPoint(c);
        if (npt >= 0) {
            if (answer[npt] == false)
                answer[npt] = true;
        } else {
            return false;
        }
        
        return (answer[0] == true && answer[1] == true && answer[2] == true) ? true : false;
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
