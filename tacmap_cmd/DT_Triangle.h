#pragma once

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
	~Triangle() {}
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

	Vertex & getVertexI() {
		return data[I];
	}

	Vertex & getVertexJ() {
		return data[J];
	}

	Vertex & getVertexK() {
		return data[K];
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

	friend bool isContained(Triangle &t, Vertex &p);
};
