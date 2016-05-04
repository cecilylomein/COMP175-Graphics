/*  =================== File Information =================
	File Name: entity.h
	Description:
	Author: Michael Shah

	Purpose: Base class to be inherited from for 3D objects that can be transformed
	Examples:

	To do:
	- This likely needs to be inherited as a virtual class so multiple instances
	  are not created. (Example ply inherits from entity, but some other object ('monster') could inherit
	  					from ply, and if it has already inherited from entity this causes a clash!
	  					I wanted to just make this note so you know this lab was coded for simplicity,
	  					but not optimized. 
	===================================================== */
#ifndef ENTITY_H
#define ENTITY_H

/*  ============== entity ==============
	Purpose: entity serves to be a base class that can be inherited from

	Example usage: 

	1.) 
	==================================== */ 
class entity{

public:
	/*	===============================================
		Desc: Default constructor for an entity object
		Precondition:
		Postcondition:
		=============================================== */ 
	entity();
		/*	===============================================
		Desc: Default constructor for an entity object
		Precondition:
		Postcondition:
		=============================================== */ 
	~entity();
	/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	float getXPosition();
		/*	===============================================
		Desc: Default constructor for an entity object
		Precondition:
		Postcondition:
		=============================================== */ 
	float getYPosition();
		/*	===============================================
		Desc: Default constructor for an entity object
		Precondition:
		Postcondition:
		=============================================== */ 
	float getZPosition();
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setPosition(float x, float y, float z);
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setXPosition(float _x);
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setYPosition(float _y);
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setZPosition(float _z);

	/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	float getXScale();
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	float getYScale();
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	float getZScale();
	/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setScale(float _sx, float _sy, float _sz);
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
	void setXScale(float _sx);
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
	void setYScale(float _sy);
		/*	===============================================
		Desc: 
		Precondition:
		Postcondition:
		=============================================== */ 
	void setZScale(float _sz);
	

private:
		// translate values
		float x,y,z;
		// scale values
		float sx,sy,sz;
};

#endif