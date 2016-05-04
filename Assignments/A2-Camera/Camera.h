

#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
public:
Camera();
~Camera();
void Orient(Point& eye, Point& focus, Vector& up);
void Orient(Point& eye, Vector& look, Vector& up);
void SetViewAngle (double viewAngle);
void SetNearPlane (double nearPlane);
void SetFarPlane (double farPlane);
void SetScreenSize (int screenWidth, int screenHeight);

Matrix GetProjectionMatrix();
Matrix GetModelViewMatrix();

void RotateV(double angle);
void RotateU(double angle);
void RotateW(double angle);
void Rotate(Point p, Vector axis, double degree);
void Translate(const Vector &v);

Point GetEyePoint();
Vector GetLookVector();
Vector GetUpVector();
double GetViewAngle();
double GetNearPlane();
double GetFarPlane();
int GetScreenWidth();
int GetScreenHeight();

double GetFilmPlaneDepth();
double GetScreenWidthRatio();



private:

	Point eye;
	Vector up;
	Vector look;
	double view;
	double near;
	double far;
	double width;
	double height;

	//Matrix projection;

	//camera position
	//orientation
	//field of view: viewangle: wide, narrow, etc
	//depth of field (near distance, far distance)
	//perspective of parallel projection
	


};
#endif

