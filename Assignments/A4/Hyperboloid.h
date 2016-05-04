#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include "Shape.h"

class Hyperboloid : public Shape {

public:
	Hyperboloid() {};
	~Hyperboloid() {};

	void draw() {
		Shape shape;

		glBegin(GL_TRIANGLES);

		std::vector<Point> lid = getlidPoints(m_segmentsX, m_segmentsY);
		std::vector<Point> points = getSidePoints(m_segmentsX, m_segmentsY);

		//creates the top and bottom lids
		for (int i = 1; i < m_segmentsX; i++) {
			double x1 = lid[i][0];
			double y1 = lid[i][1];
			double z1 = lid[i][2];
			double x2 = lid[i+1][0];
			double y2 = lid[i+1][1];
			double z2 = lid[i+1][2];

			setNormal(0, .5, 0, x2, y2, z2, x1, y1, z1);
			glVertex3f(x1, 0.5, z1);
			glVertex3f(x2, 0.5, z2);
			glVertex3f(0, .5, 0);

			setNormal(x1, -0.5, z1, x2, -0.5, z2, 0, -.5, 0);
			glVertex3f(x1, -0.5, z1);
			glVertex3f(x2, -0.5, z2);
			glVertex3f(0, -.5, 0);

		}
		//connects the max segment to the zero segment on lids
		setNormal(lid[m_segmentsX][0], lid[m_segmentsX][1], lid[m_segmentsX][2],
			0, .5, 0, lid[1][0], lid[1][1], lid[1][2]);

		glVertex3f(lid[m_segmentsX][0], lid[m_segmentsX][1], lid[m_segmentsX][2]);
		glVertex3f(0, .5, 0);
		glVertex3f(lid[1][0], lid[1][1], lid[1][2]);

		setNormal(lid[1][0], -0.5, lid[1][2], lid[0][0], -0.5, lid[0][2],
				  lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2]);
		glVertex3f(lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2]);
		glVertex3f(lid[0][0], -0.5, lid[0][2]);
		glVertex3f(lid[1][0], -0.5, lid[1][2]);

		//renders the side of the hyperboloid
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
		//renders max seg to zero for the sides
		for (int i = 0; i < m_segmentsY - 1; i++) {
			setNormal(points[(m_segmentsX-1)*m_segmentsY + i][0], points[(m_segmentsX-1)*m_segmentsY + i][1], points[(m_segmentsX-1)*m_segmentsY + i][2],
				points[i][0], points[i][1], points[i][2],
				points[i+1][0], points[i+1][1], points[i+1][2]);		
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + i][0], points[(m_segmentsX-1)*m_segmentsY + i][1], points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[i][0], points[i][1], points[i][2]);
			glVertex3f(points[i+1][0], points[i+1][1], points[i+1][2]);


			setNormal(points[i+1][0], points[i+1][1], points[i+1][2], points[(m_segmentsX-1)*m_segmentsY + 1 +i][0], points[(m_segmentsX-1)*m_segmentsY + 1 + i][1], points[(m_segmentsX -1)*m_segmentsY + 1 + i][2],
				points[(m_segmentsX-1)*m_segmentsY + i][0], points[(m_segmentsX-1)*m_segmentsY + i][1], points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + i][0], points[(m_segmentsX-1)*m_segmentsY + i][1], points[(m_segmentsX-1)*m_segmentsY + i][2]);
			glVertex3f(points[(m_segmentsX-1)*m_segmentsY + 1 +i][0], points[(m_segmentsX-1)*m_segmentsY + 1 + i][1], points[(m_segmentsX -1)*m_segmentsY + 1 + i][2]);
			glVertex3f(points[i+1][0], points[i+1][1], points[i+1][2]);

		}
		//renders bottom 'rung' of the hyperboloid
		for (int i = 0; i < m_segmentsX - 1; i++){

			setNormal(lid[i + 1][0], -0.5, lid[i + 1][2],
				points[i*m_segmentsY + m_segmentsY - 1][0],
					   points[i*m_segmentsY + m_segmentsY - 1][1],
					   points[i*m_segmentsY + m_segmentsY - 1][2],
					   lid[i + 2][0], -0.5, lid[i +2][2]);
			glVertex3f(lid[i + 1][0], -0.5, lid[i + 1][2]);
			glVertex3f(points[i*m_segmentsY + m_segmentsY - 1][0],
					   points[i*m_segmentsY + m_segmentsY - 1][1],
					   points[i*m_segmentsY + m_segmentsY - 1][2]);
			glVertex3f(lid[i + 2][0], -0.5, lid[i +2][2]);

			
			setNormal(points[i*m_segmentsY + m_segmentsY - 1][0],
					   points[i*m_segmentsY + m_segmentsY - 1][1],
					   points[i*m_segmentsY + m_segmentsY - 1][2],
					   points[i*m_segmentsY + m_segmentsY 
											+ m_segmentsY - 1][0],
				       points[i*m_segmentsY + m_segmentsY 
				       						+ m_segmentsY - 1][1],
				       points[i*m_segmentsY + m_segmentsY 
				       						+ m_segmentsY - 1][2],
				   	lid[i + 2][0], -0.5, lid[i +2][2]);


			glVertex3f(points[i*m_segmentsY + m_segmentsY - 1][0],
					   points[i*m_segmentsY + m_segmentsY - 1][1],
					   points[i*m_segmentsY + m_segmentsY - 1][2]);
			glVertex3f(points[i*m_segmentsY + m_segmentsY 
											+ m_segmentsY - 1][0],
				       points[i*m_segmentsY + m_segmentsY 
				       						+ m_segmentsY - 1][1],
				       points[i*m_segmentsY + m_segmentsY 
				       						+ m_segmentsY - 1][2]);
			glVertex3f(lid[i + 2][0], -0.5, lid[i +2][2]);
		}

		//connects the max seg and zero seg for bottom rung
		setNormal(lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2],
			points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][0],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][1],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][2],
			lid[1][0], -0.5, lid[1][2]);
		glVertex3f(lid[m_segmentsX][0], -0.5, lid[m_segmentsX][2]);
		glVertex3f(points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][0],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][1],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][2]);
		glVertex3f(lid[1][0], -0.5, lid[1][2]);
			

		setNormal(points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][0],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][1],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][2],
				   points[m_segmentsY - 1][0],
		 	       points[m_segmentsY - 1][1],
		 	       points[m_segmentsY - 1][2],
		 	       lid[1][0], -0.5, lid[1][2]);
		glVertex3f(points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][0],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][1],
				   points[(m_segmentsX-1)*m_segmentsY + m_segmentsY - 1][2]);
		glVertex3f(points[m_segmentsY - 1][0],
		 	       points[m_segmentsY - 1][1],
		 	       points[m_segmentsY - 1][2]);
		glVertex3f(lid[1][0], -0.5, lid[1][2]);



		glEnd();
	};

	void drawNormal() {

		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		double x1, y1, z1, x2, y2, z2;
		double r1, r2;
		double theta, z;

		for (int i = 0; i <= m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				z = 0.5 - (i + 0.0) / m_segmentsY;
				r1 = SQR(.5) + SQR(z);
				r2 = r1 + .1;
				theta = 2 * PI * j / m_segmentsX;
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

	//points to get the top and bottom lids of the hyperboloid
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

	//vector of points of the side of the hyperboloid
	std::vector<Point> getSidePoints(int m_segmentsX, int m_segmentsY) {
		std::vector<Point> points;
		double r, x, y, z, theta;
		double a, b; 

		for (int i = 0; i < m_segmentsX; i++) {
			for (int j = 0; j < m_segmentsY; j++) {
				z = 0.5 - (j + 0.0) / m_segmentsY;
				r = SQR(.5) + SQR(z);
				theta = 2.0 * PI * i / m_segmentsX;
				x = r * cos(theta);
				y = r * sin(theta);
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
