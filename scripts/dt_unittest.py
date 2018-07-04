import unittest

from dl_util import *

class TestDTMethods(unittest.TestCase):
    def test_feq(self):
        half_err = THRESHOLD / 2.0
        full_err = THRESHOLD
        a = 4.0
        b1 = a + half_err
        b2 = a - half_err
        b3 = a + full_err
        b4 = a - full_err
        b5 = a + full_err + half_err
        b6 = a - full_err - half_err

        self.assertTrue( fequal(a, b1) )
        self.assertTrue( fequal(a, b2) )
        self.assertTrue( fequal(a, b3) )
        self.assertFalse( fequal(a, b4) )
        self.assertFalse( fequal(a, b5) )
        self.assertFalse( fequal(a, b6) )
    
    def test_vertex(self):
        p1 = Vertex(1.0, 2.0)
        p2 = Vertex(1.0, 2.0)
        p3 = Vertex(2.0, 3.0)

        self.assertTrue(p1 == p2)
        self.assertFalse(p1 == p3)
    
    def test_area_of_triangle(self):
        a = Vertex( 1.0, 2.0)
        b = Vertex(10.0, 2.0)
        c = Vertex( 1.0, 5.0)
        area = area_of_triangle(a, b, c)
        self.assertTrue( fequal(area, 13.5) )

class TestAreaErrorAnalysis(unittest.TestCase):
    def setup(self):
        pass
    def tearDown(self):
        pass
    def test_dist(self):
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
        a = numpy.array(e)
        print ('m: {} v: {}'.format(
            a.mean(),
            a.std()
        ))

class TestDTTriAdj(unittest.TestCase):
    def setUp(self):
        self.p1 = Vertex(0.9, 3.3)
        self.p2 = Vertex(2.1, 5.0)
        self.p3 = Vertex(2.1, 1.0)
        self.p4 = Vertex(4.7, 3.3)
        self.p5 = Vertex(7.0, 5.0)
        self.p6 = Vertex(6.9, 1.0)

    def tearDown(self):
        pass
    
    def test_simple(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p2, self.p3, self.p4)
        t3 = Triangle(self.p4, self.p5, self.p6)
        self.assertTrue( t1 / t2)
        self.assertFalse( t1 / t3)
        self.assertFalse( t2 / t3)
    
    def test_01(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p2, self.p3, self.p4)
        self.assertTrue( t1 / t2)

    def test_02(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p2, self.p4, self.p3)
        self.assertTrue( t1 / t2)

    def test_03(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p2, self.p3, self.p4)
        self.assertTrue( t1 / t2)

    def test_04(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p2, self.p4, self.p3)
        self.assertTrue( t1 / t2)

    def test_05(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p3, self.p2, self.p4)
        self.assertTrue( t1 / t2)

    def test_06(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p4, self.p2, self.p3)
        self.assertTrue( t1 / t2)

    def test_07(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p3, self.p2, self.p4)
        self.assertTrue( t1 / t2)

    def test_08(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p4, self.p2, self.p3)
        self.assertTrue( t1 / t2)

    def test_09(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p3, self.p4, self.p2)
        self.assertTrue( t1 / t2)

    def test_10(self):
        t1 = Triangle(self.p2, self.p3, self.p1)
        t2 = Triangle(self.p4, self.p3, self.p2)
        self.assertTrue( t1 / t2)

    def test_11(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p3, self.p4, self.p2)
        self.assertTrue( t1 / t2)

    def test_12(self):
        t1 = Triangle(self.p2, self.p1, self.p3)
        t2 = Triangle(self.p4, self.p3, self.p2)
        self.assertTrue( t1 / t2)

    def test_13(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p2, self.p3, self.p4)
        self.assertTrue( t1 / t2)

    def test_14(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p2, self.p4, self.p3)
        self.assertTrue( t1 / t2)

    def test_15(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p3, self.p2, self.p4)
        self.assertTrue( t1 / t2)

    def test_16(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p4, self.p2, self.p3)
        self.assertTrue( t1 / t2)

    def test_17(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p3, self.p4, self.p2)
        self.assertTrue( t1 / t2)

    def test_18(self):
        t1 = Triangle(self.p1, self.p2, self.p3)
        t2 = Triangle(self.p4, self.p3, self.p2)
        self.assertTrue( t1 / t2)

if __name__ == '__main__':
    unittest.main()
