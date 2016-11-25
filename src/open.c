/*	open.c
	the main program file
	
	part of the code from:
           OpenGL Framework using GLUT 3.7 

           Rob Fletcher  2001
        
*/
#include "open.h"

GLvoid change_view_to_boat(GLvoid);
GLvoid change_view_to_boat_back(GLvoid);
GLvoid change_view_to_side(GLvoid);
GLvoid change_view_to_mobile(GLvoid);
GLvoid change_view_to_plan(GLvoid);
GLvoid idle(GLvoid);
GLvoid moveBoat(float, float);
GLvoid rotateBoat(float);
int collisionTest(float, float);	/* function tests for collision and returns */
float boatAreaTest(float, float);
extern int verbose;
extern GLvoid generateLandscape(GLvoid);
extern GLvoid drawLandscape(GLvoid);
extern GLvoid drawWater(GLvoid);
extern GLvoid drawParticles(GLvoid);
extern GLvoid generateBoat(GLvoid);
extern GLvoid drawBoat(GLvoid);
extern GLvoid drawBoatScreen(GLvoid);
extern int shade_mode;
extern GLvoid lights(GLvoid);
extern GLfloat mod(GLfloat);
extern GLvoid moveParticle(int is_moving, int);
extern GLvoid createParticleList(GLvoid);
extern GLvoid moveBezierPoints(int, int);
extern GLvoid printBoatData();
extern double sqr(double);			/* simple square function, helps in long arrays */

extern GLvoid createBoatList(GLvoid);
									/* Here we will put variables which we wish to be global */
GLint window;						/* The number and then size of our GLUT window */
const GLint window_width = 1027;
const GLint window_height = 768;
GLint look_flag = VIEW_BOAT_FRONT;	/* the mode of view */
									/* camera viewing options
										free few and boat view */
int demo = 1;
GLdouble camera_distance = 40.0, theta = 5.0 * PI / 180.0, phi = 0.0;
GLfloat boatX=0.0, boatZ=0.0, boat_angle=0.0;
									/* Bezier Curves */
int bez_z = 0, bez_y = 0, bez_sign_minus = 0;

									/*	the variables for movement boat*/
GLfloat boat_rotate_delta = 0.0, boat_move_delta = 0.0;
GLint particleTime = 0;				/* trace the particle movement */
GLint mouseDelta[2] = {0, 0};		/* mouse change */
GLint mouseCoords[2] = {0, 0};
GLint mouseSet = 0;
									/*	integer values that correspod to lists of 
												landscape, boat and particles
									*/
GLint landscape_list, boat_list, screen_list, part_list, flus1_list;
									/* views */
const double deg_2_rad = PI / 180.0;
const double rad_2_deg = 180.0 / PI;
//////////////////////////////////////////////
//Functions
//////////////////////////////////////////////
									/* Simple window transformation routine */
GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, (GLint)Width, (GLint)Height);  /* Set the viewport */
  glMatrixMode(GL_PROJECTION);      /* Select the projection matrix */
  glLoadIdentity();					/* Reset The Projection Matrix */
  
  if (verbose) {	printf("seting up vewport, projection matrix, aspect ratio and model view matrix\n");	}


  gluPerspective(90.0, (GLfloat) Width / (GLfloat) Height, 1.0, 500.0);  /* Calculate The Aspect Ratio Of The Window */

 
									//*/
  glMatrixMode(GL_MODELVIEW);       /* Switch back to the modelview matrix */
}


									/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)	
{
									/*Set the clear colour, colour of the sky*/
	glClearColor(0.6, 0.7, 0.9, 0.0);
	Transform( Width, Height );     /* Perform the transformation */
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR );
	glShadeModel(GL_SMOOTH);
	glPointSize(5.0);
	glLineWidth(2.0);
	
	generateLandscape();
	generateBoat();
	createParticleList();
	lights();

}
									/* The function called when our window is resized  */
GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)    Height=1;       /* Sanity checks */
  if (Width==0)      Width=1;
  Transform( Width, Height );       /* Perform the transformation */

  glutPostRedisplay();
}


									/* The main drawing function

   In here we put all the OpenGL and calls to routines which manipulate
   the OpenGL state and environment.

   This is the function which will be called when a "redisplay" is requested.
*/

GLvoid drawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */
	
	glPushMatrix();
	glLoadIdentity();
	
	if(look_flag == VIEW_MOBILE)
	{
		double cameraX, cameraY, cameraZ;
		double l_cosT = camera_distance * cos(theta);
	
	    cameraX = l_cosT*sin(phi);
	    cameraY = camera_distance * sin(theta);
	    cameraZ = l_cosT*cos(phi);
	
	    gluLookAt(cameraX, cameraY, cameraZ,
					0.0, 0.0, 0.0,
					0.0, 1.0, 0.0);
	} else if (look_flag == VIEW_SIDE)
	{
			gluLookAt(5.0, -4.0, 0.0,
					0.0, -5.0, 2.0,
					0.0, 1.0, 0.0);
			glRotatef(180-boat_angle, 0.0, 1.0, 0.0);
			glTranslatef(-boatX, -8.0, -boatZ);
		
	} else if (look_flag == VIEW_BOAT_FRONT)
	{
    /**********************************************
       From Practical 3: robot -> boat
		 A boat view point is constructed using the 
       boat's own variable in a rotation and a
       translation. Note the angle has been shifted
       by 180 degrees because front for the boat is
       the +ve Z axis but default cameras look down 
       the -ve Z axis.
    ************************************************/
			glTranslatef(0.0, 0.25, 2.5 - BOAT_OFFSET);
			glRotatef(180-boat_angle, 0.0, 1.0, 0.0);
			glTranslatef(-boatX, -2.0, -boatZ);
	} else if (look_flag == VIEW_BOAT_BACK) {
			glTranslatef(0.0, 0.0, BOAT_OFFSET - 4.0);
			glRotatef(-boat_angle, 0.0, 1.0, 0.0);
			glTranslatef(-boatX, -2.2, -boatZ);
  } else {
    gluLookAt(0.0, 100.0,  0.0,
	      0.0,   0.0,  0.0,
	      0.0,   0.0, -1.0);
  }	
	/*
		The following order of rendering secures correct blending
	*/
    drawLandscape();
	drawParticles();
	drawBoat();
	drawWater();
	drawBoatScreen();
	
	glPopMatrix();
	glFlush();
	glutSwapBuffers();                    
}

GLvoid lights(GLvoid)
{
	//light position
	GLfloat light_pos[] = {	0.0, 50.0, 20.0, 1.0 };
	//light parameters
	GLfloat specular[]={0.6, 0.6, 0.6, 1.0};
	GLfloat global_abm[]=  {0.2, 0.2, 0.2, 1.0};
	GLfloat amb_and_diff[] =   {0.4, 0.4, 0.4, 1.0};
  
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb_and_diff);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, amb_and_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//local mode, two-sided, global ambient
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_abm);

	//enables the lighting
	glEnable(GL_LIGHT0);
	return;
}

GLvoid activeMouseMotion(int x, int y)
{
	int tdelta;
	/*	mouse change triggers the change of delta
		which in tern changes the camera position

		once the mouse is depressed the valuse must be cleared

		egh... mouseDelta is not really needed! <:->
	*/
	if (look_flag == VIEW_MOBILE)
	{
		mouseDelta[0] = mouseCoords[0] - x;	mouseCoords[0] = x;
		tdelta = mouseDelta[1] = mouseCoords[1] - y; mouseCoords[1] = y;
		
		phi		+= MOUSE_RESP * mouseDelta[0];
		theta	+= MOUSE_RESP * tdelta;
		if (mod(theta)  >= (PI / 2.0))
		{
		theta	-= MOUSE_RESP * tdelta;
		}
		glutPostRedisplay();
	}
	
}

