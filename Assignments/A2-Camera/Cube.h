#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include <iostream>


class Cube : public Shape {
public:
	Cube() {};
	~Cube() {};

	void draw() {
		
		Shape shape;

		std::vector<Point> *face = new std::vector<Point>[6];
		face = getPoints(m_segmentsX,m_segmentsY);


		glBegin(GL_TRIANGLES);
		double a, b, c, d;
		a = 0;
		b = a + 1;
		c = a + m_segmentsY + 1;
		d = c + 1;

			//iterates through the x and y segments for each individual face
			for (int k = 0; k < m_segmentsX*m_segmentsY; k++) {

				double *v1 = new double[4];
				double *v2 = new double[4];
				double *v3 = new double[4];
				double *v4 = new double[4];


				for(int m = 0; m < 6; m++){

					face[m][a].unpack(v1);
					face[m][b].unpack(v2);
					face[m][c].unpack(v3);
					face[m][d].unpack(v4);

					double x1 = v1[0];

					double y1 = v1[1];
					double z1 = v1[2];
					double x2 = v2[0];
					double y2 = v2[1];
					double z2 = v2[2];
					double x3 = v3[0];
					double y3 = v3[1];
					double z3 = v3[2];
					double x4 = v4[0];
					double y4 = v4[1];
					double z4 = v4[2];

					if (m == 0 || m == 3 || m == 5){
						setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3);
					}
					if (m == 1 || m == 2 || m == 4){
						setNormal(x3, y3, z3, x2, y2, z2, x1, y1, z1);
					}
					glVertex3f(x1, y1, z1);
					glVertex3f(x2, y2, z2);
					glVertex3f(x3, y3, z3);

					if (m == 1 || m == 2 || m == 4){ 
						setNormal(x2, y2, z2, x3, y3, z3, x4, y4, z4);
					}
					if (m == 0 || m == 3 || m == 5){
						setNormal(x4, y4, z4, x3, y3, z3, x2, y2, z2);
					}

					glVertex3f(x2, y2, z2);
					glVertex3f(x3, y3, z3);
					glVertex3f(x4, y4, z4);


				}
				//correctly iterates through the segments
				if ((k+1) % m_segmentsY == 0) {
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
	};


	void drawNormal() {

		std::vector<Point> *face = new std::vector<Point>[6];
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);

		face = getPoints(m_segmentsX,m_segmentsY);

		
		for (int i = 0; i < 6; i++){
			for(int j = 0; j < ((m_segmentsX+1)*(m_segmentsY+1)); j++){
				switch (i) {
					case 0:
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]-0.1);
						break;
					case 1:
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]+0.1);
						break;
					case 2:
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0], face[i][j][1]-0.1, face[i][j][2]);
						break;
					case 3:
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0], face[i][j][1]+0.1, face[i][j][2]);
						break;
					case 4:
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0]-0.1, face[i][j][1], face[i][j][2]);
						break;
					case 5: 
						glVertex3f(face[i][j][0], face[i][j][1], face[i][j][2]);
						glVertex3f(face[i][j][0]+0.1, face[i][j][1], face[i][j][2]);
						break;

				}
			}
		}
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);

		glEnd();

	};


protected:
	std::vector<Point> * getPoints(int m_segmentsX, int m_segmentsY){
		std::vector<Point> *face = new std::vector<Point>[6];
	 	
		double dx = 1.0/m_segmentsX;
		double dy = 1.0/m_segmentsY;
		double dz = 1.0/m_segmentsX;
		double x  = -.5;
		double y  = -.5;
		double z  = -.5;
		int index = 0;
		//create an array of 6 (faces) point vectors
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
				y += dy;
			}
			x += dx;
		}
		return face;
	};



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
