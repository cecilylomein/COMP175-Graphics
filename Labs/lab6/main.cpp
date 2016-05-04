/*  =================== File Information =================
	File Name: ppm.h
	Description:
	Author: Michael Shah
	Last Modified: 3/10/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	http://antongerdelan.net/opengl/raycasting.html
	===================================================== */
#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "object.h"

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 0;
int filled = 1;
int paint = 0;

int red_Scroll = 0;
int green_Scroll = 255;
int blue_Scroll = 0;

// Definition for rollout for r g b color sliders
#define COLORR_ID 101
#define COLORG_ID 102
#define COLORB_ID 103

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

// function declarations
void drawRayFunc(int x, int y);

// Flag for OpenGL interaction
bool drawRay = false;

// Window information
double windowXSize = 500;
double windowYSize = 500;

float camera_left = -1.0f;
float camera_right = 1.0f;
float camera_top = 1.0f;
float camera_bottom = -1.0f;
float camera_near = -1.0f;
float camera_far = 2.0f;

float mouseX = 0;
float mouseY = 0;
float mouseZ = 1.0f;
int mouseScreenX = 0;
int mouseScreenY = 0;

/* 	======== Our Scene ========
	We are going to create a single sphere that we load
	=========================== */
object* myObject = new object();
/***************************************** myGlutMotion() ***********/

// Updates the mouse cooridnates
// This function turns a x and y screen cooridnate into a world coordinate
// in object space
void updateMouse(int x, int y){
	mouseScreenX = x;
	mouseScreenY = y;

	mouseX = (2.0f * x)/windowXSize - 1.0f;
    mouseY = 1.0f - (2.0f * y)/ windowYSize;
    mouseZ = camera_near;
    //std::cout << "Screen (" << x << "," << y << ") to Object (" << mouseX << "," << mouseY << ")" << std::endl;
}

// This function is called everytime the mouse moves
// In order to get our ray to draw nicely, we update the mouse coordinates
// where the ray is first cast, and then draw the ray, and then draw the rest
// of our scene.
void myGlutMotion(int x, int y )
{
	updateMouse(x,y);
	drawRayFunc(mouseX,mouseY);
    glutPostRedisplay();
}

/***************************************** myGlutMouse() ***********/
/*	This function is called everytime the mouse is clicked

	left click - draw our ray
	right click - color the pixel appropriately.
	
*/
void myGlutMouse(int button, int button_state, int x, int y )
{
	updateMouse(x,y);

    // Cast a ray to the screen
    // Setup a flag so we can see the actual ray that is being cast.
    if(button_state == GLUT_UP && button==GLUT_LEFT_BUTTON){
        drawRay = false;
    }
    if((button_state == GLUT_DOWN && button==GLUT_LEFT_BUTTON) && drawRay==false){
        drawRay = true;
    }
}    


//paint object takes two floats, x and y, which go from 0 to 1. 
void paintObject(float x, float y) {
	myObject->paintTexture(x, y, red_Scroll, green_Scroll, blue_Scroll);
}

