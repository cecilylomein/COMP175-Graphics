#include "Cube.h"
#include "Algebra.h"
#include "Shape.h"
#include <GL/glui.h>
#include <iostream>

/*Cube::Cube() {}
Cube::~Cube() {}*/

void draw() {
	Shape shape;
	int m_segmentsX = shape.getxseg();
	int m_segmentsY = shape.getyseg();
 	std::vector<Point>vertexList = shape.pointVector;
 	std::vector<Point> face[6];
	
	std::cout << "here" << std::endl;

	double dx = 1/m_segmentsX;
	double dy = 1/m_segmentsY;
	double dz = 1/m_segmentsX;
	//double dz = 1/m_segmentsX;
	double x  = dx;
	double y  = dy;
	double z  = dz;
	int index = 0;
	for (int i = 0; i <= m_segmentsX; i++) {
		for (int j = 0; j <= m_segmentsY; j++) {
			face[0][index] = Point(x, y, -0.5);
			face[1][index] = Point(x, y,  0.5);
			face[2][index] = Point(x, -0.5, y);
			face[3][index] = Point(x,  0.5, y);
			face[4][index] = Point(-0.5, y, x);
			face[5][index] = Point( 0.5, y, x);
			index ++;
			y += dy;
		}
		x += dx;
	}
	glBegin(GL_TRIANGLES);
	double a, b, c, d;
	a = 0;
	b = a + 1;
	c = a + m_segmentsY + 1;
	d = c + 1;
	for (int i = 0; i < m_segmentsX*m_segmentsY; i++) {
		//if not on edge (use y segments to calculate)
		
		double x1 = vertexList[a][0];
		double y1 = vertexList[a][1];
		double z1 = vertexList[a][2];
		double x2 = vertexList[b][0];
		double y2 = vertexList[b][1];
		double z2 = vertexList[b][2];
		double x3 = vertexList[c][0];
		double y3 = vertexList[c][1];
		double z3 = vertexList[c][2];
		double x4 = vertexList[d][0];
		double y4 = vertexList[d][1];
		double z4 = vertexList[d][2];
		std::cout<<"point 1"<<x1<<y1<<z1<<std::endl;
		

		//setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);


		//setNormal(x2, y2, z2, x3, y3, z3, x4, y4, z4);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);
		glVertex3f(x4, y4, z4);


		if ((i+1)%m_segmentsY == 0) {
			a += 2;
			b += 2;
			c += 2;
			d += 2;
		} else {
			a++;
			b++;
			c++;
			d++;
		}

	}

	glEnd();
}





/*
void Cube::drawNormal() {

}*/