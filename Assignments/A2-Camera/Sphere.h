#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

	void draw() {
		Shape shape;

		Point top = Point(0, .5, 0);
		Point bottom = Point(0, -.5, 0);
		std::vector<Point> bottom_points = getBottomPoints(m_segmentsX, m_segmentsY);
		std::vector<Point> points = getSidePoints(m_segmentsX, m_segmentsY);

		glBegin(GL_TRIANGLES);

		//renders bottom part of sphere
		for (int i = 1; i < m_segmentsX; i++) {
			double x3 = bottom_points[i][0];
			double y3 = bottom_points[i][1];
			double z3 = bottom_points[i][2];
			double x4 = bottom_points[i+1][0];
			double y4 = bottom_points[i+1][1];
			double z4 = bottom_points[i+1][2];

			setNormal(x3, y3, z3, x4, y4, z4, 0, -.5, 0);
			glVertex3f(x3, y3, z3);
			glVertex3f(x4, y4, z4);
			glVertex3f(0, -.5, 0);

		}
		//connects max segments and 0 segments
		setNormal( bottom_points[1][0], bottom_points[1][1],
			       bottom_points[1][2], 0, -.5, 0,
			       bottom_points[m_segmentsX][0],
			       bottom_points[m_segmentsX][1],
			       bottom_points[m_segmentsX][2]);
		glVertex3f(bottom_points[m_segmentsX][0],
				   bottom_points[m_segmentsX][1],
				   bottom_points[m_segmentsX][2]);
		glVertex3f(0, -0.5, 0);
		glVertex3f(bottom_points[1][0], bottom_points[1][1],
				   bottom_points[1][2]);
		
		

		//creates middle & top of sphere
		for (int j = 0; j < m_segmentsX - 1; j++){
			for (int k = 0; k < (m_segmentsY-1); k++) { 
				int index = j*m_segmentsY + k;
				double x1 = points[index][0];
				double y1 = points[index][1];
				double z1 = points[index][2];
				double x2 = points[index+1][0];
				double y2 = points[index+1][1];
				double z2 = points[index+1][2];
				double x3 = points[index+m_segmentsY][0];
				double y3 = points[index+m_segmentsY][1];
				double z3 = points[index+m_segmentsY][2];
				double x4 = points[index+m_segmentsY + 1][0];
				double y4 = points[index+m_segmentsY + 1][1];
				double z4 = points[index+m_segmentsY + 1][2];

				setNormal(x1, y1, z1, x3, y3, z3, x4, y4, z4);
			    glVertex3f(x1, y1, z1);
				glVertex3f(x3, y3, z3);
				glVertex3f(x4, y4, z4);

				setNormal(x4, y4, z4, x2, y2, z2, x1, y1, z1);
				glVertex3f(x1, y1, z1);
				glVertex3f(x2, y2, z2);
				glVertex3f(x4, y4, z4);
			}
		}
		//connects max segments and 0 segments
		for (int i = 0; i < m_segmentsY - 1; i++) {

			setNormal( points[(m_segmentsX-1)*m_segmentsY + i][0],
					   points[(m_segmentsX-1)*m_segmentsY + i][1],
					   points[(m_segmentsX-1)*m_segmentsY + i][2],
					   points[i][0], points[i][1], points[i][2],
					   points[i+1][0], points[i+1][1], points[i+1][2]);
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + i][0],
					   points[(m_segmentsX-1)*m_segmentsY + i][1],
					   points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[i][0], points[i][1], points[i][2]);
			glVertex3f(points[i+1][0], points[i+1][1], points[i+1][2]);

			setNormal( points[i+1][0], points[i+1][1], points[i+1][2], 
					   points[(m_segmentsX-1)*m_segmentsY + 1 +i][0],
					   points[(m_segmentsX-1)*m_segmentsY + 1 + i][1],
					   points[(m_segmentsX -1)*m_segmentsY + 1 + i][2],
					   points[(m_segmentsX-1)*m_segmentsY + i][0],
					   points[(m_segmentsX-1)*m_segmentsY + i][1],
					   points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + i][0],
					   points[(m_segmentsX-1)*m_segmentsY + i][1],
					   points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + 1 +i][0],
					   points[(m_segmentsX-1)*m_segmentsY + 1 + i][1],
					   points[(m_segmentsX -1)*m_segmentsY + 1 + i][2]);
			glVertex3f(points[i+1][0], points[i+1][1], points[i+1][2]);

		}


		glEnd();



	};

	void drawNormal() {
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);

		glVertex3f(0, .5, 0);
		glVertex3f(0, .6, 0);

		glVertex3f(0, -.5, 0);
		glVertex3f(0, -.6, 0);

		double x1, y1, z1, x2, y2, z2;
		double r1 = .5;
		double r2 = .6;
		double phi, theta;

		for (int i = 1; i < m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				theta = 2.0 * PI * j / m_segmentsX;
				phi = PI * i / m_segmentsY;
				x1 = r1 * cos(theta) * sin(phi);
				y1 = r1 * sin(theta) * sin(phi);
				z1 = r1 * cos(phi);
				x2 = r2 * cos(theta) * sin(phi);
				y2 = r2 * sin(theta) * sin(phi);
				z2 = r2 * cos(phi);				
				glVertex3f(x1, z1, y1);
				glVertex3f(x2, z2, y2);
			}
		}

		glEnd();
	};

protected:
	// returns vector of points for the bottom ring 
	std::vector<Point> getBottomPoints(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> bottom;
		double x, y, z, theta;
		double phi = PI - (PI / m_segmentsY);
		double r = .5;
		for (int i = 0; i <= m_segmentsX; i++) {
			theta = 2.0 * PI * i / m_segmentsX;
			x = r * cos(theta) * sin(phi);
			y = r * sin(theta) * sin(phi);
			z = r * cos(phi);
			bottom.push_back(Point(x, z, y));
		}

		return bottom;
	}


	// returns vector of side points in order top to bottom and then around ... does that make sense
	std::vector<Point> getSidePoints(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> points;
		double x, y, z, theta, phi;
		double r = .5;
		for (int i = 0; i < m_segmentsX; i++) {
			for (int j = 0; j < m_segmentsY; j++) {
				theta = 2.0 * PI * i / m_segmentsX;
				phi = PI * j / m_segmentsY;
				x = r * cos(theta) * sin(phi);
				y = r * sin(theta) * sin(phi);
				z = r * cos(phi);
				points.push_back(Point(x, z, y));
			}
		}
		return points;
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