void paintObject(Point& isect) {
	myObject->paintTexture(isect, red_Scroll, green_Scroll, blue_Scroll);
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

/*
	When the window is resized, update aspect ratio to get correct viewing frustrum.
*/
void myGlutReshape(int x, int y)
{
	glViewport(0, 0, x, y);

	windowXSize = x;
	windowXSize = y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//using ortho projection for this assignment
	glOrtho(camera_left, camera_right, camera_bottom, camera_top, camera_near, camera_far);

	glutPostRedisplay();
}


/*
		You implement this!

		What it is returning is the depth at where we have intersected.

*/
double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {


	Matrix inverted = invert(transformMatrix);
	Point invEyePoint = inverted * eyePointP;
	Vector invRay = inverted * rayV;
	double a = invRay[0]*invRay[0] + invRay[1]*invRay[1] + invRay[2]*invRay[2];
	double b = 2.f * (invEyePoint[0]*invRay[0] + invEyePoint[1]*invRay[1] + invEyePoint[2]*invRay[2]);
	double c = (invEyePoint[0]*invEyePoint[0] + invEyePoint[1]*invEyePoint[1] + invEyePoint[2]*invEyePoint[2]) - 0.25;

	double p1 = ((-1.f*b) + sqrt((b*b - 4.f*a*c))/(2.f*a));
	double p2 = ((-1.f*b) - sqrt((b*b - 4.f*a*c))/(2.f*a));

        // Check discriminant
        if ((b*b) - (4.f*a*c) > 0) {
                if (p1 <= p2) {
                        return p1;
                } else {
                        return p2;
                }
        }


	return -1;
}

Vector generateRay() {
  Vector v = Vector(0,0,1.0);
  v.normalize();
  return v;
}

Point getEyePoint() {
  Point p = Point(mouseX, mouseY, -1.0);
  return p;
}

Point getIsectPointWorldCoord(Point eye, Vector ray, double t) {
	Point p = eye + (ray * t);
	return p; 
}

/*
	
	This function you will have to modify

*/
void drawRayFunc(int x, int y){
	if(drawRay==true){
		// Draw a bounding box around the sphere to help debug your intersection

		Point eyePointP = getEyePoint(); 
		Vector rayV = generateRay(); 

		float t = Intersect(eyePointP, rayV, Matrix());
		std::cout << "t: " << t << std::endl;

		Point isectPointWorldCoord = getIsectPointWorldCoord(eyePointP, rayV, t);

		if (t > 0) {
			glColor3f(1, 0, 0);
			glutWireCube(1.0f);
			if (paint == 1) {
				paintObject(isectPointWorldCoord);
			}
			else{
				glColor3f(red_Scroll / (float)255.0f, green_Scroll / (float)255.0f, blue_Scroll / (float)255.0f);
				glPushMatrix();
					glTranslatef(isectPointWorldCoord[0], isectPointWorldCoord[1], isectPointWorldCoord[2]);
					glutSolidSphere(0.05f, 10, 10);
				glPopMatrix();
			}
		}

		/*


				You can fill this in if it helps you debug

				This is the function you should call insersect in

				and then have some output whether we intersected or

				not with the object


		*/
	}
}

/***************************************** myGlutDisplay() *****************/
/*
	Renders the scene
*/	
void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glTranslatef( 0, 0, -4 );

	//glutWireCube(2.0);

	drawRayFunc(mouseX,mouseY);

	if (filled) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		myObject->render();
		glPushMatrix();
			glRotatef(90,0,1,0);
			myObject->drawTexturedSphere();
		glPopMatrix();
	}
	
	if (wireframe) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		myObject->render();
		glPushMatrix();
			glRotatef(90, 0, 1, 0);
			myObject->drawTexturedSphere();
		glPopMatrix();
	}

	glutSwapBuffers();
}

/*
		If you add any call backs for GLUI, you can put them here
*/
void control_cb( int control )
{

}


/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete myObject;
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{

	atexit(onExit);
	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowXSize, windowYSize);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 7");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);
	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/
	glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);
	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*		Setup textured Objects          */
	/****************************************/
	myObject->setTexture(0,"./data/pink.ppm");
	myObject->setTexture(1,"./data/smile.ppm");

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/
	GLUI *glui = GLUI_Master.create_glui("GLUI");

	GLUI_Scrollbar* sb1 = new GLUI_Scrollbar(glui, "Red", GLUI_SCROLL_HORIZONTAL, &red_Scroll, COLORR_ID, control_cb);
	sb1->set_int_limits(0, 255);
	GLUI_Scrollbar* sb2 = new GLUI_Scrollbar(glui, "Green", GLUI_SCROLL_HORIZONTAL, &green_Scroll, COLORG_ID, control_cb);
	sb2->set_int_limits(0, 255);
	GLUI_Scrollbar* sb3 = new GLUI_Scrollbar(glui, "Blue", GLUI_SCROLL_HORIZONTAL, &blue_Scroll, COLORB_ID, control_cb);
	sb3->set_int_limits(0, 255);
	sb1->set_int_val(255);
	sb2->set_int_val(255);

/*
    // Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our scene
    new GLUI_Column( glui, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );
*/
	glui->add_column(true);

	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Paint", &paint);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	glutMotionFunc( myGlutMotion );

	glutMainLoop();

	return EXIT_SUCCESS;
}



