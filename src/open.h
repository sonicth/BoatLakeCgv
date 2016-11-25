/*
Length and Width, as described in the practical
will be defined in the beginning
strip count along the lenght
and strip count along the width of landscape
*/
#include <stdio.h>
#include <stdlib.h>					/* For "exit" prototype */

#ifdef __APPLE__		//this is dodgy! TODO: test for Frameworks instead
#include <OpenGL/gl.h>		// Header File For The OpenGL32 Library
#include <OpenGL/glu.h>		// Header File For The GLu32 Library
#include <GLUT/glut.h>		// Header File For The GLut Library

#else
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>				/* Header File For The GLUT Library */

#endif

#include <time.h>
#include <math.h>



#define	L_LENGTH		120.0		//120.0
#define	L_WIDTH			90.0	//90.0
//more correct would be calling it strip count
#define S_LENGTH		80
#define S_WIDTH			60
//menu view options
#define VIEW_SIDE		1
#define VIEW_PLAN		2
#define VIEW_BOAT_FRONT	3
#define VIEW_BOAT VIEW_BOAT_FRONT
#define	VIEW_BOAT_BACK	4
#define VIEW_MOBILE		5
//shapes
#define SHADE_WIRE		100
#define SHADE_FLAT		101
#define SHADE_SMOOTH	102

//lake dimensions
#define LAKE_LENGTH		80.0
#define LAKE_WIDTH		60.0
//RUNNING MODE
#define WORLD_EDIT		6
#define WORLD_DEMO		7
//fractal elevaton algorithm method only perlin will be used though
#define PERLIN_TYPE		1
#define MID_SUBDIVISION_TYPE	2
#define OTHER_TYPE			3
//keyboard key defines
#define ESCAPE			27
//mouse sensitivity
#define MOUSE_RESP		0.04
//fractal/perlin frequency amplitude
#define AMPLITUDE		10
// DODGY MOUNTAINS!!!
#define MOUNTS			30
#define INTERPOLATE		cosInterpolate
//PERSISTENCE?
#define PERSISTENCE		3
#define	NUM_OF_OCTAVES	3
//Pi is required
#ifndef PI
#define PI			3.141593
#endif
//BOAT DIMENSIONS!
#define BOAT_SIZE_LENGTH	10
#define BOAT_SPEED			0.03f
#define BOAT_ROTATE_SPEED	0.2f
#define BOAT_OFFSET			5.0f
#define PARTICLE_SPEED		0.2f
//PARTICLES
#define	MAX_PARTICLES		50
//CAMERA ZOOM for mobile view
#define CAMERA_MAX_DISTANCE	200
#define CAMERA_MIN_DISTANCE	5
//rotation of view of the camera
#define	KEY_VEW_UP			'i'
#define KEY_VEW_DOWN		'k'
#define KEY_VEW_RIGHT		'l'
#define KEY_VEW_LEFT		'j'
/*	bezier edit
	experimental!

	keys for seletcing set in 
		z plane, along the boat
		y height of the boat
		x the width of thickness

	for now only one point to be used

	if sing flag is set, negative increment of point

	directions x, y and z accordingly tell about
	direction of the point change
*/
//curve values
#define	BEZ_INTERVAL		20
#define BEZ_CYCLE_Z			'1'
#define BEZ_CYCLE_Y			'2'
#define BEZ_CYCLE_X			'3'
#define	BEZ_DIR_SIGN		'4'
#define	BEZ_DIR_X			'5'
#define	BEZ_DIR_Y			'6'
#define	BEZ_DIR_Z			'7'
#define BEZ_PRINT_COORD		'p'
#define BEZ_POINT_INC		0.1
//modelling screen, offset for mirroring of the array coordinates
#define SCREEN_X_OFFSET		4
