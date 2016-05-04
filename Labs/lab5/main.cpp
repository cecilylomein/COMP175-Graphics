/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah
	Last Modified: 3/06/14

	Purpose: Driver for 3D program to demonstrate splines 
	Usage:	
	===================================================== */
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <GL/glui.h>
#include <math.h>
#include "spline.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int renderSpline = 1;
int renderControlPoints = 1;
int renderRollerCoaster = 1;
int grid = 1;
int followCoaster = 0;
// Note that the roller coaster will move along this 
// resolution
int resolution = 100;
int originalResolution = resolution;
int coasterCar1Position = 0;
int coasterCar2Position = 4;
int coasterCar3Position = 8;
int coasterCar4Position = 12;

// Length of our spline (i.e how many points do we randomly generate)
const int splineSize = 101;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

// Pointer to widget controls
GLUI_Button      *spline_btn;
// User ID for callbacks
#define SPLINE_ID	100
#define RESOLUTION_ID 101

// Pointer to widget
GLUI *glui2;
GLUI_Spinner    *resolution_spinner;


/* 	======== Our Scene ========

	============================== */
spline* mySpline = new spline(resolution);
// Add a bunch of points to our spline

// Neat function 
// author/source: http://stackoverflow.com/questions/686353/c-random-float-number-generation
float rand_FloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

/*
		This function is responsible for populating our 'mySpline' object which is of type 'spline'
		It randomly generates points that are added to the splines linked list by calling the
		member function 'addPoint'
*/
void populatePoints(){
	// Randomly generated pions
	float stepSize = 1.0f; // x coordinate (how spread out are the points along x-axis)
	float maxYHeight = 10.0f;	// y cooridnate (the up and down of our roller coaster)
	float maxZWidth = 10.0f;	// z cooridnate (how spread out is the roller coaster)

	for(int i = 0; i < splineSize; i++){
		float randomHeight = rand_FloatRange(0,maxYHeight);
		float randomWidth = rand_FloatRange(0,maxZWidth);
		mySpline->addPoint(i*stepSize,randomHeight,randomWidth);
	}

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
	We configure our window so that it correctly displays our objects
	in the correct perspective.
*/
void myGlutReshape(int x, int y)
{
	float xy_aspect;
	xy_aspect = (float)x / (float)y;

	glViewport(0, 0, x, y);
	// Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
	// Otherwise we could modify the object transormations in our world with GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	// Reset the Projection matrix to an identity matrix
	glLoadIdentity();
	// The frustrum defines the perspective matrix and produces a perspective projection.
	// It works by multiplying the current matrix(in this case, our matrix is the GL_PROJECTION)
	// and multiplies it.
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	// Since we are in projection mode, here we are setting the camera to the origin (0,0,0)
	glTranslatef(0, 0, 0);
	// Call our display function.
	glutPostRedisplay();
}

/*
	Function to draw the x,y,and z axis
*/
void drawAxis(){
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
}

/*
	Function to a renderSpline grid in our scene.
*/
void draw_grid(){
	int grid_size = 100;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-grid_size/2,0,-grid_size/2);
    // Draw a grid under the object
        for(int i = 0 ; i < grid_size; i=i+1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( 0.0, 0.0, i );  glVertex3f( grid_size, 0.0, i ); /* X axis grid */
                glVertex3f( i, 0.0, 0.0 );  glVertex3f( i, 0.0, grid_size ); /* X axis grid */
            glEnd();
        }
    
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );

}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	static float scale = 0.1;
	// Clear the buffer of colors in each bit plane.
    // bit plane - A set of bits that are on or off (Think of a black and white image)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the mode so we are modifying our objects. 
	glMatrixMode(GL_MODELVIEW);
	
	// Load the identify matrix which gives us a base for our object transformations
    // (i.e. this is the default state) 
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( obj_pos[0], obj_pos[1]-2, -obj_pos[2]-6 );
	glMultMatrixf(view_rotate);
	
	// Set our camera to follow one of the roller coaster cars
	if(followCoaster==1){
		glRotatef(90,0,1,0);
		glRotatef(30,0,0,1);
		glTranslatef(splineSize*0.5-mySpline->getComputedX(coasterCar1Position),-mySpline->getComputedY(coasterCar1Position),-mySpline->getComputedZ(coasterCar1Position));

		/* You Implement this code: 
			The above is just an example. You can make the "follow coaster" to be in the cart and taking a ride. Since you
			have not implemented your Camera yet. You can use the function gluLookAt, which looks like:
			void gluLookAt(	GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
 							GLdouble lookatPointX, GLdouble lookatPointY, GLdouble lookatPointZ,
							GLdouble upX, GLdouble upY, GLdouble upZ);
		*/

	}

	glPushMatrix();

	// Here we are centering our roller coaster by centering it .
	// (splineSize(101 by default and declared at the top of main.cpp))
	// 
	glTranslatef(-splineSize*0.5,0,0);
	// Render our scene
		if (renderControlPoints) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mySpline->render();
		}
		
		// Render just the single spline
		if (renderSpline) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			mySpline->draw_spline(resolution,0);
		}

		// Render two splines side by side
		// An easy way to implement this is to call
		// 'draw_spline' twice and translate it along the z axis.
		if (renderRollerCoaster) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//mySpline->render();
			mySpline->renderAsRollerCoaster(resolution,0.5);
		}

		// Render a series of 4 cubes in a row to look
		// like a roller coaster
		// Render a very simple roller coaster
		glPushMatrix();
			glTranslatef(mySpline->getComputedX(coasterCar1Position),mySpline->getComputedY(coasterCar1Position),mySpline->getComputedZ(coasterCar1Position));
			glutWireCube(0.5);
		glPopMatrix();
		// Render a very simple roller coaster
		glPushMatrix();
			glTranslatef(mySpline->getComputedX(coasterCar2Position),mySpline->getComputedY(coasterCar2Position),mySpline->getComputedZ(coasterCar2Position));
			glutWireCube(0.5);
		glPopMatrix();
		// Render a very simple roller coaster
		glPushMatrix();
			glTranslatef(mySpline->getComputedX(coasterCar3Position),mySpline->getComputedY(coasterCar3Position),mySpline->getComputedZ(coasterCar3Position));
			glutWireCube(0.5);
		glPopMatrix();
		// Render a very simple roller coaster
		glPushMatrix();
			glTranslatef(mySpline->getComputedX(coasterCar4Position),mySpline->getComputedY(coasterCar4Position),mySpline->getComputedZ(coasterCar4Position));
			glutWireCube(0.5);
		glPopMatrix();

		// These variable are responsible for moving each of the 
		// four cubes of the roller coaster along the spline.
		coasterCar1Position++;
		coasterCar2Position++;
		coasterCar3Position++;
		coasterCar4Position++;

		// If we go beyond our splines resolution, then we need
		// to set them back to zero and restart our simulation.
		// If we do not do this, we will get a segmentation fault
		// by going outside the bounds of our computered_x_y_z array(which exists in the
		// class spline, and is used in the functions 'getComputedX','getComputedY','getComputedZ')
		if(coasterCar1Position>=originalResolution*splineSize){
			coasterCar1Position=0;
		}
		if(coasterCar2Position>=originalResolution*splineSize){
			coasterCar2Position=0;
		}
		if(coasterCar3Position>=originalResolution*splineSize){
			coasterCar3Position=0;
		}
		if(coasterCar4Position>=originalResolution*splineSize){
			coasterCar4Position=0;
		}


	glPopMatrix();
	// Draw a grid
		if(grid){
			draw_grid();
		}


	glutSwapBuffers();
}

