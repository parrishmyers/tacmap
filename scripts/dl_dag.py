from dl_util import *

class DAG:
    def __init__(self, t1):
        pass
    
    def __match_b_in_a(self, a, b):
        av = [-1, -1, -1]
        bv = [-1, -1, -1]
        for ai in range(0,3):
            for bi in range(0,3):
                ap = a.points[ai]
                bp = b.points[bi]
                if ap == bp:
                    av[ai] = bi
                    bv[bi] = ai
        p1 = -1
        p2 = -1
        c = []
        for i in range(0,3):
            if av[i] == -1:
                p1 = i
            else:
                c.append(i)
            if bv[i] == -1:
                p2 = i
            
        return c, p1, p2
    
    def split(self, t1, p):
        pass
    
    def flip(self, t1, t2):
        c, p1, p2 = self.__match_b_in_a(a,b)
        
        t1 = Triangle(a.points[c[0]],a.points[p1],b.points[p2],name1)
        t2 = Triangle(a.points[c[1]],a.points[p1],b.points[p2],name2)
        
        a.children[0] = t1
        a.children[1] = t2
        a.children[2] = None
        a.valid = False
        
        b.children[0] = t1
        b.children[1] = t2
        b.children[2] = None
        b.valid = False

        DAG.append(t1)
        DAG.append(t2)

    
    def find_triangle_containing(p):
        pass