GLvoid processMouse(int button, int state, int x, int y)
{
	if (verbose)	{ printf("mouse processing, %d, %d %s\n", x, y, (state == GLUT_UP) ? "mouse released" : "mouse pressed"); }
	if (state == GLUT_DOWN)
	{								/* mouse pressed */
		mouseCoords[0] = x;	mouseCoords[1] = y;
		mouseSet = 1;
	}	else {						/* clear the array */
		mouseCoords[0] = 0;	mouseCoords[1] = 0;
		mouseSet = 0;
	}
}

									/*	new keyboard assignments
										rotate functions defined
										now in normalKey()
										here keyborad movements 
										move the boat if either 
										roation or movement has
										been done
									*/

static int prev_time = 0;
GLvoid idle(GLvoid)
									/*	idle function
										called either by timer
										or by glut idle call
									*/
{
	const int SLEEP_MSECONDS = 10;
	int et =  glutGet(GLUT_ELAPSED_TIME);
	int delta_time = et - prev_time;
	prev_time = et;

	// boat movement
	if (boat_rotate_delta)	{	rotateBoat(boat_rotate_delta * delta_time);	}
	if (boat_move_delta)	{	moveBoat(boat_angle, boat_move_delta * delta_time);		}
	
	// particle movement, even when idle!
	moveParticle(boat_rotate_delta || boat_move_delta, delta_time);

	/*	
		if boat doesnt move or rotate, particles are just killed
	*/
	glutPostRedisplay();

#ifdef WIN32
	// sleep for a little bit
	Sleep(SLEEP_MSECONDS);
#else
	usleep(SLEEP_MSECONDS * 1000);
#endif
}

GLvoid moveBoat(float boat_angle, float move_delta)
{
	float angle_pos = boat_angle*(float)PI / 180;
	float move_amount = BOAT_SPEED * move_delta;
	float xinc = move_amount * sin(angle_pos);
	float zinc = move_amount * cos(angle_pos);
									/*	increment of boat
										boat_angle represents the direction of boat								
									*/
	if (!collisionTest((boatX += xinc),  (boatZ += zinc) ))
	{
		/*	undo the values
			also add extra realism, back off a little
		*/

		boatX -= xinc * (1.0 );
		boatZ -= zinc * (1.0 );
	}
}

GLvoid rotateBoat(float angle_delta)
{
	boat_angle += angle_delta * BOAT_ROTATE_SPEED;
}

int collisionTest(float x, float z)
{
	//based on the oval of the lake shape
	if (boatAreaTest(x, z) < 1.0)
	{
		return 1;
	} else {
		printf("on the loan, %f, %f\n", x, z);
		return 0;
		
	}
}

float boatAreaTest(float x, float z)
{
										/*	Eppiliptical function,
											everything below the are of lake ellips returns
											value less than 1.0,
										*/
	return (sqr(z)/sqr(LAKE_WIDTH/2.0) + sqr(x)/sqr(LAKE_LENGTH/2.0));
}

GLvoid specialKey(int key, int x, int y)
{
										/*	special key hadles
											arrow keys and F# functions
										*/
	switch(key){
  
	case GLUT_KEY_DOWN:
			if (!demo)
			{
				boat_move_delta = -1;
			}
		break;
	case GLUT_KEY_UP:
		boat_move_delta = 1;
		break;
	
	case GLUT_KEY_RIGHT:
		boat_rotate_delta = -1;
		break;

	case GLUT_KEY_LEFT:
		boat_rotate_delta = 1;
		break;
	//*/
	case GLUT_KEY_PAGE_UP:
		if(camera_distance > CAMERA_MIN_DISTANCE)
			camera_distance -= camera_distance / 5.0 + 1.0;
		break;

	case GLUT_KEY_PAGE_DOWN:
		if(camera_distance < CAMERA_MAX_DISTANCE)
			camera_distance += camera_distance / 5.0 + 1.0;
			break;

									/*	views:
										F1	normal, front of the boat
										F2	back of the boat
										F3	side; for now mobile (to be fixed to fixed)
										F4	plan, above
									*/
	case GLUT_KEY_F1 :
		change_view_to_boat();
		break;

	case GLUT_KEY_F2 :
		change_view_to_boat_back();
		break;

	case GLUT_KEY_F3 :
		change_view_to_side();
		break;

	case GLUT_KEY_F4 :
		change_view_to_plan();

	default:
		fprintf(stdout, "%c, No function attached to key.\n", key);
  }
  glutPostRedisplay();
}

