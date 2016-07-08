import functools
import math
import numpy as np
import matplotlib.pyplot as plt
import sys

class Vertices:
    def __init__(self):
        self.M = -sys.float_info.max
        self.order = []
        self.next_ = 0
        self.point_ = [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0]]

    def add_point(self, x, y):
        self.point_.append([x, y])
        if self.M < x:
            self.M = x
        if self.M < y:
            self.M = y
        self.point_[0] = [3.0 * self.M, 0.0]
        self.point_[1] = [0.0, 3.0 * self.M]
        self.point_[2] = [-3.0 * self.M, -3.0 * self.M]

    def get_point(self, i):
        x = 0.0
        y = 0.0
        try:
            x = self.point_[i+3][0]
            y = self.point_[i+3][1]
        except:
            print '{} out of range 0..{}'.format(i,len(self.point_))
        return x, y

    def permute(self):
        self.order = np.random.permutation(len(self.point_)-3)

    def x(self, offset=0):
        n = []
        i = 0
        for p in self.point_:
            if i >= offset:
                n.append(p[0])
            i += 1
        return np.array(n)

    def y(self, offset=0):
        n = []
        i = 0
        for p in self.point_:
            if i >= offset:
                n.append(p[1])
            i += 1
        return np.array(n)

    def __len__(self):
        return len(self.point_)


class Triangle:
    def __init__(self, a=None, b=None, c=None, name=''):
        self.points = [a, b, c]
        self.children = [None, None, None]
        self.valid = True
        self.name = name

    def __str__(self):
        str1 = '{} = '.format(self.name)
        str1 += '{}, {}, {} :'.format(self.points[0], self.points[1], self.points[2])
        str1 += ' {}, {}, {}'.format(self.children[0], self.children[1], self.children[2])
        return str1

    def xy_pts(self):
        return np.array([Points.get_point(self.points[0]),
                         Points.get_point(self.points[1]),
                         Points.get_point(self.points[2]),
                         Points.get_point(self.points[0])])

    def center_loc(self):
        center_x = 0
        center_y = 0
        for p in self.points:
            x,y = Points.get_point(p)
            center_x += x
            center_y += y
        center_x /= 3
        center_y /= 3
        return center_x, center_y


def circle_for_points(a, b, c):
    x1, y1 = Points.get_point(a)
    x2, y2 = Points.get_point(b)
    x3, y3 = Points.get_point(c)

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
    pr_x, pr_y = Points.get_point(pr)
    x_diff = pr_x - x
    y_diff = pr_y - y
    dist = np.sqrt(x_diff**2 + y_diff**2)
    if dist <= r:
        return True
    else:
        return False


def plot_triangles(DAG):
    for t in DAG:
        if t.valid:
            xy = t.xy_pts()
            plt.plot(xy[:, 0], xy[:, 1], '-')
            x, y = t.center_loc()
            plt.text(x, y, t.name)


def plot_circle(t):
    c_x,c_y,c_r = circle_for_points(t.points[0],
                                    t.points[1],
                                    t.points[2])
    c1 = plt.Circle((c_x,c_y),c_r,color='black',fill=False)
    plt.gca().add_artist(c1)
    
        
def plot_scene(DAG,pts,a,offset=0):
    x = Points.x(offset)
    y = Points.y(offset)
    plt.plot(x, y, 'g.')
    if a is not None:
        x, y = Points.get_point(a)
        plt.plot(x, y, 'ro')
    plot_triangles(DAG)
    plt.show()


def match_b_in_a(a,b):
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


def is_adjacent(a, pr, b):
    c = 0
    pra = -1
    prb = -1
    for i in xrange(0,3):
        if a.points[i] == pr:
            pra = i
            continue
        for j in xrange(0,3):
            if b.points[j] == pr:
                prb = j
                break
            elif b.points[j] == a.points[i]:
                c += 1
    if prb < 0 and c == 2 and pra >= 0:
        return True
    else:
        return False


def divide_on_edge(a, pr, DAG, log):
    pass

def divide_on_interior(a, pr, DAG, log):
    next_num = len(DAG)    
    name1 = '{}'.format(next_num)
    name2 = '{}'.format(next_num+1)
    name3 = '{}'.format(next_num+2)
    msg = 'divide,interior: {} -> {}, {}, {}'.format(
        a.name,
        name1,
        name2,
        name3)
    print msg
    log.write(msg)
    log.write('\n')

    t1 = Triangle(pr, a.points[0], a.points[1], name1)
    t2 = Triangle(pr, a.points[1], a.points[2], name2)
    t3 = Triangle(pr, a.points[2], a.points[0], name3)
    # form connections
    a.children[0] = t1
    a.children[1] = t2
    a.children[2] = t3
    a.valid = False
    # add to DAG
    DAG.append(t1)
    DAG.append(t2)
    DAG.append(t3)
    
    return t1, t2, t3

