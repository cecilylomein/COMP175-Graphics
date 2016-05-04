#include <GL/glui.h>
#include "solarSystem.h"

solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){
	
	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitX = 0.15;
	static float orbitSpeed = 0.2;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;

	// The Sun
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glColor3f(0.96f,0.85f,0.26f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();
	// Add more planets, moons, and rings here!	
		

	rotation+= 0.01;
	orbitX+=orbitSpeed;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}