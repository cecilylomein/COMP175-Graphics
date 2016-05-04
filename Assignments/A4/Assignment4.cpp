#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include </Library/Frameworks/GLUI.framework/Headers/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"
#include "vector"
#include "Algebra.h"
#include <math.h>

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

std::vector<SceneNode*> tree;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/general/test.xml";
int recursion;
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();


void setupCamera();
void updateCamera();
SceneColor raytrace(int w, int h);
SceneColor findNearestIntersection(Point eye, Vector d);
SceneColor computeSceneColor(SceneMaterial material, Vector Nhat, Point pos);
Vector computeNormal(Point intersection, PrimitiveType shape);
float solveCube(Point eye, Vector d);
float solveFace(Point eye, Vector d, int i, float n);
float solveCylinder(Point eye, Vector d);
float minPos(float x, float y);
float solveCone(Point eye, Vector d);
float solveSphere(Point eye, Vector d);
float solveQuadratic(double A, double B, double C);
void flatten_tree(SceneNode* n, SceneTransformation* t);

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

void callback_start(int id) {
	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	

	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
		    SceneColor color = raytrace(i, j);	
            setPixel(pixels, i, j, color.r, color.g, color.b);
		}
	}
	glutPostRedisplay();
}

SceneColor raytrace(int w, int h) {
    Point p_eye = Point(0, 0, 0);
    Point lookAt = Point(-1 + 2.0*w/screenWidth, -1 + 2.0*h/screenHeight, -1);

	double far = camera->GetFarPlane();
    double theta_h = DEG_TO_RAD(viewAngle);
	double theta_w = 2.0 * atan(camera->GetScreenWidth() * tan(theta_h/2.0) / camera->GetScreenHeight());
	double height = 1.0 / (tan(theta_h/2.0) * far);
	double width = 1.0 / (tan(theta_w/2.0) * far);

    Vector scale(
           width,
           height,
           1 / far);
    Matrix invScale = inv_scale_mat(scale);

    Matrix invModelView = invert(camera->GetModelViewMatrix());
    
    Point eye_world = invModelView * invScale * p_eye;
    Point lookAt_world = invModelView * invScale * lookAt;
    
    Vector d = lookAt_world - eye_world;
	//std::cout << "d " << lookAt_world[0] << ", " << lookAt_world[1] << ", " << lookAt_world[2] << std::endl;
    d.normalize();
    return findNearestIntersection(eye_world, d);
}

SceneColor findNearestIntersection(Point eye, Vector d){
    float t_min = -1;
    SceneColor color;
    color.r = 0, color.g = 0, color.b = 0;

    for (int i = 0; i < tree.size(); i++) {
        for (auto const& prim : tree[i]->primitives) {
            float t;
            Matrix m       = tree[i]->transformations[0]->matrix;
            Matrix m_inv   = invert(m);
            Point  eye_obj = m_inv * eye;
            Vector d_obj   = m_inv * d;

            switch(prim->type) {
                case SHAPE_CUBE:
                   t = solveCube(eye_obj, d_obj);
                   break;
                case SHAPE_CYLINDER:
                   t = solveCylinder(eye_obj, d_obj);
                   break; 
                case SHAPE_CONE:
                   t = solveCone(eye_obj, d_obj);
                   break; 
                case SHAPE_SPHERE:
                   t = solveSphere(eye_obj, d_obj);
                   break; 
            }
            
            if (t >= 0 && (t_min < 0 || t < t_min)) {
//              if (prim->type == SHAPE_CUBE) std::cout << t << std::endl;
                t_min = t;
                //Point intersection = eye + d * t_min;
                //Point intersection_obj = m_inv * intersection;
                Point intersection_obj = eye_obj + d_obj * t_min;
                Point intersection = transpose(m) * intersection_obj;
                Vector normal = computeNormal(intersection_obj, prim->type); 
                normal = transpose(m_inv) * normal;
                normal.normalize();
                color = computeSceneColor(prim->material, normal, intersection);
            }
        }
    }

    //if (t_min >= 0) color.r = 255;

	//if (t_min >= 0) std::cout << "color.g is  " << color.g << std::endl;
    //std::cout << isectOnly << endl;
	if (isectOnly && t_min >= 0) {
		color.r = 255;
		color.g = 255;
		color.b = 255;
        color.a = 255;
	}
    return color;
}

