#ifndef COHE_H
#define CONE_H

#include "Shape.h"
#include <iostream>


class Cone : public Shape {
public:
	Cone() {

	};

	~Cone() {};

	void draw() {
		Shape shape;
		Point tip = Point(0, .5, 0);
		std::vector<Point> peak = getPeak(m_segmentsX, m_segmentsY);
		std::vector<Point> rest = getRest(m_segmentsX, m_segmentsY);

		//tip of cone
		glBegin(GL_TRIANGLES);
		double x1 = tip[0];
		double y1 = tip[1];
		double z1 = tip[2]; 


		//draw peak
		for (int i = 0; i < peak.size() - 1; i++) {
			double x2 = peak[i][0];
			double y2 = peak[i][1];
			double z2 = peak[i][2];
			double x3 = peak[i + 1][0];
			double y3 = peak[i + 1][1];
			double z3 = peak[i + 1][2];


			setNormal(x3, y3, z3, x2, y2, z2, x1, y1, z1);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
		}

		
		setNormal(x1, y1, z1, peak[0][0], peak[0][1], peak[0][2],
			      peak[peak.size() - 1][0], peak[peak.size() - 1][1],
			      peak[peak.size() - 1][2]);

		glVertex3f(x1, y1, z1);
		glVertex3f(peak[0][0], peak[0][1], peak[0][2]);
		glVertex3f(peak[peak.size() - 1][0], peak[peak.size() - 1][1],
				   peak[peak.size() - 1][2]);



		//draw body of the cone
		for (int j = 0; j < m_segmentsX - 1; j++){
			for (int k = 0; k < (m_segmentsY-1); k++) { 
				int index = j*m_segmentsY + k;
				double x1 = rest[index][0];
				double y1 = rest[index][1];
				double z1 = rest[index][2];
				double x2 = rest[index + 1][0];
				double y2 = rest[index + 1][1];
				double z2 = rest[index + 1][2];
				double x3 = rest[index+m_segmentsY][0];
				double y3 = rest[index+m_segmentsY][1];
				double z3 = rest[index+m_segmentsY][2];
				double x4 = rest[index+m_segmentsY + 1][0];
				double y4 = rest[index+m_segmentsY + 1][1];
				double z4 = rest[index+m_segmentsY + 1][2];

				//top and bottom triangles of each y segmented square
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
		//connect max x segment to the zero x segment(last one from loop above)
		for (int i = 0; i < m_segmentsY - 1; i++) {
			setNormal(rest[(m_segmentsX - 1)*m_segmentsY + i][0],
					  rest[(m_segmentsX - 1)*m_segmentsY + i][1],
					  rest[(m_segmentsX - 1)*m_segmentsY + i][2],
					  rest[i][0], rest[i][1], rest[i][2], rest[i + 1][0],
					  rest[i + 1][1], rest[i + 1][2]);

			glVertex3f(rest[(m_segmentsX - 1)*m_segmentsY + i][0],
					   rest[(m_segmentsX - 1)*m_segmentsY + i][1],
					   rest[(m_segmentsX - 1)*m_segmentsY + i][2]);
			glVertex3f(rest[i][0], rest[i][1], rest[i][2]);
			glVertex3f(rest[i + 1][0], rest[i + 1][1], rest[i+1][2]);

			setNormal(rest[i+1][0], rest[i+1][1], rest[i+1][2],
					  rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][0],
					  rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][1],
					  rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][2],
					  rest[(m_segmentsX - 1)*m_segmentsY + i][0],
					  rest[(m_segmentsX - 1)*m_segmentsY + i][1],
					  rest[(m_segmentsX - 1)*m_segmentsY + i][2]);

			glVertex3f(rest[(m_segmentsX - 1)*m_segmentsY + i][0],
					   rest[(m_segmentsX - 1)*m_segmentsY + i][1],
					   rest[(m_segmentsX - 1)*m_segmentsY + i][2]);
			glVertex3f(rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][0],
					   rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][1],
					   rest[(m_segmentsX - 1)*m_segmentsY + 1 + i][2]);
			glVertex3f(rest[i + 1][0], rest[i + 1][1], rest[i + 1][2]);

		}

		// draw cone base
		for (int i = 0; i < m_segmentsX; i++) {
			double r = .5;
			double z = -.5;
			double theta1 = 2 * PI * i / m_segmentsX;
			double theta2 = 2 * PI * (i + 1.0) / m_segmentsX;
			double x1 = r * cos(theta1);
			double y1 = r * sin(theta1);
			double x2 = r * cos(theta2);
			double y2 = r * sin(theta2);

			setNormal(x1, z, y1, x2, z, y2, 0, -.5, 0);
			glVertex3f(x1, z, y1);
			glVertex3f(x2, z, y2);
			glVertex3f(0, -.5, 0);
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
		double r1, r2;
		double theta, z;

		//creates normals layer by layer
		for (int i = 1; i <= m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				theta = 2 * PI * j / m_segmentsX;
				z = .5 - (i+0.0) / m_segmentsY;
				r1 = (i+0.0)/2.0/m_segmentsY;
				r2 = r1 + .1;
				x1 = r1 * cos(theta);
				y1 = r1 * sin(theta);
				x2 = r2 * cos(theta);
				y2 = r2 * sin(theta);
				glVertex3f(x1, z, y1);
				glVertex3f(x2, z + .05, y2);
			}
		}

		glEnd();
	};


protected:
	
	std::vector<Point> getPeak(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> peak;
		double r = .5/m_segmentsY;
		double x, y, z;
		for (int i = 0; i < m_segmentsX; i++) {
			x = r * cos(2.0 * PI * i / m_segmentsX);
			y = r * sin(2.0 * PI * i / m_segmentsX);
			z = .5 - 1.0/m_segmentsY;
			peak.push_back(Point(x, z, y));
		}
		return peak;
	}

	std::vector<Point> getRest(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> rest;// = new std::vector<Point>;
		double r, x, y, z;

		for (int i = 0; i < m_segmentsX; i++) {
			for (int j = 0; j < m_segmentsY; j++) {
				r = (j + 1.0)/2.0/m_segmentsY;
				x = r * cos(2.0 * PI * i / m_segmentsX);
				y = r * sin(2.0 * PI * i / m_segmentsX);
				z = .5 - (j+1.0)/m_segmentsY;
				rest.push_back(Point(x, z, y));
			}
		}
		return rest;


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
