#include "Camera.h"

Camera::Camera() {
	/*
	eye = Point(2.0, 2.0, 2.0);
	up = Vector(0, 0, 0);
	look = Vector(-2.0, -2.0, -2.0);
	view = 45.0;
	near = 0.001;
	far = 30.0;
	width = 10.0;
	height = 10.0;
	*/
}

Camera::~Camera() {
}

void Camera::Orient(Point& eyePoint, Point& focus, Vector& upVector) {
	eye =  eyePoint;
	look = Vector(focus[0], focus[1], focus[2]);
	up = upVector;
}


void Camera::Orient(Point& eyePoint, Vector& lookVector, Vector& upVector) {
	eye =  eyePoint;
	look = lookVector;
	up =  upVector;

}

Matrix Camera::GetProjectionMatrix() {
	double near = GetNearPlane();
	double far = GetFarPlane();
	double c = -near/far;
	double theta_h = DEG_TO_RAD(view);
	double theta_w = theta_h * GetScreenWidthRatio();
	double h = 1.0 / (tan(theta_h/2.0) * far);
	double w = 1.0 / (tan(theta_w/2.0) * far);

	Matrix m1 = Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.0/(c+1.0), c/(c+1.0), 0, 0, -1, 0);
	Matrix m2 = Matrix(w, 0, 0, 0, 0, h, 0, 0, 0, 0, 1.0/far, 0, 0, 0, 0, 1);

	return m1*m2;



}


void Camera::SetViewAngle (double viewAngle) {
	//view = DEG_TO_RAD(viewAngle);
	view = viewAngle;

	//double ratio = GetScreenWidthRatio();
	//view = ratio + viewAngle;

}

void Camera::SetNearPlane (double nearPlane) {
	near = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
	far = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
	width = screenWidth;
	height = screenHeight;
}

Matrix Camera::GetModelViewMatrix() {
	Vector up = GetUpVector();
	Vector look = GetLookVector();
	Vector w = -1.0*look/ length(look);//sqrt( pow(look[0], 2) + pow(look[1], 2) + pow(look[2], 2) );
	Vector u = cross(up, w) / length(cross(up, w));
	Vector v = cross (w, u);
	Point e1 = GetEyePoint();
	Vector e = Vector(e1[0], e1[1], e1[2]);
	Matrix m1 = Matrix(u[0], u[1], u[2], 0, v[0], v[1], v[2], 0, w[0], w[1], w[2], 0, 0, 0, 0, 1);
	Matrix m2 = Matrix(1, 0, 0, -1.0*e[0], 0, 1, 0, -1.0*e[1], 0, 0, 1, -1.0*e[2], 0, 0, 0, 1);
	return m1 * m2;

}

//TODO
void Camera::RotateV(double angle) {

	Vector w = -1.0*look/ length(look);//sqrt( pow(look[0], 2) + pow(look[1], 2) + pow(look[2], 2) );
	Vector u = cross(up, w) / length(cross(up, w));
	Vector v = cross (w, u);
	Matrix m = rot_mat(v, DEG_TO_RAD(angle));
	look = m * look;
	
	up = m * up;
}

//TODO
void Camera::RotateU(double angle) {
	Vector w = -1.0*look/ length(look);//sqrt( pow(look[0], 2) + pow(look[1], 2) + pow(look[2], 2) );
	Vector u = cross(up, w) / length(cross(up, w));
	Matrix m = rot_mat(u, DEG_TO_RAD(angle));
	look = m * look;
	up = m * up;
}

void Camera::RotateW(double angle) {
	Vector w = -1.0*look/ length(look);//sqrt( pow(look[0], 2) + pow(look[1], 2) + pow(look[2], 2) );
	Matrix m = rot_mat(w, DEG_TO_RAD(angle));
	look = m * look;
	up = m * up;
}

//TODO
void Camera::Translate(const Vector &v) {
	Matrix m = Matrix(1, 0, 0, -1.0 * v[0], 0, 1, 0, -1.0 * v[1], 0, 0, 1, -1.0 * v[2], 0, 0, 0, 1);
	eye = m * eye;
	look = m * look;
	up = m * up;
}

//TODO
void Camera::Rotate(Point p, Vector axis, double degrees) {
	Matrix m1 = rotY_mat(-1.0*atan(axis[0]/axis[2]));
	double l = sqrt( pow(axis[0], 2) + pow(axis[1], 2) );
	double theta = asin(axis[0]/l) + PI/2;
	Matrix m2 = rotZ_mat(DEG_TO_RAD(theta));
	Matrix m3 = rotX_mat(DEG_TO_RAD(degrees));
	p = m1 * m2 * m3 * p;
}


Point Camera::GetEyePoint() {
	return eye;
}

Vector Camera::GetLookVector() {
	return look;
}

Vector Camera::GetUpVector() {
	return up;
}

double Camera::GetViewAngle() {
	return view;
}

double Camera::GetNearPlane() {
	return near;
}

double Camera::GetFarPlane() {
	return far;
}

int Camera::GetScreenWidth() {
	return width;
}

int Camera::GetScreenHeight() {
	return height;
}

//TODO
double Camera::GetFilmPlaneDepth() {
	return 0;
}

double Camera::GetScreenWidthRatio() {
	return width/height;
}
