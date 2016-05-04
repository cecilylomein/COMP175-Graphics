/*  =================== File Information =================
	File Name: entity.cpp
	Description:
	Author: Michael Shah

	Purpose: Base class to be inherited from for 3D objects that can be transformed
	Examples:
	===================================================== */
#include "entity.h"

/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
entity::entity(){
	x = 0;
	y = 0;
	z = 0;
	sx = 1.0;
	sy = 1.0;
	sz = 1.0;
}
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
entity::~entity(){
	
}
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getXPosition(){ return x; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getYPosition(){ return y; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getZPosition(){ return z; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setPosition(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setXPosition(float _x){ x = _x; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setYPosition(float _y){ y = _y; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setZPosition(float _z){ z = _z; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getXScale(){ return sx; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getYScale(){ return sy; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
float entity::getZScale(){ return sz; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setScale(float _sx, float _sy, float _sz){
	sx = _sx;
	sy = _sy;
	sz = _sz;
}
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setXScale(float _sx){ sx = _sx; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setYScale(float _sy){ sy = _sy; }
/*	===============================================
	Desc: 
	Precondition:
	Postcondition:
	=============================================== */ 
void entity::setZScale(float _sz){ sz = _sz; }