#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	void draw() {
		Shape shape;
		glBegin(GL_TRIANGLES);

		std::vector<Point> lid = getlidPoints(m_segmentsX, m_segmentsY);

		//create top and bottom lids for cylinder
		for (int i = 1; i < m_segmentsX; i++) {
			double x1 = lid[i][0];
			double y1 = lid[i][1];
			double z1 = lid[i][2];
			double x2 = lid[i+1][0];
			double y2 = lid[i+1][1];
			double z2 = lid[i+1][2];

			setNormal(x1, 0.5, z1, 0, .5, 0, x2, 0.5, z2);
			glVertex3f(x1, 0.5, z1);
			glVertex3f(x2, 0.5, z2);
			glVertex3f(0, .5, 0);

			setNormal(x1, -0.5, z1, x2, -0.5, z2, 0, -0.5, 0);
			glVertex3f(x1, -0.5, z1);
			glVertex3f(x2, -0.5, z2);
			glVertex3f(0, -.5, 0);

		}
		//connects max seg with zero seg for top and bottom
		setNormal(lid[m_segmentsX][0], 0.5, lid[m_segmentsX][2], 0, .5, 0,
			      lid[1][0], 0.5, lid[1][2]);
		glVertex3f(lid[m_segmentsX][0], 0.5, lid[m_segmentsX][2]);
		glVertex3f(0, .5, 0);
		glVertex3f(lid[1][0], 0.5, lid[1][2]);

		setNormal(lid[1][0], -0.5, lid[1][2], lid[0][0], -0.5, lid[0][2],
				  lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2]);
		glVertex3f(lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2]);
		glVertex3f(lid[0][0], -0.5, lid[0][2]);
		glVertex3f(lid[1][0], -0.5, lid[1][2]);
		
		//creates the body of the clyinder
		for (int i = 1; i <= m_segmentsX; i++) {
			double dy1 = 1.0/m_segmentsY;
			double dy2 = dy1;
			double dy3 = 0;

			for (int j = 0; j < m_segmentsY; j++){
				double x1 = lid[i][0];
				double y1 = lid[i][1];
				double z1 = lid[i][2];
				double x2 = lid[i+1][0];
				double y2 = lid[i+1][1];
				double z2 = lid[i+1][2];

				//makes the top and bottom triangles for each m_segmentY square
				setNormal(x1, y1-dy3, z1, x2, y2-dy3, z2, x2, y2-dy2, z2);
				glVertex3f(x1, y1-dy3, z1);
				glVertex3f(x2, y2-dy3, z2);
				glVertex3f(x2, y2-dy2, z2);

				setNormal(x2, y2-dy2, z2, x1, y1-dy2, z1, x1, y1-dy3, z1);
				glVertex3f(x1, y1-dy3, z1);
				glVertex3f(x1, y1-dy2, z1);
				glVertex3f(x2, y2-dy2, z2);

				dy2 += dy1;
				dy3 += dy1;
			}
		}

		glEnd();
	};

	void drawNormal() {


		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		double x1, y1, z1, x2, y2, z2;
		double r1 = .5;
		double r2 = .6;
		double theta, z;
        
        glVertex3f(0, .5, 0);
        glVertex3f(0, .6, 0);
        glVertex3f(0, -.5, 0);
        glVertex3f(0, -.6, 0);

		//creates normals layer by layer
		for (int i = 0; i <= m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				theta = 2 * PI * j / m_segmentsX;
				z = .5 - (i+0.0) / m_segmentsY;
				x1 = r1 * cos(theta);
				y1 = r1 * sin(theta);
				x2 = r2 * cos(theta);
				y2 = r2 * sin(theta);
				glVertex3f(x1, z, y1);
				glVertex3f(x2, z, y2);
			}
		}

		glEnd();
	};


protected:
	//creates the vector of points holding the lids of the cylinder
	std::vector<Point> getlidPoints(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> lid;// = new std::vector<Point>;
		double r = .5;
		double z = .5;
		double x, y;
		lid.push_back(Point(0, .5, 0));
		for (int i = 0; i <= m_segmentsX; i++) {
			x = r * cos(2*PI*i/m_segmentsX);
			y = r * sin(2*PI*i/m_segmentsX);
			lid.push_back(Point(x, z, y));
		}
		return lid;

	}

	void setNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3) {


		float v1x, v1y, v1z;
		float v2x, v2y, v2z;
		float cx, cy, cz;

		//find vector between x2 and x1
		v1x = x1 - x2;
		v1y = y1 - y2;
		v1z = z1 - z2;

		//find vector between x3 and x2
		v2x = x2 - x3;
		v2y = y2 - y3;
		v2z = z2 - z3;

		//cross product v1xv2
		cx = v1y * v2z - v1z * v2y;
		cy = v1z * v2x - v1x * v2z;
		cz = v1x * v2y - v1y * v2x;

		//normalize
		float length = sqrt(cx * cx + cy * cy + cz * cz);
		cx = cx / length;
		cy = cy / length;
		cz = cz / length;

		glNormal3f(cx, cy, cz);
	}



};

#endif
