/*  =================== File Information =================
  File Name: ply.cpp
  Description:
  Author: Michael Shah

  Last Modified: 1/28/14

  Purpose:
  Examples:
  ===================================================== */

#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include <time.h>
#include "ply.h"	// Our ply loader
#include "movieCamera.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 0;
int filled = 1;
int grid = 0;

// The camera position
float view_pos[] = { 0.0, 0.0, 0.0 };

// Pointer to widget of controls we're using
GLUI *glui2;
GLUI_Button      *action_btn;
GLUI_Button      *cut_btn;

/* ================= User ID for Callbacks =================
   ========================================================= */
#define ACTION_ID      	100
#define CUT_ID      	101


/* 	================= Movie variables =================

	=================================================== */
bool startTheScene = false;
float carSpeed = 0;
float rexSpeed = 0;	
float jump = 0;

/* ================= Movie Objects =================
	Our handy ply function allows us to load more interesting
	scenes.
   ================================================= */
ply* 	trex = new ply("./data/trex.ply");	
ply* 	jeep = new ply("./data/jeep.ply");
ply* 	cam = new ply("./data/camera.ply");

ply* 	log_ = new ply("./data/log.ply");	
ply* 	gate = new ply("./data/gate.ply");

/*  ================= Design Note =================
	Thought exercise, do I need to load 6 models if they are all the same?
	Or can I load one tree and then render it 6 times?
	What if I want to modify individual properties of each tree?
	=============================================== */
ply* 	tree = new ply("./data/tree.ply");	
ply* 	grass = new ply("./data/grass.ply");

/* 	================= Camera's =================
	We will have one camera in this scene
	You are free to add more if you are feeling creative

	We will be calling different functions in this camera
	to run our scene however.
	============================================ */
movieCamera* camera1 = new movieCamera();

/* 	================= Animation =================
	Timer for the animation
	============================================= */
time_t start; 


/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
void startScene(){

		glEnable(GL_LIGHTING);
		if(startTheScene==true){
			carSpeed+=0.01;
			rexSpeed+=0.02;
			jump += 0.01;
		}

		if(wireframe==1){
			glDisable(GL_LIGHTING);
		}

		cam->render();

		// Tree 1
		glPushMatrix();
			glColor3f(0.6f,0.9f,0.1f);
			glTranslatef(-3,5,-9);
			glScalef(10,10,10);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();
		// Tree 2
		glPushMatrix();
			glColor3f(0.6f,0.7f,0.1f);
			glTranslatef(0,4.5,-8);
			glScalef(9,9,9);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();
		// Tree 3
		glPushMatrix();
			glColor3f(0.6f,0.8f,0.1f);
			glTranslatef(3,5,-9);
			glScalef(10,10,10);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();
		// Tree 4
		glPushMatrix();
			glColor3f(0.2f,0.6f,0.1f);
			glTranslatef(4,5,-8);
			glScalef(10,10,10);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();
		// Tree 5
		glPushMatrix();
			glColor3f(0.2f,0.9f,0.1f);
			glTranslatef(6,5,-9);
			glScalef(10,10,10);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();
		// Tree 6
		glPushMatrix();
			glColor3f(0.6f,0.9f,0.1f);
			glTranslatef(8,5,-8);
			glScalef(10,10,10);
			glRotatef(-90,1,0,0);
			tree->render();
		glPopMatrix();

		// Grass
		glPushMatrix();
			glColor3f(0.0f,0.2f,0.0f);
			glTranslatef(8,0,-8);
			glScalef(30,5,30);
			glRotatef(-90,1,0,0);
			grass->render();
		glPopMatrix();

		// Fallen log
		glPushMatrix();
			glRotatef(45,0,1,0); // Second rotate log 45 degrees to put it at an angle
				glPushMatrix();
					glColor3f(0.15f,0.1f,0.1f);
					glTranslatef(8,0.5,-2);
					glScalef(5,5,5);
					glRotatef(-90,1,0,0);	// First rotate log 90 degrees so it is flat
					log_->render();
				glPopMatrix();
		glPopMatrix();
		// Gate
		glPushMatrix();
			glColor3f(0.1f,0.1f,0.1f);
			glTranslatef(-6,2,-5);
			glScalef(5,5,5);
			glRotatef(90,0,1,0);
			gate->render();
		glPopMatrix();

		// T-rex
		glPushMatrix();	
			float trex_hopping = 1.0;
			trex->setPosition(-5+rexSpeed,(sin(jump)*trex_hopping)+1.25,-6);	// Set position variables (useful for later)	
			if(wireframe==1){ glColor3f(0.0,0.0,0.0); trex->renderWireFrame(); }
			if(filled==1)	{ glColor3f(0.25f,0.2f,0.1f); trex->render(); }
		glPopMatrix();
		
		// Jeep
		glPushMatrix();
			jeep->setPosition(5+carSpeed,1,-6);
			if(wireframe==1){ glColor3f(0.0,0.0,0.0); jeep->renderWireFrame(); }
			if(filled==1)	{ glColor3f(0.15f,0.6f,0.1f); jeep->render(); }			
		glPopMatrix();		
}

/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
void resetScene(){
	std::cout << "cut, cut, CUT!" << std::endl;
	double seconds_since_start = difftime(time(NULL),start);
	std::cout << "seconds since start: " << seconds_since_start << std::endl;

	view_pos[0] = 0;
	view_pos[1] = 1;
	view_pos[2] = 2;

	carSpeed = 0;
	rexSpeed = 0;	
	jump = 0;
}

/*  ===============================================
      Desc: Callbacks for the scene
      Precondition:
      Postcondition:
    =============================================== */ 
