/*	boat_model.c

	all boat and particle rendering and creation functions
*/
#include "open.h"
//variables
extern GLint boat_list, screen_list, part_list, flus1_list, flus2_list, flus3_list;
extern GLint verror;
extern GLfloat boatX, boatZ, boat_angle;
extern int bez_z, bez_y, bez_x;
extern int demo;
int patch = 1;
int particles_count = 0;
int array_initialised = 0;
//functions
void createParticleList(void);
void createBoatList(void);
void createBoatScreenList(void);
void generateBoat(void);
extern float mod(float);
extern double drand48(void);
void moveParticle(int is_moving, int);
void newParticle(int, float);
void moveBezierPoints(int, int);
void printBoatData();
void initBoatArray(void);
//arrays
GLfloat part_diffuse[]	= {0.9, 0.9, 0.9, 1.0};
/*	Bezier patches data*/
GLfloat boatBack[4][4][3];
GLfloat boatData[8][4][3] = 
{
 {      {-1.000, 2.000, 0.000},
        {-1.000, 2.000, 0.000},
        {-1.000, 2.000, 0.000},
        {-1.000, 2.000, 0.000},
        },
 {      {-1.800, 2.000, -1.000},
        {-2.000, 0.000, -2.500},
        {0.000, 0.000, -2.500},
        {0.000, 2.000, -2.800},
        },
 {      {-1.500, 2.000, -4.000},
        {-0.600, 1.300, -6.500},
        {-0.300, 1.300, -6.500},
        {-0.200, 2.000, 2.400},
        },
 {      {-0.600, 2.000, -7.500},
        {-0.600, 1.500, -7.300},
        {-0.600, 1.500, -7.300},
        {-0.600, 2.000, -7.500},
        },
/*	second part of the array
	the first and last vextors
	are dummy as they get
	reinnitialised later on
*/
{{-1.000, 2.000, 0.000},
{-1.000, 2.000, 0.000},  
{-1.000, 2.000, 0.000},  
{-1.000, 2.000, 0.000}},

{{0.000, 2.000, -2.800},
{0.000, 0.000, -2.500}, 
{-2.000, 0.000, -2.500},
{-3.000, 2.000, -1.000}},

{{-0.200, 2.000, 2.400}, 
{0.000, 0.000, -1.000}, 
{-2.000, 0.000, -5.000},
{-2.000, 2.000, -4.000}},

{{0.000, 2.000, -7.500}, 
{0.000, 0.000, -7.500}, 
{-2.000, 0.000, -7.500},
{-1.750, 2.000, -7.500}
}
};

GLfloat boatScreenFlus[2][4][4][3] = {
{
	{	{-1.1, 0.0, -3.0}, {-1.1, 0.05, -3.0}, {-1.05, 0.1, -3.0}, {-1.0, 0.15, -3.0} },
	{	{-1.0, 0.0, 0.0}, {-1.0, 0.3, -0.1}, {-1.0, 0.6, -0.3}, {-1.0, 0.9, -1.0} },
	/*	down here points dont play any role as they are mirrored
		later on during initialisation */
	{	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} },
	{	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.6, 0.0}, {0.0, 0.0, 0.0} }
},
{
	{	{-1.1, 0.0, -4.0}, {-1.1, 0.01, -4.0}, {-1.1, 0.01, -4.0}, {-1.1, 0.01, -4.0} },
	{	{-1.1, 0.0, 1.0}, {-1.1, 0.6, 0.6}, {-0.7, 0.2, -3.6}, {-0.8, 0.0, -5.0} },
	/*	the part of fluselage that is below the glass screen
		down here points dont play any role as they are mirrored
		later on during initialisation */
	{	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} },
	{	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.6, 0.0}, {0.0, 0.0, 0.0} }
} };

GLfloat particles[MAX_PARTICLES][4];

