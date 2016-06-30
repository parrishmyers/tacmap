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
    Triangle *a,*b;
    Vertex i,j,k,l;
    DAG dag;

    virtual void SetUp() {
        i.set(1.8,4.0);
        j.set(2.0,1.0);
        k.set(3.5,3.0);
        l.set(4.0,1.0);
                
        a = dag.get();
        a->setVertices(&i, &j, &k);
        
        b = dag.get();
        b->setVertices(&k, &j, &l);
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
    
    Vertex a = Vertex(3.0,17.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&a) );
    
    Vertex b = Vertex(7.0,17.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&b) );
    
    Vertex c = Vertex(13.0,17.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&c) );
    
    Vertex d = Vertex(3.0,12.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&d) );
    
    Vertex e = Vertex(6.0,12.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&e) );
    
    Vertex f = Vertex(11.0,12.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&f) );
    
    Vertex g = Vertex(4.0,9.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&g) );
    
    Vertex h = Vertex(7.0,9.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&h) );
    
    Vertex i = Vertex(14.0,9.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&i) );
    
    Vertex j = Vertex(14.0,9.0);
    EXPECT_EQ( 0, onEdge(&n1,&n2,&j) );

    double tn = 0.5;
    double nx = 0.1 + n1.getX() + tn * (n2.getX() - n1.getX());
    double ny = n1.getY() + tn * (n2.getY() - n1.getY());
    
    Vertex k = Vertex(nx,ny);
    EXPECT_EQ( 1, onEdge(&n1,&n2,&k) );
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

TEST_F(DelaunayTriangulationTest, TestFind) {
    Vertex p(2.5,2.5);
    Triangle * fa = dag.findTriangleContainingPoint(&p);
    ASSERT_TRUE(nullptr != fa);
    ASSERT_TRUE(fa == a);
}

TEST_F(DelaunayTriangulationTest, TestDAGSplitOnInterior) {
    Vertex p(2.5,2.5);
    Triangle * aa = dag.findTriangleContainingPoint(&p);
    ASSERT_TRUE(nullptr != aa);
    
    int len_before = dag.len();
    
    dag.divide(aa, &p);
    
    int len_after = dag.len();
    
    ASSERT_EQ(len_after, len_before + 3);
    ASSERT_FALSE(aa->isValid());
    ASSERT_EQ(aa->numChildren(),3);
    
    Triangle * child_a = aa->getChild(0);
    ASSERT_TRUE(child_a->containsPoints(p,i,j) ||
                child_a->containsPoints(p,j,k) ||
                child_a->containsPoints(p,k,i) );
    
    Triangle * child_b = aa->getChild(1);
    ASSERT_TRUE(child_b->containsPoints(p,i,j) ||
                child_b->containsPoints(p,j,k) ||
                child_b->containsPoints(p,k,i) );

    Triangle * child_c = aa->getChild(2);
    ASSERT_TRUE(child_c->containsPoints(p,i,j) ||
                child_c->containsPoints(p,j,k) ||
                child_c->containsPoints(p,k,i) );
}

TEST_F(DelaunayTriangulationTest, TestDAGSplitOnEdge) {
    Vertex p = pointAlongLine2D(&j,&k,0.5);
    
    int len_before = dag.len();
    dag.divide(b, &p);
    int len_after = dag.len();
    
    EXPECT_EQ(len_after, len_before + 4);
    EXPECT_FALSE(b->isValid());
    EXPECT_FALSE(a->isValid());
    EXPECT_EQ(b->numChildren(), 2);
    EXPECT_EQ(a->numChildren(), 2);
    
    Triangle * child_a0 = a->getChild(0);
    ASSERT_TRUE(nullptr != child_a0);
    EXPECT_TRUE(child_a0->containsPoints(p,i,j) ||
                child_a0->containsPoints(p,k,i) );
    
    Triangle * child_a1 = a->getChild(1);
    ASSERT_TRUE(nullptr != child_a1);
    EXPECT_TRUE(child_a1->containsPoints(p,i,j) ||
                child_a1->containsPoints(p,k,i) );
    
    Triangle * child_b0 = b->getChild(0);
    ASSERT_TRUE(nullptr != child_b0);
    EXPECT_TRUE(child_b0->containsPoints(p,l,j) ||
                child_b0->containsPoints(p,k,l) );
    
    Triangle * child_b1 = b->getChild(1);
    ASSERT_TRUE(nullptr != child_b1);
    EXPECT_TRUE(child_b1->containsPoints(p,l,j) ||
                child_b1->containsPoints(p,k,l) );
}
