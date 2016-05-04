/*   =================== File Information =================

    Last Modified: 1/27/13
    Description:
    Author:

    Purpose: Driver for UI interaction assignment
    Usage:
     
    Contributions: 
    Based on example5.cpp by Paul Rademacher (rademach@cs.unc.edu) on 7/10/98
    ===================================================== */


#include <string.h>
#include "ply.h"
#include <GL/glui.h>
#include <iostream>

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0;
float rotationY = 0.0;

/** These are the live variables passed into GLUI ***/
int   wireframe = 0;

int   light0_enabled = 1;
float light0_intensity = 1.0;

int   main_window;
float zoom = 1.0;

int   show_ply=1;
int   show_axes = 1;

float ply_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
float cam_pos[] = {0.0,0.0,-1.0};
int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui; 
GLUI *glui2;
GLUI_Spinner    *light0_spinner;
GLUI_RadioGroup *radio;
GLUI_Panel      *obj_panel;
GLUI_Scrollbar *sb_r, *sb_g, *sb_b;

/* ====================
PLY Model
======================*/
ply* myPLY;

/********** User IDs for callbacks ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT0_INTENSITY_ID  250
#define ENABLE_ID            300
#define DISABLE_ID           301
#define SHOW_ID              302
#define HIDE_ID              303
#define RANDOMIZE_LIGHTS     304


/********** Miscellaneous global variables **********/

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

/*

            Implement some additional functionality into this callback function,
            or create another callback function for your buttons.

*/

void control_cb( int control )
{
    if ( control == LIGHT0_ENABLED_ID ) {
        if ( light0_enabled ) {
            glEnable( GL_LIGHT0 );
            light0_spinner->enable();
        }
        else {
            glDisable( GL_LIGHT0 );
            light0_spinner->disable();
        }
    }
    else if ( control == LIGHT0_INTENSITY_ID ) {
        float v[] = {
            light0_diffuse[0],  light0_diffuse[1],
            light0_diffuse[2],  light0_diffuse[3] };
        
        v[0] *= light0_intensity;
        v[1] *= light0_intensity;
        v[2] *= light0_intensity;
        
        glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
    }
    else if ( control == ENABLE_ID )
    {
        glui2->enable();
    }
    else if ( control == DISABLE_ID )
    {
        glui2->disable();
    }
    else if ( control == SHOW_ID )
    {
        glui2->show();
    }
    else if ( control == HIDE_ID )
    {
        glui2->hide();
    }
    else if (control == RANDOMIZE_LIGHTS) {
        light0_diffuse[0] = 1.0*rand()/RAND_MAX;
        light0_diffuse[1] = 1.0*rand()/RAND_MAX;
        light0_diffuse[2] = 1.0*rand()/RAND_MAX;



        float v[] = {
            light0_diffuse[0],  light0_diffuse[1],
            light0_diffuse[2],  light0_diffuse[3] };
        
        
        std::cout << v[0] << std::endl;
        
        glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
        //control_cb(LIGHT0_INTENSITY_ID);
        //myGlutDisplay->draw();    
        sb_r->set_float_val(v[0]);
        sb_g->set_float_val(v[1]);
        sb_b->set_float_val(v[2]);
        glui->sync_live();


    }
}

/***************************************** myGlutMouse() **********/


/*

    Implement some Mouse functions here

    Example parameters for button
        button_state == GLUT_DOWN
    Example parameters for button_state
        button == GLUT_LEFT_BUTTON

*/
void myGlutMouse(int button, int button_state, int x, int y )
{

    if (button_state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {

            zoom += .1;
            glui->sync_live();
        } else if (button == GLUT_RIGHT_BUTTON) {
            zoom -= .1;
            glui->sync_live();

        }
    } 





}

/**************************************** myGlutKeyboard() **********/

/*

            Implement some Keyboard functions here

            Add in 'w','a','s','d' movement
            Add other keyboard buttons to do neat things

*/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 'w':
            cam_pos[1] -= .01;
            break;
        case 'a':
            cam_pos[0] += .01;
            break;
        case 's':
            cam_pos[1] += .01;
            break;
        case 'd':
            cam_pos[0] -= .01;
            break;

        case 27:
        case 'q':
            exit(0);
            break;
    };
    
    glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
    myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
    /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
    if ( glutGetWindow() != main_window )
        glutSetWindow(main_window);
    
    /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
     application  */
    
    glutPostRedisplay();
}