def flip(a,b,DAG,log):
    next_num = len(DAG)    
    name1 = '{}'.format(next_num)
    name2 = '{}'.format(next_num+1)
    msg = 'flip: {}, {} -> {}, {}'.format(a.name,b.name,name1,name2)
    print msg
    log.write(msg)
    log.write('\n')
    
    c, p1, p2 = match_b_in_a(a,b)
    
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
    return t1, t2


def is_contained(p, t):
    x, y = Points.get_point(p)

    x1, y1 = Points.get_point(t.points[0])
    x2, y2 = Points.get_point(t.points[1])
    x3, y3 = Points.get_point(t.points[2])

    denom = (x1 * (y2 - y3) + y1 * (x3 - x2) + x2 * y3 - y2 * x3)
    t1 = (x * (y3 - y1) + y * (x1 - x3) - x1 * y3 + y1 * x3) / denom
    t2 = (x * (y2 - y1) + y * (x1 - x2) - x1 * y2 + y1 * x2) / -denom

    if 0.0 <= t1 <= 1.0 and 0.0 <= t2 <= 1.0 and t1 + t2 <= 1.0:
        return True
    else:
        return False


def is_adjacent(a, pr, b):
    c = 0
    pra = -1
    prb = -1
    for i in xrange(0,3):
        if a.points[i] == pr:
            pra = i
            continue
        for j in xrange(0,3):
            if b.points[j] == pr:
                prb = j
                break
            elif b.points[j] == a.points[i]:
                c += 1
    if prb < 0 and c == 2 and pra >= 0:
        return True
    else:
        return False


def depth_first_search(a, DAG, test):
    if a == None:
        return None
    elif a.valid:
        if test(a):
            return a
        else:
            return None
    else:
        ret = depth_first_search(a.children[0], DAG, test)
        if ret is not None:
            return ret
        ret = depth_first_search(a.children[1], DAG, test)
        if ret is not None:
            return ret
        ret = depth_first_search(a.children[2], DAG, test)
        if ret is not None:
            return ret


def find_adjacent(t, pr, DAG):
    root = DAG[0]
    return depth_first_search(root, DAG, functools.partial(is_adjacent, t, pr))


def find_contained(pr, DAG):
    root = DAG[0]
    return depth_first_search(root, DAG, functools.partial(is_contained, pr))


def valid_edge(t, pr, DAG, log):
    plot_scene(DAG,Points,pr)
    t_adj = find_adjacent(t, pr, DAG)
    if t_adj is not None:
        plot_circle(t_adj)
    if t_adj is not None and in_circle(t_adj, pr):
        t1, t2 = flip(t, t_adj, DAG, log)
        valid_edge(t1, pr, DAG, log)
        valid_edge(t2, pr, DAG, log)

# #########################################################
# Initialize
# #########################################################

Points = Vertices()
Points.add_point(-0.02222276248244826, -0.4979727817680433)
Points.add_point(-0.4285431913366012,   0.4745826469497594)
Points.add_point( 0.3105396575392593,   0.2400179190933871)
Points.add_point(-0.01883958887200765,  0.3630260628303755)
Points.add_point( 0.3790312361708201,   0.3779794437605696)
Points.add_point(-0.2994955874043476,   0.3776609263174803)
Points.add_point( 0.3471817493878135,   0.08365533089605659)
Points.add_point(-0.00485819764887746,  0.3482682405489201)
Points.add_point( 0.3443122672329771,  -0.1437312230875075)
Points.add_point( 0.309330780347186,   -0.07758103877080702)
Points.order = [2, 9, 7, 8, 4, 1, 3, 0, 6, 5]

DAG = []
log = open('log.txt','w')

t0 = Triangle(-1, -2, -3, '0')
DAG.append(t0)

for pr in Points.order:
    # find triangle that contains pr
    t = find_contained(pr, DAG)
    t1, t2, t3 = divide_on_interior(t, pr, DAG, log)
    valid_edge(t1, pr, DAG, log)
    valid_edge(t2, pr, DAG, log)
    valid_edge(t3, pr, DAG, log)

print 'Done'
for t in DAG:
    if t.points[0] < 0 or t.points[1] < 0 or t.points[2] < 0:
        t.valid = False

plot_scene(DAG,Points,None,3)

log.close()