/*	upper baoat fluselage, just below the screen */
void createBoatFlus1List(void)
{

	GLfloat diffuse[] = {0.8, 0.8, 0.85, 1.0};
	GLfloat specular[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat emission[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat shininess = 100;
	
	flus1_list = glGenLists(1);

	if (flus1_list != 0)
	{
		glNewList(flus1_list, GL_COMPILE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

		if (patch)
		{
			//
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 12, 4,
					0, 1, 3, 4, &boatScreenFlus[1][0][0][0]);
			glEvalMesh2(GL_FILL, 0, BEZ_INTERVAL, 0, BEZ_INTERVAL);
		
		}
		glEndList();
	} else {
		if (verror)
		{	
			printf("error, could not generate list for upper boat fluselage\n");
		}
	}
}

void createBoatScreenList(void)
{
	int i, j;
	GLfloat diffuse[]  = {0.3, 0.0, 0.3, 0.5};
	GLfloat specular[] = {0.7, 0.7, 0.8, 1.0};
	GLfloat emission[] = {0.05, 0.05, 0.05, 1.0};
	GLfloat shininess = 300;
	
	screen_list = glGenLists(1);

	if (screen_list != 0)
	{
		glNewList(screen_list, GL_COMPILE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		//control points
		if (!demo)
		{
			glBegin(GL_POINTS);
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{

					glVertex3fv(boatScreenFlus[0][i][j]);

				}
			}
			glEnd();
		//////////////////////////
		/*	bezier stuff */		
		}
		if (patch)
		{

			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatScreenFlus[0][0][0][0]);
			glEvalMesh2(GL_FILL, 0, BEZ_INTERVAL, 0, BEZ_INTERVAL);
		} else {
			glBegin(GL_POINTS);
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatScreenFlus[0][0][0][0]);
			for (i = 0; i <= 8; i++)
			{
				for (j = 0; j <= 30; j++)
				{
					glEvalCoord2f((GLfloat)j/30.0, (GLfloat)i/8.0);
				}
			}
			glEnd();
		}
		glEndList();
	} else {
		if (verror)
		{	
			printf("error, could not generate list for screen\n");
		}
	}
}

void createParticleList(void)
{
	GLfloat specular[]		= {0.6, 0.6, 0.6, 1.0};
	GLfloat emission[]		= {0.4, 0.4, 0.4, 1.0};
	GLfloat shininess = 300;

		part_list = glGenLists(1);

	if(part_list != 0)
	{
		glNewList(part_list, GL_COMPILE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, part_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		//glut sphere
		glutSolidSphere(0.1, 6, 6);
		
		glEndList();
	} else {
		if (verror)
		{
			printf("Cannot generate GL list for a sphere the particle\n");
		}
	}
}

void createBoatList(void)
{
	int i, j;
	GLfloat diffuse1[]	=	{0.9, 0.1, 0.1, 1.0};
	GLfloat diffuse2[]	=	{0.3, 0.4, 0.9, 1.0};
	GLfloat diffuse3[]	=	{0.8, 1.0, 0.0, 1.0};
	GLfloat specular[]	=	{0.8, 0.8, 0.8, 1.0};
	GLfloat emission1[]	=	{0.04, 0.03, 0.01, 0.2};
	GLfloat emission2[]	=	{0.6, 0.03, 0.01, 0.2};
	GLfloat shininess = 300;
	
	boat_list = glGenLists(1);

	if(boat_list != 0)
	{
		glNewList(boat_list, GL_COMPILE);

	/*	material definiton here
	*/
		if (!demo)
		{
			glBegin(GL_POINTS);
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					/*	show which point is selected*/
					if (i == bez_z && j == bez_y)
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse3);
						glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission2);
					} else {
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);
						glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
					}
					glVertex3fv(boatData[i][j]);
				//	printf("working on %d row away, %d element\n", i, j);
				}
			}
			glEnd();
		}

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission1);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

		//actual modelling
		////
		//test wheter points or lighted patch
		////
		if (patch)
		{
			//BOTTOM
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatData[0][0][0]);
			glEvalMesh2(GL_FILL, 0, BEZ_INTERVAL, 0, BEZ_INTERVAL);
			//TOP
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatData[4][0][0]);
			glEvalMesh2(GL_FILL, 0, BEZ_INTERVAL, 0, BEZ_INTERVAL);
			//BACK
			glMap2f(GL_MAP2_VERTEX_3,	0, 1, 3, 4,
										0, 1, 12,4, &boatBack[0][0][0]);
			glEvalMesh2(GL_FILL, 0, BEZ_INTERVAL, 0, BEZ_INTERVAL);
		} else {

			glBegin(GL_POINTS);
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatData[0][0][0]);
			for (i = 0; i <= 8; i++)
			{
				for (j = 0; j <= 30; j++)
				{
					glEvalCoord2f((GLfloat)j/30.0, (GLfloat)i/8.0);
				}
			}
			glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &boatData[4][0][0]);
			for (i = 0; i <= 8; i++)
			{
				for (j = 0; j <= 30; j++)
				{
					glEvalCoord2f((GLfloat)j/30.0, (GLfloat)i/8.0);
				}
			}
			glEnd();
		}

		//*/
		glEndList();
	} else {
		if (verror)
		{
			printf("Cannot generate GL list for boat\n");
		}
	}
}

void initBoatArray(void)
{
	GLint i, j;
	/*	Upper part of the boat rail
		The idea is to mirror coordinates along
		y plane and then scale them along y axis too.
	*/
	//mirror the bottom part for 
	for (i = 4; i < 4 * 2 ; i++)
	{
		for (j = 1; j < 3; j++)
		{
		
			boatData[i][j][0] = boatData[i - 4][j][0];
			boatData[i][j][1] = - 0.25 * boatData[i - 4][j][1] + 2.5;
			boatData[i][j][2] = boatData[i - 4][j][2];
		}
	}

}

