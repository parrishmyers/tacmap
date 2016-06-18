#pragma once

#include <assert.h>
#include <string>

#include "DT_Vertex.h"
#include "DT_Vector.h"

extern FILE * DebugLog;

class Triangle {
private:
	int pts;
	Vertex * data[3];

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
        data[0] = nullptr;
        data[1] = nullptr;
        data[2] = nullptr;
		child[0] = nullptr;
		child[1] = nullptr;
		child[2] = nullptr;
	}
	Triangle(Vertex * i, Vertex * j, Vertex * k) {
        setVertices(i, j, k);
	}
	
    ~Triangle() {}
    
    void setVertices(Vertex * i, Vertex * j, Vertex * k) {
        assert( nullptr != i && nullptr != j && nullptr != k );
        data[0] = i;
        data[1] = j;
        data[2] = k;
        pts = 3;
        valid = true;
        child[0] = nullptr;
        child[1] = nullptr;
        child[2] = nullptr;
        nChild = 0;
    }

    Vertex ** getVertices() {
        return data;
    }
    
    void getVerticesNotContainingPoint(Vertex *p, Vertex *e[2])
    {
        Vertex **ap = getVertices();
        e[0] = nullptr;
        e[1] = nullptr;

        for (int i = 0, j = 0; i < 3; i++) {
            if (ap[i] != p) {
                assert(j < 2);
                e[j] = ap[i];
                j += 1;
            }
        }
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
        if (*data[0] == p)
            return 0;
        else if (*data[1] == p)
            return 1;
        else if (*data[2] == p)
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
    
    bool onEdge(Vertex * a) {
        Vector ij = Vector(data[0],data[1]);
        Vector jk = Vector(data[1],data[2]);
        Vector ki = Vector(data[2],data[0]);
        
        bool found = false;
        
        if (ij.onEdge(a)) {
            found = true;
        } else if (jk.onEdge(a)) {
            found = true;
        } else if (ki.onEdge(a)) {
            found = true;
        }
        return found;
    }
    
    void print() {
        fprintf(DebugLog, "{'addr':'0x%0lx', 'valid': %d, ",
                (unsigned long)this,
                isValid());
        
        fprintf(DebugLog,"'triangle': [");
        for (int d = 0; d < 3; d++) {
            if (nullptr != data[d])
                fprintf(DebugLog, "%s", data[d]->str().c_str());
            else
                fprintf(DebugLog, "None");
            
            if (d < 2)
                fprintf(DebugLog,", ");
        }
        fprintf(DebugLog,"], ");
        
        fprintf(DebugLog, "'children': [");
        for (int j = 0; j < 3; j++) {
            Triangle * c = getChild(j);
            if (nullptr == c)
                fprintf(DebugLog, "None");
            else
                fprintf(DebugLog, "'0x%0lx'", (unsigned long)c);
            
            if (j < 2)
                fprintf(DebugLog,", ");
        }
        fprintf(DebugLog, "]}");
    }
    
    friend bool isContained(Triangle * t, Vertex * p);
};