SceneColor computeSceneColor(SceneMaterial material, Vector Nhat, Point pos) {
	
    SceneGlobalData data;
    parser->getGlobalData(data);
    
    float ka = data.ka;

    float kd = data.kd;

    SceneColor Oa = material.cAmbient;
    SceneColor Od = material.cDiffuse;
    
    SceneColor color; 
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = 255;

    int nLights = parser->getNumLights();
    for (int m = 0; m < nLights; m++) {
        SceneLightData lData;
        parser->getLightData(m, lData);
        SceneColor lm = lData.color;
        Vector Lhatm = lData.pos - pos;
        Lhatm.normalize();
//         std::cout << "nhat is  " << Nhat[0] << Nhat[1] << Nhat[2] << std::endl;
        //std::cout << lm.g << std::endl;
       	if (Od.r > 1.0) Od.r = 1.0;
       	if (Od.g > 1.0) Od.g = 1.0;
       	if (Od.b > 1.0) Od.b = 1.0;
       	if (lm.r > 1.0) lm.r = 1.0;
       	if (lm.g > 1.0) lm.g = 1.0;
       	if (lm.b > 1.0) lm.b = 1.0;

        if (dot(Nhat, Lhatm) > 0) {
            color.r += (kd * Od.r * lm.r * dot(Nhat, Lhatm));  
            color.g += (kd * Od.g * lm.g * dot(Nhat, Lhatm));  
            color.b += (kd * Od.b * lm.b * dot(Nhat, Lhatm));  
        }
    }
    if (Oa.r > 1.0) Oa.r = 1.0;
   	if (Oa.g > 1.0) Oa.g = 1.0;
   	if (Oa.b > 1.0) Oa.b = 1.0;
    color.r += ka * (Oa.r);
    color.g += ka * (Oa.g);
    color.b += ka * (Oa.b);

    color.r *= 255;
    color.g *= 255;
    color.b *= 255;

    return color;
}

Vector computeNormal(Point intersection, PrimitiveType shape) {
    switch(shape) {
        case SHAPE_CUBE: {
            if (intersection[0] > .4999)
                return Vector(1, 0, 0);
            if (intersection[0] < -.4999)
                return Vector(-1, 0, 0);
            if (intersection[1] > .4999)
                return Vector(0, 1, 0);
            if (intersection[1] < -.4999)
                return Vector(0, -1, 0);
            if (intersection[2] > .4999)
                return Vector(0, 0, 1);
            if (intersection[2] < -.4999)
                return Vector(0, 0, -1);
            }
            break;
        case SHAPE_CYLINDER:
           if (IN_RANGE(intersection[1], 0.5))
               return Vector(0, 1, 0);
           if (IN_RANGE(intersection[1], -0.5))
               return Vector(0, -1, 0);
           return Vector(intersection[0], 0, intersection[2]);
           break; 
        case SHAPE_CONE: {
           if (IN_RANGE(intersection[1], -0.5))
               return Vector(0, -1, 0);
           Vector v1 = Vector(intersection[0], 0, intersection[2]);
           v1.normalize();
           Vector v2 = Vector(0, .5, 0);
           return v1 + v2; }
           break; 
        case SHAPE_SPHERE:
           return Vector(intersection[0], intersection[1], intersection[2]);
           break;
    }
}

float solveCube(Point eye, Vector d) {
    float t1 = solveFace(eye, d, 0, 0.5);
    float t2 = solveFace(eye, d, 1, 0.5);
    float t3 = solveFace(eye, d, 2, 0.5);
    float t4 = solveFace(eye, d, 0, -0.5);
    float t5 = solveFace(eye, d, 1, -0.5);
    float t6 = solveFace(eye, d, 2, -0.5);
    
    return minPos(t1, (minPos(t2, (minPos(t3, (minPos(t4, (minPos(t5, t6)))))))));
}

float solveFace(Point eye, Vector d, int i, float n) {
    float t = (n - eye[i]) / d[i];
    Point intersect = eye + d * t;
    if ((intersect[(i + 1) % 3] < 0.5 && intersect[(i + 1) % 3] > -0.5) &&
        (intersect[(i + 2) % 3] < 0.5 && intersect[(i + 2) % 3] > -0.5)) {
        return t;
    } else {
        return -1;
    }
}

float solveCylinder(Point eye, Vector d) {
    double A = d[0]*d[0] + d[2]*d[2];
    double B = 2*eye[0]*d[0] + 2*eye[2]*d[2];
    double C = eye[0]*eye[0] + eye[2]*eye[2] - 0.25;

    float t_body = solveQuadratic(A, B, C);
    Point intersect = eye + d * t_body;
    if (!(intersect[1] > -0.5 && intersect[1] < 0.5)) {
        t_body = -1;
    }

    float t_cap1 = (0.5 - eye[1]) / d[1];
    intersect = eye + d * t_cap1;
    if (!(intersect[0]*intersect[0] + intersect[2]*intersect[2] <= 0.25)) {
        t_cap1 = -1;
    }

    float t_cap2 = (-0.5 - eye[1]) / d[1];
    intersect = eye + d * t_cap2;
    if (!(intersect[0]*intersect[0] + intersect[2]*intersect[2] <= 0.25)) {
        t_cap2 = -1;
    }
    
    return minPos(t_body, minPos(t_cap1, t_cap2));
}

