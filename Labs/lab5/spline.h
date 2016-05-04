/*  =================== File Information =================
	File Name: spline.cpp
	Description:
	Author: Michael Shah
	Last Modified: 3/06/14

	Purpose: Create a cubic spline 
	Usage:	

	Further Reading resources: 
	http://mathworld.wolfram.com/CubicSpline.html
	http://www.geos.ed.ac.uk/~yliu23/docs/lect_spline.pdf
	===================================================== */
#ifndef SPLINE_H
#define SPLINE_H

#include "controlPoint.h"


/*
	
*/
class spline{
	public:
		/*	===============================================
		Desc:	Default constructor for spline
		Precondition: 	_resolution specifies how many points will be drawn between each control
						point for the spline.
		Postcondition:
		=============================================== */ 
		spline(int _resolution);
		/*	===============================================
		Desc:			Deallocate memory
		Precondition:
		Postcondition:
		=============================================== */ 
		~spline();
		/*	===============================================
		Desc:	Reallocates memory for our resolution table.
				Note that as many possible resolution points are copied
				over to the new array.
		Precondition:
		Postcondition:
		=============================================== */
		void update(int _resolution);
		/*	===============================================
		Desc:			Adds a point to the linked list of control points
		Precondition:	If it is the first point, make sure it is the head nad tail.
						If there is no memory available, terminate the program.
		Postcondition:	
		=============================================== */ 
		void addPoint(float x, float y, float z);
		/*	===============================================
		Name: draw_spline
		Desc: 	Called once to calculate a table of values once we have
				added all of our points.
		Precondition: 	resolution(in my implementation), specifies how many points to draw between
						each control point.

						output is used as a flag to output the points along the spline to the console.
						A value of 1 will output each interpolated point, while a value of 0 will output
						nothing to standard IO.
						You may remove this is parameter if you think it is unnecessary.
		Postcondition:
		=============================================== */ 
		void draw_spline(int resolution, int output);
		
		/*	===============================================
		Name: calculate_Spline
		Desc: 	Function for returning an interpolated point between and sPos(starting position) and
				an gPos (goal/ending) position.
				The velocity at the starting point (Vs) and goal point(Vg) can be user specified.
		
				S = starting position of your interval
				G = ending(or "goal") position of your interval
				Vs = velocity your points grow from from your starting point
				Vg = velocity your point ends on from your goal position

		Precondition:
		Postcondition:
		=============================================== */ 
		float calculate_Spline (float t, float S, float G, float Vs, float Vg);
		
		/*	===============================================
		Name: 	render
		Desc:	This function is responsible for drawing the original control points
				Use this as an example of how to iterate through the data structure.

				This function renders the control points and draws straight lines between them.
				You can use this function to compare with the draw_spline function.
		Precondition:
		Postcondition:
		=============================================== */ 
		void render();
		/*	===============================================
		Name: 	renderAsRollerCoaster
		Desc:	renderAsRollerCoaster is a hacked together function.
				For this you get to be creative with how you could render two or more splines
				together in order to create cool shapes. 
				Perhaps you could draw a tunnel to go through?
				Perhaps you create an overhead roller coaster?
				Perhaps you have other ideas?
		Precondition:
		Postcondition:
		=============================================== */ 
		void renderAsRollerCoaster(int resolution, float width);
		
		/*	===============================================
		Desc: 	Returns a value from the computed_x_y_z table
			  	
			  	These are all helper functions if you decide to use a table
				They can be helpful for moving a roller coaster object along a path
				(Note: These functions can probably be improved for safety and do
				bounds checking)
		Precondition:	Memory has been allocated for computed_x_y_z
		Postcondition:
		=============================================== */
		float getComputedX(int point){
			if(computed_x_y_z==NULL){return 0;}
			return computed_x_y_z[point*3];
		}
		float getComputedY(int point){
			if(computed_x_y_z==NULL){return 0;}
			return computed_x_y_z[point*3+1];
		}
		float getComputedZ(int point){
			if(computed_x_y_z==NULL){return 0;}
			return computed_x_y_z[point*3+2];
		}

	private:
		// The number of points drawn between two control points
		int resolution;
		// Number of control points
		int points;

		// Linked list of points
		controlPoint* head;
		controlPoint* tail;

		/* You could consider adding a table that computes the points once and then stores them
		   What data structure would you use?
		   Dynamic array would likely work fine and still be fast, but remember to resize if you
		   add points!
		  
		   The amount of memory you would allocated would be equal to resolution*points*3
		   The number 3 comes from x,y,z where 
		   0=x 
		   1=y 
		   2=z 
		   3= your second x 
		   4= your second y 
		   5= your second z etc...
		*/
		float* computed_x_y_z;
};

#endif