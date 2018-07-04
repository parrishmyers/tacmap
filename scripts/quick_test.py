import random
import numpy
from dl_util import *

def show(a, b, c):

    a1 = area_of_triangle(a, b, c)
    e1 = error_in_area(a, b, c)

    print('T:{} A:{} E:{} Z:{}'
    .format(
            THRESHOLD,
            a1,
            e1,
            area_equal(e1, 0.0)
           )
    )
    return e1

if __name__ == '__main__':
    e = []
    for i in range(0, 10000):
        x = [0.0, 0.0, 0.0]
        y = [0.0, 0.0, 0.0]
        for j in range(0,3):
            x[j] = random.uniform(-1000.0, 1000.0,)
            y[j] = random.uniform(-1000.0, 1000.0)
        a = Vertex(x[0], y[0])
        b = Vertex(x[1], y[1])
        c = Vertex(x[2], y[2])
        print ('************************')
        e.append(show(a, b, c))
    a = numpy.array(e)
    print ('m: {} v: {}'.format(
        a.mean(),
        a.std()
    ))