// Callbacks for the scene
void pointer_cb( GLUI_Control* control )
{
  // Draw the spline once more, but output debugging information
  if (control->get_id() == SPLINE_ID ) {
  	std::cout << "Generating Spline" << std::endl;
  	mySpline->draw_spline(resolution,1);
  }
  // Reallocate memory for our spline based on the resolution
  // that we specify
  if(control->get_id()==RESOLUTION_ID){
  	mySpline->update(resolution);
  }
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete mySpline;
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
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 3");
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutSpecialFunc(NULL);

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
	/*         Here's the GLUI code         */
	/****************************************/

	//GLUI *glui = GLUI_Master.create_glui("GLUI");

	/*** Create the bottom subwindow ***/

	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
	glui2->set_main_gfx_window(main_window);
	// Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our scene
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );

    new GLUI_Checkbox(glui2, "Follow Coaster", &followCoaster);

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Render Spline", &renderSpline);
	new GLUI_Checkbox(render_panel, "Render ControlPoints", &renderControlPoints);
	new GLUI_Checkbox(render_panel, "Render Roller Coaster", &renderRollerCoaster);
	new GLUI_Checkbox(render_panel, "Grid", &grid);

	resolution_spinner =
    new GLUI_Spinner( render_panel, "Resolution:",
                     &resolution, RESOLUTION_ID,
                     pointer_cb );
    resolution_spinner->set_int_limits( 50, 500 );

	new GLUI_Button(render_panel, "CalculateSpline", SPLINE_ID, pointer_cb);
	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);

	//glui->set_main_gfx_window(main_window);
	// We register the idle callback with GLUI, *not* with GLUT
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	// Setup the points for our spline
	populatePoints();

	glutMainLoop();


	return EXIT_SUCCESS;
}



