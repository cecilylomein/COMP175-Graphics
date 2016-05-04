/*  =================== File Information =================
  File Name: movieCamera.h
  Description:
  Author: (You)

  Purpose:
  Examples:



Some notes on Camera:

Use glMatrixMode(GL_MODELVIEW)  to setup your view matrix
	This could include calls to glTranlsate and glRotate and glTranslate() or otherwise
	using your own matrix library to load a 4x4 matrix.

Use glMatrixMode(GL_PROJECTION) to modify your projection matrix.
	Relevant calls are glFrustum() which sets up the viewing volume. 

  ===================================================== */
#ifndef MOVIE_CAMERA_H
#define MOVIE_CAMERA_H

#include <GL/glui.h>

class movieCamera{

	public:
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		movieCamera();
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		~movieCamera();
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		void closeUp(float your_x, float your_y, float your_z, float near, float far);
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		void wideAngle(float fov);
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		void orthogonal(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal);
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear,GLdouble zFar);
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		/* Here we are basically implementing the gluLookAt utility function 
		*/
		void follow(float your_x, float your_y, float your_z,
					float target_x, float target_y, float target_z,
					float up_x, float up_y, float up_z);
	/*  ===============================================
	      Desc: 
	      Precondition:
	      Postcondition:
    	=============================================== */ 
		void spinAroundPoint(float your_x, float your_y, float your_z, float distance);


	private:
		// Some private member variables to save state of camera
		float cam_near,cam_far,fov;

		// Used in the SpinAroundPoint function
		// Allows us to 'save' the value from our function call
		// so we can spint around the object with each call at an incremented
		// position
		static float rotate_around;

};

#endif