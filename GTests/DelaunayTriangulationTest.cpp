//
//  PoolTest.cpp
//  tacmap
//
//  Created by Parrish Myers on 5/28/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#include <cstdlib>
#include <stdio.h>
#include "gtest/gtest.h"

#include "DelaunayTriangulation.h"
#include "DT_Pool.h"
#include "DT_Triangle.h"
#include "DT_Utils.h"
#include "DT_Vertex.h"

class DelaunayTriangulationTest : public ::testing::Test {
protected:
    Triangle a,b;
    Vertex i,j,k,l;

    virtual void SetUp() {
        i.set(1.8,4.0);
        j.set(2.0,1.0);
        k.set(3.5,3.0);
        l.set(4.0,1.0);
        a.setVertices(&i, &j, &k);
        b.setVertices(&k, &j, &l);
        
    }
    
    virtual void TearDown() {
    }

};

TEST_F(DelaunayTriangulationTest, TestVertex) {
    Vertex a = Vertex(1.0,1.0);
    Vertex b = Vertex(1.1, 1.1);
    Vertex c = Vertex(1.0001,1.0001);
    ASSERT_FALSE(a == b);
    ASSERT_TRUE(a == c);
}

TEST_F(DelaunayTriangulationTest, TestContainment) {
    Vertex a(4.0, 9.0);
    Vertex b(7.0, 5.0);
    Vertex c(10.0, 16.0);
    Vertex p1(7.0, 9.0); // inside
    Vertex p2(5.0, 13.0); // outside
    
    double x = 0.5 * c.getX() + (1.0 - 0.5) * a.getX();
    double y = 0.5 * c.getY() + (1.0 - 0.5) * a.getY();
    Vertex p3(x,y); // along edge a,c
    
    Triangle t(&a, &b, &c);
    
    EXPECT_TRUE ( isContained(&t, &p1) ); // inside
    EXPECT_FALSE( isContained(&t, &p2) ); // outside
    EXPECT_TRUE ( isContained(&t, &p3) ); // along a,c
    EXPECT_TRUE ( isContained(&t, &a)  ); // coincident with a
    EXPECT_TRUE ( isContained(&t, &b)  ); // coincident with b
    EXPECT_TRUE ( isContained(&t, &c)  ); // coincident with c
}

TEST_F(DelaunayTriangulationTest, TestInCircle) {
    Vertex * i = new Vertex(0.0,0.0);
    Vertex * j = new Vertex(1.0,0.0);
    Vertex * k = new Vertex(0.0,1.0);
    
    Vertex * a = new Vertex(0.25,0.25);
    Vertex * c = new Vertex(1.0,1.0);

    EXPECT_TRUE(inCircle(i, j, k, a));
    EXPECT_FALSE(inCircle(i, j, k, c));
    
    delete i;
    delete j;
    delete k;
    delete a;
    delete c;
}

TEST_F(DelaunayTriangulationTest, TestOnEdge) {
    Vertex n1(5.0,10.0);
    Vertex n2(10.0,16.0);
    
    Vector n(&n1,&n2);
    
    Vertex a = Vertex(3.0,17.0);
    EXPECT_EQ( 0, n.onEdge(&a) );
    
    Vertex b = Vertex(7.0,17.0);
    EXPECT_EQ( 0, n.onEdge(&b) );
    
    Vertex c = Vertex(13.0,17.0);
    EXPECT_EQ( 0, n.onEdge(&c) );
    
    Vertex d = Vertex(3.0,12.0);
    EXPECT_EQ( 0, n.onEdge(&d) );
    
    Vertex e = Vertex(6.0,12.0);
    EXPECT_EQ( 0, n.onEdge(&e) );
    
    Vertex f = Vertex(11.0,12.0);
    EXPECT_EQ( 0, n.onEdge(&f) );
    
    Vertex g = Vertex(4.0,9.0);
    EXPECT_EQ( 0, n.onEdge(&g) );
    
    Vertex h = Vertex(7.0,9.0);
    EXPECT_EQ( 0, n.onEdge(&h) );
    
    Vertex i = Vertex(14.0,9.0);
    EXPECT_EQ( 0, n.onEdge(&i) );
    
    Vertex j = Vertex(14.0,9.0);
    EXPECT_EQ( 0, n.onEdge(&j) );

    double tn = 0.5;
    double nx = 0.1 + n1.getX() + tn * (n2.getX() - n1.getX());
    double ny = n1.getY() + tn * (n2.getY() - n1.getY());
    
    Vertex k = Vertex(nx,ny);
    EXPECT_EQ( 1, n.onEdge(&k) );
}

TEST_F(DelaunayTriangulationTest, TestPool) {
    Pool<Vertex,1000> p;
    
    for (int i = 0; i < 1000; i++) {
        Vertex * a = p.get();
        ASSERT_NE(nullptr, a);
    }
    Vertex * b = p.get();
    ASSERT_EQ(nullptr, b);
    
}

TEST_F(DelaunayTriangulationTest, TestDAGSplitOnInterior) {
    Vertex p(2.5,2.5);
}

TEST_F(DelaunayTriangulationTest, TestDAGSplitOnEdge) {
    Vertex p = pointAlongLine2D(&j,&k,0.5);
}

TEST_F(DelaunayTriangulationTest, TestAlgorithm) {
    DelaunayTriangulation dt;
    
    std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator
    for (int i = 0; i < 1000; i++) {
        double x = (double)std::rand();
        double y = (double)std::rand();
        
        dt.addPt(x, y, 0.0);
    }
    
    dt.compute();

    ASSERT_TRUE(false);
}