void generateBoat(void)
{
	int i, j, k;
	//initialise only once, to preserve changed values
	if (!array_initialised)
	{
		initBoatArray();
		array_initialised = ! array_initialised;
	}
	//mirror the initial points of the bottom partbottom
	for (i = 4; i < 4 * 2 ; i++)
	{
		for (j = 0; j < 2; j++)
		{
			boatData[i][j*3][0] = boatData[i - 4][j*3][0];
			boatData[i][j*3][1] = - 0.25 * boatData[i - 4][j*3][1] + 2.5;
			boatData[i][j*3][2] = boatData[i - 4][j*3][2];
		}
	}
	/*//////
	//list for screen
	//////
	//mirror the coordinates for boatScreen array
	*/
	for (i = 2; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 2; k++)
			{
				//x is mirrored against the x plane, easy
				boatScreenFlus[k][i][j][0] = -boatScreenFlus[k][3-i][j][0];
				//y is left identical symmetrically, all columns have the same height
				boatScreenFlus[k][i][j][1] = boatScreenFlus[k][3-i][j][1];
				//z is mirrored identically too
				boatScreenFlus[k][i][j][2] = boatScreenFlus[k][3-i][j][2];
			}
		}
	}
	//back of the boat array setup
	
	boatBack[0][0][0] = -0.6;
		boatBack[0][0][1]  = 2.0;
		boatBack[0][0][2] = -5.5;
	for (i = 0; i < 3; i++)
	{
		//once side
		//boatBack[0][0][i] =boatData[3][1][i];
		boatBack[0][3][i] = boatData[3][0][i];
		boatBack[0][1][i] = (boatBack[0][0][i] * 2.0 + boatBack[0][3][i]) / 3.0;
		boatBack[0][2][i] = (boatBack[0][0][i] + boatBack[0][3][i] * 2.0) / 3.0;
		//the last side
		boatBack[3][0][i] = boatBack[0][0][i];
		boatBack[3][3][i] = boatBack[0][3][i];
		boatBack[3][1][i] = boatBack[0][1][i];
		boatBack[3][2][i] = boatBack[0][2][i];
		//middle;
	}
	//mirror x values
	boatBack[3][0][0] = -boatBack[0][0][0];
	boatBack[3][3][0] = -boatBack[0][3][0];
	boatBack[3][1][0] = -boatBack[0][1][0];
	boatBack[3][2][0] = -boatBack[0][2][0];
	for (i = 0; i < 3; i++)
	{
		boatBack[1][0][i] = (boatBack[0][0][i] * 2.0 + boatBack[3][0][i]) / 3.0;
		boatBack[1][3][i] = (boatBack[3][3][i] + boatBack[0][3][i] * 2.0) / 3.0;
		boatBack[2][0][i] = (boatBack[3][0][i] * 2.0 + boatBack[0][0][i]) / 3.0;
		boatBack[2][3][i] = (boatBack[0][3][i] + boatBack[3][3][i] * 2.0) / 3.0;
	//and evaluate the last 4 values
		boatBack[1][1][i] = (boatBack[0][1][i] * 2.0 + boatBack[3][1][i]) / 3.0;
		boatBack[1][2][i] = (boatBack[3][2][i] + boatBack[0][2][i] * 2.0) / 3.0;
		boatBack[2][1][i] = (boatBack[3][1][i] * 2.0 + boatBack[0][1][i]) / 3.0;
		boatBack[2][2][i] = (boatBack[0][2][i] + boatBack[3][2][i] * 2.0) / 3.0;
	}
	boatBack[1][1][2] -= 0.3;
	boatBack[2][1][2] -= 0.3;
	boatBack[1][2][2] -= 0.2;
	boatBack[2][2][2] -= 0.2;
	boatBack[1][1][1] -= 0.1;
	boatBack[2][1][1] -= 0.1;
	///////
	//bezier
	///////
	glEnable(GL_MAP2_VERTEX_3);
	//for bezier patch
	if (patch)
	{
		glEnable(GL_AUTO_NORMAL);
		glMapGrid2f(BEZ_INTERVAL, 0.0, 1.0, BEZ_INTERVAL, 0.0, 1.0);
	}
	/*	doing the lists */
	createBoatList();
	createBoatFlus1List();
	createBoatScreenList();

}



void drawBoat(void)
{
	//glLoadIdentity();
	glPushMatrix();
	glTranslatef(boatX, 0.0, boatZ);
	glRotatef(boat_angle, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 7.0 - BOAT_OFFSET);
	glRotatef(-5.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		glTranslatef(0.0, -1.3, 0.0);
		
		glCallList(boat_list);
		glScalef (-1.0, 1.0, 1.0);
		glCallList(boat_list);
		
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0, 0.7, -1.3);
		glScalef(-1.0, 1.0, 1.0);

		glCallList(flus1_list);
	glPopMatrix();

	glPopMatrix();
	///////////
	//particles work only here for now
	///////////
}