/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
    glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
    float xy_aspect;
    xy_aspect = (float)x / (float)y;

    // glViewport(0, 0, x, y);
    // ^ we have subviews so this causes the drawing area to be obscured by them:
    // instead:
    //    int tx, ty, tw, th;
    //    GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
    //    glViewport(tx, ty, tw, th);
    // which is equivalent to:
    GLUI_Master.auto_set_viewport();

    // Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
    // Otherwise we could modify the object transormations in our world with GL_MODELVIEW
    glMatrixMode(GL_PROJECTION);
    // Reset the Projection matrix to an identity matrix
    glLoadIdentity();
    // The call gluPerspective defines the perspective matrix and produces a perspective projection.
    gluPerspective(65.0, xy_aspect, .1, 15);
    // Since we are in projection mode, here we are setting the camera to the origin (0,0,0)
    glTranslatef(0, 0, 0);
    // Call our display function.
    glutPostRedisplay();
}


/************************************************** drawAxes() **********/
/* Disables lighting, then draws RGB axes                                */

void drawAxes( float scale )
{
    glDisable( GL_LIGHTING );
    
    glPushMatrix();
        glScalef( scale, scale, scale );
        
        glBegin( GL_LINES );
            glColor3f( 1.0, 0.0, 0.0 );
            glVertex3f( .8f, 0.05f, 0.0 );  glVertex3f( 1.0, 0.25f, 0.0 ); /* Letter X */
            glVertex3f( 0.8f, .25f, 0.0 );  glVertex3f( 1.0, 0.05f, 0.0 );
            glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 1.0, 0.0, 0.0 ); /* X axis      */
            
            glColor3f( 0.0, 1.0, 0.0 );
            glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 1.0, 0.0 ); /* Y axis      */
            
            glColor3f( 0.0, 0.0, 1.0 );
            glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 1.0 ); /* Z axis    */
        glEnd();
    glPopMatrix();

    // Draw a grid under the object
    float gridSize = 1.0f;
    float halfGridSize = gridSize * 0.5;
        for(float i = 0 ; i < gridSize; i=i+0.1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( -halfGridSize, 0.0, i-halfGridSize );  glVertex3f( halfGridSize, 0.0, i-halfGridSize ); /* X axis grid */
                glVertex3f( i-halfGridSize, 0.0, -halfGridSize );  glVertex3f( i-halfGridSize, 0.0, halfGridSize ); /* X axis grid */
            glEnd();
        }
    
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( -halfGridSize, 0.0, halfGridSize );  glVertex3f( halfGridSize, 0.0, halfGridSize ); /* X axis grid */
            glVertex3f( halfGridSize, 0.0, -halfGridSize );  glVertex3f( halfGridSize, 0.0, halfGridSize ); /* X axis grid */
        glEnd();
    glEnable( GL_LIGHTING );
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
    // Clear the buffer of colors in each bit plane.
    // bit plane - A set of bits that are on or off (Think of a black and white image)
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
    // Set the mode so we are modifying our objects.    
    glMatrixMode( GL_MODELVIEW );
    // Load the identify matrix which gives us a base for our object transformations
    // (i.e. this is the default state)      
    glLoadIdentity();
    
    // This is the part in the scene where our camera
    // stands, and we view the scene.
    // (Note the reverse order of the below)
    // (Remember how operations are processed in OpenGL!)
    // Step 2: Turn our camera to look around our scene
    glMultMatrixf( view_rotate );
    // Step 1: Push us back to a fixed position
    glTranslatef( cam_pos[0], cam_pos[1], cam_pos[2] );
    // Draw each of the axes if the live variable is set to 1, otherwise do not do so.
    if ( show_axes ){
        drawAxes(.52f);
    }

    /*  Now we render object, using some of the live variables.
        These are _live_ variables, which are transparently updated by GLUI 
        
        They are tied below directly to components

        These include:
        (1) obj_pos
        (2) zoom
        (3) ply_rotate
    */    
    glPushMatrix();
        glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );
        glScalef( zoom, zoom, zoom );
        glMultMatrixf( ply_rotate );

        // We wil either show a filled model or a 
        // filled model with the wireframe drawn over it.
        if(wireframe){
                glDisable( GL_LIGHTING );
                glColor3f(1.0f,1.0f,0.0f);
                // Render a wireframe view of our object
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                myPLY->render();
                // We must remember to enable lighting after we have disabled it.
                glEnable( GL_LIGHTING );
        }

        // If we are not drawing the wireframe of our model, otherwise just render
        // the object if the flag for showing the object is shown.
        if(show_ply){
            // Draw the filled model
            // Ensure lighting is enabled
            glEnable( GL_LIGHTING );
            glEnable(GL_POLYGON_OFFSET_FILL);
            glColor3f(0.6f,0.6f,0.6f);
            glPolygonMode(GL_FRONT, GL_FILL);
            // Call our render function
            myPLY->render();
        }
        
    glPopMatrix();

    
    // swap the back and front buffers to draw an updated image.
    glutSwapBuffers();
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    /****************************************/
    /*   Initialize GLUT and create window  */
    /****************************************/
    
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 800, 600 );
    
    main_window = glutCreateWindow( "User Interaction - InClass Assignment 2" );
    glutDisplayFunc( myGlutDisplay );
    GLUI_Master.set_glutReshapeFunc( myGlutReshape );
    GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
    GLUI_Master.set_glutSpecialFunc( NULL );
    GLUI_Master.set_glutMouseFunc( myGlutMouse );
    glutMotionFunc( myGlutMotion );
    
    /****************************************/
    /*       Set up OpenGL lights           */
    /****************************************/
    
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
    
    
    /****************************************/
    /*          Enable z-buferring          */
    /****************************************/
    
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1, 1);
    
    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/

    /*

            Implement functionality here

            Spend some time

    */
    
    /*** Create the side subwindow ***/
    glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
    
    obj_panel = new GLUI_Rollout(glui, "Properties", true );
    
    /***** Control for object params *****/
    
    new GLUI_Checkbox( obj_panel, "Wireframe", &wireframe, 1, control_cb );
    
    GLUI_Spinner *zoom_spinner =
    new GLUI_Spinner( obj_panel, "Zoom:", &zoom);
    zoom_spinner->set_float_limits( .2f, 4.0 );
    zoom_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    
    /******** Add some controls for lights ********/
    
    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Lights", false );
    
    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Light 0" );
    
    new GLUI_Checkbox( light0, "Enabled", &light0_enabled, LIGHT0_ENABLED_ID, control_cb );
    light0_spinner =
    new GLUI_Spinner( light0, "Intensity:",
                     &light0_intensity, LIGHT0_INTENSITY_ID,
                     control_cb );
    light0_spinner->set_float_limits( 0.0, 1.0 );

    sb_r = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL, &light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
    sb_r->set_float_limits(0,1);
    sb_g = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL, &light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
    sb_g->set_float_limits(0,1);
    sb_b = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL, &light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
    sb_b->set_float_limits(0,1);
        
    /*** Add another rollout ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Options", true );

    new GLUI_Checkbox( options, "Draw PLY", &show_ply );
    new GLUI_Checkbox( options, "Draw Axes and Grid", &show_axes );
        
    /*** Disable/Enable buttons ***/
    new GLUI_Button( glui, "Disable movement", DISABLE_ID, control_cb );
    new GLUI_Button( glui, "Enable movement", ENABLE_ID, control_cb );
    new GLUI_Button( glui, "Randomize lights", RANDOMIZE_LIGHTS, control_cb );
    
    new GLUI_StaticText( glui, "" );
    
    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );
    
    /**** Link windows to GLUI, and register idle callback ******/
    
    glui->set_main_gfx_window( main_window );
    
    /*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
    glui2->set_main_gfx_window( main_window );
    
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "View", view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );

    new GLUI_Column( glui2, false );
    GLUI_Rotation *tor_rot = new GLUI_Rotation(glui2, "PLY", ply_rotate );
    tor_rot->set_spin( .98 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy =  new GLUI_Translation(glui2, "Object XY", GLUI_TRANSLATION_XY, obj_pos );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Object X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Object Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Object Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .005 );
    
#if 0
    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif
    

    // Load our model
    myPLY = new ply("./bunny.ply");

    /**** Regular GLUT main loop ****/
    
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