void pointer_cb( GLUI_Control* control )
{
  if (control->get_id() == ACTION_ID ) {
  	std::cout << "And Action!" << std::endl;
  	startTheScene=true;
  	time(&start);
  }
  else if(control->get_id() == CUT_ID){
  	startTheScene=false;
  	resetScene();
  }
}


/***************************************** myGlutIdle() ***********/
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
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
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 25.0);

	glutPostRedisplay();
}

/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
void draw_grid(){
	int grid_size = 20;
	glDisable(GL_LIGHTING);
	glPushMatrix();
		// Center the scene
		glTranslatef(-grid_size/2,0,-grid_size/2);
	    // Draw a grid under the scene
	        for(float i = 0 ; i < grid_size; i+=.2){
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
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
    =============================================== */ 
void myGlutDisplay(void)
{
	static float scale = 0.1;
	// Set our background color that the screen refreshes too
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	// Which bit planes get cleared
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set up
	glMatrixMode(GL_MODELVIEW);
	// Give us a default matrix to start our scene from
	glLoadIdentity();

	if(startTheScene==false){
		glTranslatef( view_pos[0], view_pos[1]-2, -view_pos[2]-6 );
	}
	/* Perform camera transformations 
	   Each camera funtion will be called after
	   a certain amount of time has elapsed.
	
		You are free to implement your own camera functions!
		*** Be Creative! ***
	
	*/
	if(startTheScene==true){
		// Output the time to the console, just a a director would have time
		// running on the camera.
		cout << difftime(time(NULL),start) << "s : ";
		if(difftime(time(NULL),start) <= 2){
			std::cout << "orthogonal" << endl;
			camera1->orthogonal(-1,1,-1,1,1,10);	// Get a unit perspective from above
		}
		else if(difftime(time(NULL),start) > 2 && difftime(time(NULL),start) <= 6){
			std::cout << "Close Up" << endl;
			// Close up shot on the T-Rex
			camera1->closeUp(trex->getXPosition(), trex->getYPosition(),trex->getZPosition(),
							1,10);	
		}
		else if(difftime(time(NULL),start) > 6 && difftime(time(NULL),start) <= 10){
			std::cout << "follow shot" << endl;
			//camera1->perspective(105,.75,1,10);	// Get the regular perspective
			camera1->follow(trex->getXPosition(), trex->getYPosition()-2,trex->getZPosition(),
				0, 0, 0,
				0, 0, 0); // attach the camera to the t-rex
		}
		else if(difftime(time(NULL),start) > 10 && difftime(time(NULL),start) <= 14){
			std::cout << "spin around" << endl;
			// Spin around a point in space.
			// Radius is fixed, but could be adjusted.
			// Y height may also need to be adjusted.
			camera1->spinAroundPoint(jeep->getXPosition(),jeep->getYPosition()-2,jeep->getZPosition(),2);
		}
		else{
			startTheScene=false;
			resetScene();
		}
	}

	if (filled) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(0.5, 0.5, 0.5);
		glPolygonMode(GL_FRONT, GL_FILL);
		startScene();
	}
	// Set our scene to a wireframe mode.
	if (wireframe) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		startScene();
	}

	if(grid){
		draw_grid();
	}


	glutSwapBuffers();
}



/*	================= Call destructors =================
	Reclaim memory we've allocated

	Design Note: (Going further)
	You could imagine as we load lots of objects, we might
	accidently forget to delete one!  What if we could instead
	have some sort of object manager? Ah, perhaps on your own time
	you could build on top of the entity class, and have an
	object manager that would instead call the destructor
	of every object we allocate through it.  We'll learn of at least
	one data structure that can be used to organize a scene, and thus
	solve this problem.
	==================================================== */
void onExit()
{
	delete trex;
	delete jeep;
	delete cam;

	delete log_;
	delete gate;

	delete tree;

	delete grass;

	delete camera1;
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
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(600, 600);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 6");
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/
	/* OLD LIGHTING SYSTEM
	// Essentially set the background color of the 3D scene.
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glShadeModel(GL_FLAT);

	GLfloat light_pos0[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 0.0f };
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	*/


	/* New Lighting System */
	glClearColor(0.1, 0.1, 0.1, 1.0);
	GLfloat one[] = { 0.1f, 0.1f, 0.1f, 0.1f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);
	GLfloat mat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
	GLfloat mat_shininess[] = { 0.5 };
	GLfloat mat_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_position[] = { 0.5, 0.5, 0.5, 0.0 };
	GLfloat light1_position[] = { 0.0, 0.6, -1.0, 0.0 };
	GLfloat light2_position[] = { 1.0, 0.6, 0.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, mat_ambient); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse); 
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, mat_ambient); 
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse); 
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*           Setup 3D Objects           */
	/****************************************/

	// Make the t-rex big!
	trex->setScale(10,10,10); 
	// Increase the size of the t-rex as well
	jeep->setScale(3,3,3);	  

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	//GLUI *glui = GLUI_Master.create_glui("GLUI");


	/*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );

    // Navigate our movie scene
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "View X", GLUI_TRANSLATION_X, view_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "View Y", GLUI_TRANSLATION_Y, &view_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "View Z", GLUI_TRANSLATION_Z, &view_pos[2] );
    trans_z->set_speed( .1 );

    // Adds another column so the next panel will contain all of these elements.
	glui2->add_column(true);
	// Create a panel that will have 3 checkbox components
	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);

	// Seperate out another column
	glui2->add_column(true);
	// Add a new panel, and then 3 buttons to it.
	GLUI_Panel *action_panel = glui2->add_panel("Ready And...");
	new GLUI_Button(action_panel, "Action", ACTION_ID, pointer_cb);
	new GLUI_Button(action_panel, "cut cut Cut!", CUT_ID, pointer_cb);

	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);
	glui2->set_main_gfx_window(main_window);
	//glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