GLvoid specialReleaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : 
		case GLUT_KEY_RIGHT : 
			boat_rotate_delta = 0.0;
			break;
		case GLUT_KEY_UP : 
		case GLUT_KEY_DOWN :
			boat_move_delta = 0.0;
			break;
	}
}

GLvoid normalKey(GLubyte key, GLint x, GLint y) 
{
    switch ( key )    { 
	
	case ESCAPE :
        printf("escape pressed. exit.\n");
		glutDestroyWindow(window);	/* Kill our window */
		exit(0); 						/* Very dirty exit */                  
        break;						/* Do we need this??? */

	case KEY_VEW_DOWN:
		if(theta > -1.0)
		theta -= 5.0 * deg_2_rad;
		break;

	case KEY_VEW_UP:
	    if(theta < 1.0)
			theta += 5.0 * deg_2_rad;
		break;

	case KEY_VEW_RIGHT:
		phi += 5.0 * deg_2_rad;
		break;

	case KEY_VEW_LEFT:
	    phi -= 5.0 * deg_2_rad;
	    break;

									/*	handle bezier functions
										bezier point cycle
									*/
	case BEZ_CYCLE_Z:
		if (!demo)
		{
			bez_z = ++bez_z % 4;
			generateBoat();
		}
		break;
	case BEZ_CYCLE_Y:
		if (!demo)
		{
			bez_y = ++bez_y % 4;
			generateBoat();
		}
		break;

	case BEZ_DIR_SIGN :
		bez_sign_minus = !bez_sign_minus;
		break;
	case BEZ_DIR_X :
	case BEZ_DIR_Y :
	case BEZ_DIR_Z :
		if (!demo)
		{
			moveBezierPoints((int)key - 53, bez_sign_minus);
			printf("key value: %d, sign %c\n", (int)key - 53, (bez_sign_minus) ? '-' : '+');
			generateBoat();
		}
		break;
	
	case BEZ_PRINT_COORD :
		if (!demo)
		{
			printBoatData();
		}
		break;

     default:
	break;
    }
	glutPostRedisplay();
}

GLvoid change_view_to_plan(GLvoid){
  float w, h;
  look_flag = VIEW_PLAN;
  w = (float) glutGet(GLUT_WINDOW_WIDTH );
  h = (float) glutGet(GLUT_WINDOW_HEIGHT );
  ReSizeGLScene(w, h);
  return;
}

GLvoid change_view_to_mobile(GLvoid){
  if(look_flag == VIEW_PLAN){
    float w, h;
    w = (float) glutGet(GLUT_WINDOW_WIDTH );
    h = (float) glutGet(GLUT_WINDOW_HEIGHT );
    ReSizeGLScene(w, h);
  }
  look_flag = VIEW_MOBILE;
  glutPostRedisplay();
  return;
}

GLvoid change_view_to_side(GLvoid){
  if(look_flag == VIEW_PLAN){
    float w, h;
    w = (float) glutGet(GLUT_WINDOW_WIDTH );
    h = (float) glutGet(GLUT_WINDOW_HEIGHT );
    ReSizeGLScene(w, h);
  }
  look_flag = VIEW_SIDE;
  glutPostRedisplay();
  return;
}

GLvoid change_view_to_boat(GLvoid)
{	

	if(look_flag == VIEW_PLAN)
	{
	    float w, h;
		w = (float) glutGet(GLUT_WINDOW_WIDTH );
	    h = (float) glutGet(GLUT_WINDOW_HEIGHT );
		ReSizeGLScene(w, h);
	}
	look_flag = VIEW_BOAT_FRONT;
	glutPostRedisplay();
	return;
}

