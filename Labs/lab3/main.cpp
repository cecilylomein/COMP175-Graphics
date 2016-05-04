/****************************************************************************

Last Modified: 12/23/13

****************************************************************************/

#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "solarSystem.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 1;
int filled = 1;
int grid = 0;
int orbits = 1;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 1.0, 1.0, 2.0 };

// Pointer to widget
GLUI *glui2;


/* 	======== Solar System ========

	============================== */
solarSystem* mySolarSystem = new solarSystem();


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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	glTranslatef(0, 0, 0);

	glutPostRedisplay();
}



void draw_circle(float radius){
	float x,y;

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	    glColor4f(1.0f,1.0f,1.0f,0.5f);
	     
	    x = (float)radius * cos(359 * PI/180.0f);
	    y = (float)radius * sin(359 * PI/180.0f);
	    for(int j = 0; j < 360; j++)
	    {
	        glVertex3f(x,0,y);
	        x = (float)radius * cos(j * PI/180.0f);
	        y = (float)radius * sin(j * PI/180.0f);
	        glVertex3f(x,0,y);
	    }
	glEnd();
	glEnable(GL_LIGHTING);
}

void draw_oribits(){
	draw_circle(2.0f);
	draw_circle(3.0f);
	draw_circle(4.0f);
	draw_circle(5.0f);
	draw_circle(7.0f);
	draw_circle(9.0f);
}

void draw_grid(){
	int grid_size = 10;
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( obj_pos[0], obj_pos[1]-2, -obj_pos[2]-6 );
	glMultMatrixf(view_rotate);

		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mySolarSystem->render();
		}
		
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPopMatrix();
			mySolarSystem->render();
		}

		if(grid){
			draw_grid();
		}

		if(orbits){
			draw_oribits();
		}

	glutSwapBuffers();
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete mySolarSystem;
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
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutSpecialFunc(NULL);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	/* Old Lighting System */
	/*glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	//static float one[] = { 1, 1, 1, 1 };
	GLfloat one[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);*/

	/* New Lighting System */
	GLfloat one[] = { 0.1f, 0.1f, 0.1f, 0.1f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);
	GLfloat mat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
	GLfloat mat_shininess[] = { 0.5 };
	GLfloat mat_diffuse[] = {0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_ambient[] = {0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse); 
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);


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

    // Navigate our solar system
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);
	new GLUI_Checkbox(render_panel, "Orbits", &orbits);

	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);



	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);



	glutMainLoop();

	return EXIT_SUCCESS;
}



