//
//  DT_Utils.hpp
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/10/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef DT_Utils_hpp
#define DT_Utils_hpp

#include <stdio.h>

#include "DT_Vertex.h"
#include "DT_Triangle.h"
#include "DT_Circle.h"

//
// Friend functions of Triangle & Vertex
//
bool isContained(Triangle * t, Vertex * p);

//
// Circle tests
//
bool inCircle(Triangle *a, Vertex * pr);
bool inCircle(Vertex * a, Vertex * b, Vertex * c, Vertex * d);
Circle circleForPoints(Vertex * a, Vertex * b, Vertex * c);
Vertex pointAlongLine2D(Vertex * a, Vertex * b, double tn);

#endif /* DT_Utils_hpp */
