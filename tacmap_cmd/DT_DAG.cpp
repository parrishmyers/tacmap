#include <cmath>
#include "DT_DAG.h"



//
// DAG member functions
//
template<int Size>
Triangle * DAG<Size>::find(Vertex & p)
{
	for (int i = 0; i < tri.len(); i++) {
		Triangle * t = tri[i];
		if (t != nullptr && t->isValid() && isContained(*t, p)) {
			return t;
		}
	}
	return nullptr;
}

template<int Size>
void DAG<Size>::divide(Triangle & a)
{
}

template<int Size>
Triangle * DAG<Size>::get()
{
    Triangle *t = tri.get();
    if (t == nullptr)
        fprintf(stdout, "WARNING: DAG::get() pool is empty!\n");

    return t;
}

template<int Size>
void DAG<Size>::removeTriangleContainingPoint(Vertex & a)
{
    Triangle * t = find(a);
    while(t != nullptr) {
        t->setValid(false);
    }
}