float minPos(float x, float y) {
    if (x < 0) {
        if (y < 0)
            return -1;
        else 
            return y;
    } else {
        if (y < 0)
            return x;
        else 
            return x < y ? x : y;
    }
}

float solveCone(Point eye, Vector d) {
    double A = d[0]*d[0] + d[2]*d[2] - (.25*d[1]*d[1]);
    double B = 2*eye[0]*d[0] + 2*eye[2]*d[2] - .5*eye[1]*d[1] + .25*d[1];
    double C = eye[0]*eye[0] + eye[2]*eye[2] - .25*eye[1]*eye[1] + .25*eye[1] - .25*.25;

    float t_sides = solveQuadratic(A, B, C);
    
    Point intersect = eye + d * t_sides;
    if (!(intersect[1] > -0.5 && intersect[1] < 0.5)) {
        t_sides = -1;
    }

    float t_cap = (-0.5 - eye[1]) / d[1];
    intersect = eye + d * t_cap;
    if (!(intersect[0]*intersect[0] + intersect[2]*intersect[2] <= 0.25)) {
        t_cap = -1;
    }
    
    return minPos(t_sides, t_cap);
}

float solveSphere(Point eye, Vector d) {
    double A = dot(d, d);
   // if (d[2] < -.999999) std::cout << "d " << d[0] << ", " << d[1] << ", " << d[2] << std::endl;
    double B = 2.0*(dot(Vector(eye[0], eye[1], eye[2]), d));
    double C = dot(Vector(eye[0], eye[1], eye[2]), 
                   Vector(eye[0], eye[1], eye[2])) - pow(0.5, 2);

    float q = solveQuadratic(A, B, C);
    return q;
}

float solveQuadratic(double A, double B, double C) {
    double det = B*B - 4.0*A*C;
        	//std::cout << "det is  " << det << std::endl;
    if (det > 0) {
        double t0 = (-1.0*B + sqrt(det)) / (2.0*A);
        double t1 = (-1.0*B - sqrt(det)) / (2.0*A);
        //std::cout << "t0 is  " << t0 << std::endl;
        return minPos(t0, t1);
    } else if (det == 0) {
        double t = (-B + sqrt(det)) / (2.0*A);

        return t;
    } else {
        return -1; 
    }   
}

void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

    SceneNode* root = parser->getRootNode();

    tree.clear();
    SceneTransformation *t = new SceneTransformation();
    flatten_tree(root, t);
    
    setupCamera();
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();

	Point guiEye;
	guiEye = Point(eyeX, eyeY, eyeZ);
	//Point guiEye = Point(eyeX, eyeY, eyeZ);
	Point guiLook;
	guiLook = Point(lookX, lookY, lookZ);
	//Vector guiLook = Vector(lookX, lookY, lookZ);
	Vector upV = camera->GetUpVector();
	camera->SetViewAngle(viewAngle);
	camera->Orient(guiEye, guiLook, upV);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/
void flatten_tree(SceneNode* root, SceneTransformation* parent_transforms) {

    std::vector<SceneNode*> children;
    children = root->children;
    
    Matrix transformations;
	transformations = parent_transforms->matrix;

	//std::cout << "# prims is " << root->primitives.size() << std::endl;


	for (int i = 0; i < root->transformations.size(); i++) {
		SceneTransformation *transform = root->transformations[i];

        switch (transform->type)
        {
            case TRANSFORMATION_TRANSLATE:
                transformations = transformations * trans_mat(transform->translate);
                break;
            case TRANSFORMATION_SCALE:
                transformations = transformations * scale_mat(transform->scale);
                break;
            case TRANSFORMATION_ROTATE:
                transformations= transformations * rot_mat(transform->rotate, transform->angle);
                break;
            case TRANSFORMATION_MATRIX: 
            	transformations = transformations * transform->matrix;
            	break;
        }
	}
	SceneNode *node = new SceneNode();
	SceneTransformation *t = new SceneTransformation();
	t->matrix = transformations;
	node->primitives = root->primitives;
	node->transformations.push_back(t); 
	tree.push_back(node);

	if (children.size()!=0) {
		for (int i = 0; i < children.size(); i++) {
			flatten_tree(children[i], t);
		}
	}
	
	return;

}

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	GLUI_Spinner* recursionWidget = new GLUI_Spinner(glui, "Recursion:", &recursion);
	recursionWidget->set_float_limits(0, 10); //fix me
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);

	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