void drawBoatScreen(void)
{
	glPushMatrix();
	glTranslatef(boatX, 0.0, boatZ);
	glRotatef(boat_angle, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 7.0 - BOAT_OFFSET);
	glRotatef(-5.0, 1.0, 0.0, 0.0);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glDepthMask(GL_FALSE);
		glEnable( GL_BLEND );
			glTranslatef(0.0, 0.8, -1.0);
				glScalef(-1.0, 1.0, 1.0);
			glCallList(screen_list);
		glDisable( GL_BLEND );
		glDepthMask(GL_TRUE);

	glPopMatrix();
	return;
}


void drawParticles(void)
{
	int i;
		glPushMatrix();
	
	if (!demo)
	{
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable( GL_BLEND );
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, part_diffuse);
	for (i = 0; i < MAX_PARTICLES; i++)
	{
		//glLoadIdentity();
		if (!demo)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
		}
		glPushMatrix();
		glTranslatef(particles[i][0], particles[i][1], particles[i][2]);
		if (particles[i][1])
			{	
				if (!demo)
				{
					glColor4f(0.9, 0.9, 1.0, particles[i][3] / (float) MAX_PARTICLES);
				}
				glTranslatef(0.0, sin(particles[i][3])/10.0, 0.0);
				glCallList(part_list);
			}
		glPopMatrix();
		if (!demo)
		{
			glPopAttrib();
		}
	}
	if (!demo)
	{
		glDisable( GL_BLEND );
	}

	glPopMatrix();
	
}



void newParticle(int is_moving, float move_speed)
{
	if (is_moving)
   //translation to the back of either of boat rails, plus timestamp
	{
		//offset for the motor, or the distance between particle lines
		double motor_offset = ((drand48() > 0.5) ? 1.0 : -1.0) + drand48() - 0.5;
		

		particles[particles_count][0] = (GLfloat) boatX - cos(boat_angle*PI/180) * (motor_offset) - ( BOAT_OFFSET + 1.0) * sin(boat_angle*PI/180);
		particles[particles_count][1] = move_speed * drand48() / 2.0 - 0.25;
		particles[particles_count][2] = (GLfloat) boatZ + sin(boat_angle*PI/180) * (motor_offset) - ( BOAT_OFFSET + 1.0) * cos(boat_angle*PI/180);
		particles[particles_count][3] = MAX_PARTICLES;		//save angle too.
	} else {
		/*	particles are not created as boat doesnt move! 
			timestamp and material are collapesed - particle deleted
		*/
		particles[particles_count][0] = 0.0;
		particles[particles_count][1] = 0.0;
		particles[particles_count][2] = 0.0;
		particles[particles_count][3] = 0.0;
	}
	//printf("new particle created at %f, %f, %f\n", particles[particles_count][0], particles[particles_count][1], particles[particles_count][2]);

}

void moveParticle(int is_moving, int move_delta)
{
	int i;
	
	// determine if we need to move the particles!
	static int move_sum = 0;
	move_sum += move_delta;
	const float PARTICLE_MOVE_STEP = 10;

	if (move_sum > PARTICLE_MOVE_STEP)
	{
		float move_speed = move_delta * PARTICLE_SPEED;

		newParticle(is_moving, move_speed);
		move_sum -= PARTICLE_MOVE_STEP;

		//circular cueue for particles
		particles_count = (1 + particles_count) % MAX_PARTICLES;

		// advance all particles
		for (i = 0; i < MAX_PARTICLES; i++)
		{
			particles[i][1] += move_speed * (drand48() - 0.5);
		}
	}

	
}
//////////
//mini bezier constructor
//////////
void moveBezierPoints(int axis, int minus_sign)
{
	boatData[bez_z][bez_y][axis] += BEZ_POINT_INC * ((minus_sign) ? -1.0 : 1.0);
}

void printBoatData(void)
{
	int i, j;
	//clear the screen
	for (i = 0; i < 25; i++)
		printf("\n");
	printf("Boat surface Bezier control points\n");
	for (i = 0; i < 80; i++)
		printf("-");
	printf("\n{\n");

	for (i = 0; i < 4 ; i++)
	{
		printf(" {\t");
		for (j = 0; j < 4; j++)
		{
			printf("{%.3f, %.3f, %.3f},\n\t", boatData[i][j][0],boatData[i][j][1],boatData[i][j][2]);

			//printf("{%.3f,\t%.3f,\t%.3f)\n", boatData[i + 4][j][0],boatData[i + 4][j][1],boatData[i + 4][j][2]);
		}
		printf("},\n");
	}
	printf("\n}\n");
}
