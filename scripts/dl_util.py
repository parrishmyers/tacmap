import math
# %% 
# Util Functions on basic types
#
THRESHOLD = 0.00001
AREA_THRESHOLD = 0.01
def fequal(x1,x2, threshold=THRESHOLD):
    d = x1 - x2
    if d < 0.0:
        d *= -1.0
    if threshold >= d:
        return True
    else:
        return False

def area_equal(a1, a2, threshold=AREA_THRESHOLD):
    return fequal(a1, a2, threshold)

class Vertex:
    x = 0.0
    y = 0.0

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, b):
        result = True
        if not fequal(self.x, b.x):
            result = False
        if not fequal(self.y, b.y):
            result = False
        return result

def area_of_triangle(a,b,c):
    a = ( a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) )/ 2.0
    if a < 0.0:
        return -a
    else:
        return a

def error_in_area(a, b, c):
    area = area_of_triangle(a, b, c)

    a1 = Vertex(a.x+THRESHOLD, a.y)
    b1 = Vertex(b.x, b.y)
    c1 = Vertex(c.x, c.y)

    dAx = area_of_triangle(a1, b1, c1) - area

    a2 = Vertex(a.x, a.y)
    b2 = Vertex(b.x+THRESHOLD, b.y)
    c2 = Vertex(c.x, c.y)

    dBx = area_of_triangle(a2, b2, c2) - area

    a3 = Vertex(a.x, a.y)
    b3 = Vertex(b.x, b.y)
    c3 = Vertex(c.x+THRESHOLD, c.y)

    dCx = area_of_triangle(a3, b3, c3) - area

    a4 = Vertex(a.x, a.y+THRESHOLD)
    b4 = Vertex(b.x, b.y)
    c4 = Vertex(c.x, c.y)

    dAy = area_of_triangle(a4, b4, c4) - area

    a5 = Vertex(a.x, a.y)
    b5 = Vertex(b.x, b.y+THRESHOLD)
    c5 = Vertex(c.x, c.y)

    dBy = area_of_triangle(a5, b5, c5) - area

    a6 = Vertex(a.x, a.y)
    b6 = Vertex(b.x, b.y)
    c6 = Vertex(c.x, c.y+THRESHOLD)

    dCy = area_of_triangle(a6, b6, c6) - area

    da = math.sqrt( dAx ** 2 
                  + dBx ** 2
                  + dCx ** 2
                  + dAy ** 2
                  + dBy ** 2
                  + dCy ** 2
                  )
    return da

def are_triangles_adjacent(t1,t2):
    result = False
    
    if   t1.a == t2.a and t1.b == t2.b:
        result = True
    elif t1.a == t2.a and t1.b == t2.c:
        result = True 
    elif t1.a == t2.a and t1.c == t2.b:
        result = True 
    elif t1.a == t2.a and t1.c == t2.c:
        result = True 
    
    elif t1.a == t2.b and t1.b == t2.a:
        result = True 
    elif t1.a == t2.b and t1.b == t2.c:
        result = True 
    elif t1.a == t2.b and t1.c == t2.a:
        result = True 
    elif t1.a == t2.b and t1.c == t2.c:
        result = True 
    
    elif t1.a == t2.c and t1.b == t2.a:
        result = True 
    elif t1.a == t2.c and t1.b == t2.b:
        result = True 
    elif t1.a == t2.c and t1.c == t2.a:
        result = True 
    elif t1.a == t2.c and t1.c == t2.b:
        result = True 
    
    elif t1.b == t2.a and t1.c == t2.b:
        result = True 
    elif t1.b == t2.a and t1.c == t2.c:
        result = True 

    elif t1.b == t2.b and t1.c == t2.a:
        result = True 
    elif t1.b == t2.b and t1.c == t2.c:
        result = True

    elif t1.b == t2.c and t1.c == t2.a:
        result = True 
    elif t1.b == t2.c and t1.c == t2.b:
        result = True
    
    else:
        result = False

    return result

def circle_for_points(a, b, c):
    x1 = a.x
    y1 = a.y
    x2 = b.x
    y2 = b.y
    x3 = c.x
    y3 = c.y

    X2 = (x2-x1)
    X3 = (x3-x1)
    Y2 = (y2-y1)
    Y3 = (y3-y1)
    
    alpha = X3 / X2
    
    bx2 = (x2+x1) * X2
    bx3 = (x3+x1) * X3
    by2 = (y2+y1) * Y2
    by3 = (y3+y1) * Y3
    
    h = 0.0
    k = 0.0
    r = 0.0
    
    k = bx3 + by3 - alpha * (bx2 + by2)
    k /= 2 * (Y3 - alpha * Y2)
    
    h = bx2 + by2 - 2 * k * Y2
    h /= 2 * X2
    
    r = math.sqrt( (x1 - h)*(x1 - h) + (y1 - k)*(y1 - k) )
    
    return h, k, r


def in_circle(t, pr):
    x, y, r = circle_for_points(t.points[0], t.points[1], t.points[2])
    x_diff = pr.x - x
    y_diff = pr.y - y
    dist = math.sqrt(x_diff**2 + y_diff**2)
    if dist <= r:
        return True
    else:
        return False

# %%
# Basic classes
#

class DagNode:
    parrent = [None, None]
    children = [None, None, None]

    def split(self):
        pass
    

class Triangle(DagNode):
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

    def area(self):
        a = area_of_triangle(self.a, self.b, self.c)
        if area_equal(a, 0.0):
            return 0.0
        else:
            return a
    
    def area_zero(self):
        if area_equal(a, 0.0):
            return True
        else:
            return False
    

    def inside(self, p):
        a = area_of_triangle(self.a, self.b, self.c)
        a1  = area_of_triangle(self.a, self.b, p)
        a1 += area_of_triangle(self.b, self.c, p)
        a1 += area_of_triangle(self.c, self.a, p)
        return fequal(a, a1)
    
    def are_adjacent(self, a, b):
        return are_triangles_adjacent(a, b)

    # The '/' symbol is used to determine if 2 triangles
    # are adjacent
    def __truediv__(self, b):
        return are_triangles_adjacent(self, b)