GLvoid change_view_to_boat_back(GLvoid)
{
		if(look_flag == VIEW_PLAN)
	{
	    float w, h;
		w = (float) glutGet(GLUT_WINDOW_WIDTH );
	    h = (float) glutGet(GLUT_WINDOW_HEIGHT );
		ReSizeGLScene(w, h);
	}
	look_flag = VIEW_BOAT_BACK;
	glutPostRedisplay();
	return;
}

GLvoid menu(GLint id)
{


  if(id == look_flag)
    return;
  printf("changing view to # %d\n", id);

  switch(id){

	case VIEW_BOAT_FRONT:
		change_view_to_boat();	
		break;
	
	case VIEW_PLAN:
		change_view_to_plan();
		break;

	case VIEW_MOBILE:
		change_view_to_mobile();
		break;
	
	case VIEW_BOAT_BACK:
		change_view_to_boat_back();
		break;
	
	case VIEW_SIDE:
		change_view_to_side();
		break;
	
	case -1:
		exit(0);

  }

  return;
}

GLvoid world_menu(GLint mode)
{
	demo = mode - 6;
	generateLandscape();
	generateBoat();
	glutPostRedisplay();
}

GLvoid viz(int state)
								/* doesnt do much, no input, though when minimized reduced CPU load */
{
 switch (state)
 {
   case GLUT_VISIBLE:
             glutIdleFunc(idle);
             break;
   case GLUT_NOT_VISIBLE:
              glutIdleFunc(NULL);
              break;
   default:
             break;
 }
}

GLvoid Timer( int value )
{
   if( value ) idle();
   glutTimerFunc(40,Timer,value);
}

/*************************** Main ***************************************************************/

int main(int argc, char **argv) 
{
	GLint world_m;
									/* Initialisation and window creation */

	glutInit(&argc, argv);            /* Initialize GLUT state. */

  glutInitDisplayMode(GLUT_RGBA	|      /* RGB and Alpha */
                      GLUT_DOUBLE |    /* Single buffer */
                      GLUT_DEPTH);     /* Z buffer (depth) */

  glutInitWindowSize(window_width, window_height);         /* set initial window size. */
  glutInitWindowPosition(100, 100);         /* upper left corner of the screen. */

  window = glutCreateWindow("L0");     /* Open a window with a title. */ 
  InitGL(400,400);                     /* Initialize our window. */

									/* Now register the various callback functions */

	glutDisplayFunc(drawGLScene);       /* Function to do all our OpenGL drawing. */
	glutReshapeFunc(ReSizeGLScene);		/* resize function */
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(normalKey);        /* Normal key is pressed */
	glutSpecialUpFunc(specialReleaseKey);
	glutSpecialFunc(specialKey);
	glutMotionFunc(activeMouseMotion);	/* process mouse when key is pressed */
	glutMouseFunc(processMouse);
	glutIdleFunc(idle);					/* idle function for boat movement ... or use timer instead*/
	//glutTimerFunc(40,Timer,1);
	glutVisibilityFunc(viz);			/* minimized idle rendering time */


	glEnable(GL_DEPTH_TEST);			/* for correct rendering of depth */
	 
	/*	MENU
		menu to change plan view
	*/
	
	world_m = glutCreateMenu(world_menu);
	
		glutAddMenuEntry("Editing mode", WORLD_EDIT);
		glutAddMenuEntry("Demo mode", WORLD_DEMO);



	glutCreateMenu(menu);
	glutAddMenuEntry("Normal View",	VIEW_BOAT_FRONT);
	glutAddMenuEntry("Back View",	VIEW_BOAT_BACK);
	glutAddMenuEntry("Mobile View", VIEW_MOBILE);
	glutAddMenuEntry("Side View",	VIEW_SIDE);
	glutAddMenuEntry("Above View",	VIEW_PLAN);
		

	glutAddSubMenu("Editin mode", world_m);


	glutAddMenuEntry("Exit", -1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);





									/* Now drop into the event loop from which we never return */

  glutMainLoop();                      /* Start Event Processing Engine. */  
  return 1;
}
