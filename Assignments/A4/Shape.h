#ifndef SHAPE_H
#define SHAPE_H

#include </Library/Frameworks/GLUI.framework/Headers/glui.h>
#include "Algebra.h"
#include <vector>

class Shape {
public:
	Shape() {};
	~Shape() {};
	int m_segmentsX, m_segmentsY;

	void setSegments(int x, int y) {
		m_segmentsX = x;
		m_segmentsY = y;
	}
	int getxseg(){
		return m_segmentsX;
	}
	int getyseg(){
		return m_segmentsY;
	}

	virtual void draw() {};
	virtual void drawNormal() {};

	
	//std::vector<Point> pointVector;

protected:
	void normalizeNormal (float x, float y, float z) {
		normalizeNormal (Vector(x, y, z));
	};

	void normalizeNormal (Vector v) {
		v.normalize();
		glNormal3dv(v.unpack());
	};


	std::vector<Point> * getPoints(int m_segmentsX, int m_segmentsY){
		std::vector<Point> *face = new std::vector<Point>[6];
	 	
		//std::cout << m_segmentsX<< std::endl;
		double dx = 1.0/m_segmentsX;
		double dy = 1.0/m_segmentsY;
		double dz = 1.0/m_segmentsX;


		//double dz = 1/m_segmentsX;
		double x  = -.5;
		double y  = -.5;
		double z  = -.5;
		int index = 0;

		for (int i = 0; i <= m_segmentsX; i++) {
			y = -.5;
			for (int j = 0; j <= m_segmentsY; j++) {

				(face[0]).push_back(Point(x, y, -0.5));
				(face[1]).push_back(Point(x, y,  0.5));
				(face[2]).push_back(Point(x, -0.5, y));
				(face[3]).push_back(Point(x,  0.5, y));
				(face[4]).push_back(Point(-0.5, y, x));
				(face[5]).push_back(Point( 0.5, y, x));

				index ++;
				//std::cout<<"y "<<y<<std::endl;
				y += dy;
			}
			x += dx;
		}
		return face;
	};

};

#endif
