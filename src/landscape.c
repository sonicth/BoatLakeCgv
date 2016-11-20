/*	landscape.c
	land and water for the landscape is drawn here
	normals are computed first for faces of each triangle
	and then for

*/

//include files
#include "open.h"		//local definitions



/*	GLOBAL VARIABLES
	the array of vectors of the scene,
	and for normals too
	integers for option wheither output is constrained, whether landscape is generated

	GLinteger values from landscape lists
*/
int verbose = 1, verror = 1, landscape_generated = 0;
int fractal_type = PERLIN_TYPE;
GLint landscape_list, water_list;

GLfloat landscape[S_LENGTH + 1][S_WIDTH + 1][3];
GLfloat water[S_LENGTH + 1][S_WIDTH + 1][3];
//land and water
GLfloat landVertexNormals[S_LENGTH + 1][S_WIDTH + 1][3];
GLfloat waterVertexNormals[S_LENGTH + 1][S_WIDTH + 1][3];
//land and water
GLfloat landFaceNormals[S_LENGTH][2 * S_WIDTH][3];
GLfloat waterFaceNormals[S_LENGTH][2 * S_WIDTH][3];
void generateLandscape(void);
GLvoid createLandscapeList(GLvoid);
GLvoid createWaterList(GLvoid);
GLvoid createFaceNormals(GLvoid);
GLvoid createVertexNormals(GLvoid);
GLvoid elevateLand(GLvoid);
GLvoid drawLandScape(GLvoid);
GLvoid drawWater(GLvoid);
extern GLdouble sqr(GLdouble);
extern double PerlinNoise_2D(double, double);
extern GLvoid normalise(GLfloat [3]);
extern float boatAreaTest(float, float);
extern GLvoid cross(GLfloat [3], GLfloat [3], GLfloat [3], GLfloat [3]);

/////////////////////////////
/*	GENERATING LANDSCAPE
	start from perlin fractal land
*/
GLvoid elevateLand(GLvoid)
{
	int i, j, k = 0;
	float ellip_area;
	if (fractal_type == PERLIN_TYPE)
	{
		if (verbose)	{	printf("making mountains on Perlin algorithm\n"); }
		
		for (i = 0; i <= S_LENGTH; i++)
		{
			for (j = 0; j <= S_WIDTH; j++)
			{
				ellip_area = (boatAreaTest(landscape[i][j][0], landscape[i][j][2]) - 1.0);
				//actual definition of each point of the landscape
				landscape[i][j][1] = (PerlinNoise_2D((double)i / 3.0, (double)j / 6.0) * 2.0 + 5.0) * ellip_area;
			}
		}
	} else {
		//no other types available after i've decided perlin wins!
		return;
	}
}

GLvoid generateLandscape(GLvoid)
{
	int i, j;
	GLfloat tmp_x, tmp_z;
	//legth and width of idividual triangle strip
	const GLfloat sl_size = L_LENGTH	/ S_LENGTH;
	const GLfloat sw_size = L_WIDTH		/ S_WIDTH;

	for (i = 0; i <= S_LENGTH; i++)
	{
		tmp_x = (sl_size * i) - L_LENGTH * 0.5;
		for (j = 0; j <= S_WIDTH; j++)
		{
			tmp_z = (sw_size * j) - L_WIDTH * 0.5;
			//inital points for land...
			landscape[i][j][0] = tmp_x;
			landscape[i][j][1] = 0.0;
			landscape[i][j][2] = tmp_z;
			//...and for water
			water[i][j][0] = tmp_x;
			water[i][j][1] = 0.0;
			water[i][j][2] = tmp_z;
		}
	}
	//mountains
	elevateLand();
	//normals

	if (verbose)	{	printf("generating face and vertex normals...\n"); }
	createFaceNormals();
	createVertexNormals();
	//lists for water and land
	createLandscapeList();
	
	createWaterList();
	landscape_generated = 1;
}

GLvoid createFaceNormals(GLvoid)
{
	int i, j, k;
	GLfloat n[3];
	//for land
	for (i = 0; i < S_LENGTH; i++)
	{
		for (j = 0; j < S_WIDTH; j++)
		{
					cross(	landscape[i][j],
							landscape[i + 1][j],
							landscape[i][j + 1],
							n);

					normalise(n);
					for (k = 0; k < 3; k++) landFaceNormals[i][j * 2][k] = n[k];

					cross(	landscape[i][j + 1],
							landscape[i + 1][j],
							landscape[i + 1][j + 1],
							n);

					normalise(n);
					for (k = 0; k < 3; k++) landFaceNormals[i][j * 2 + 1][k] = n[k];
		}
	}
	//for water
	for (i = 0; i < S_LENGTH; i++)
	{
		for (j = 0; j < S_WIDTH; j++)
		{
					cross(	water[i][j],
							water[i + 1][j],
							water[i][j + 1],
							n);

					normalise(n);
					for (k = 0; k < 3; k++) waterFaceNormals[i][j * 2][k] = n[k];

					cross(	water[i][j + 1],
							water[i + 1][j],
							water[i + 1][j + 1],
							n);

					normalise(n);
					for (k = 0; k < 3; k++) waterFaceNormals[i][j * 2 + 1][k] = n[k];
		}
	}
}

GLvoid createVertexNormals(GLvoid)
{
	int i, j, k;
	//////////
	//for land
	//////////
	//sorting out the vertex in the middle of the triangle
	for (i = 0; i <=  S_LENGTH; i++)
	{
		for (j = 0; j <= S_WIDTH ; j++)
		{
			for (k = 0; k < 3; k++)
			{
				landVertexNormals[i][j][k] = 
				( landFaceNormals[(i - 1 + S_LENGTH) % S_LENGTH][(2 * j - 1 + S_WIDTH * 2) % S_WIDTH * 2][k] 
				+ landFaceNormals[(i - 1 + S_LENGTH) % S_LENGTH][(2 * j + S_WIDTH * 2) % S_WIDTH * 2][k]
				+ landFaceNormals[(i - 1 + S_LENGTH) % S_LENGTH][(j * 2 + 1 + S_WIDTH * 2) % S_WIDTH * 2][k]) +
				( landFaceNormals[(i + S_LENGTH) % S_LENGTH][(2 * j - 2 + S_WIDTH * 2) % S_WIDTH * 2][k] 
				+ landFaceNormals[(i + S_LENGTH) % S_LENGTH][(2 * j - 1 + S_WIDTH * 2) % S_WIDTH * 2][k] 
				+ landFaceNormals[(i - 1 + S_LENGTH) % S_LENGTH][(j * 2 + S_WIDTH * 2) % S_WIDTH * 2][k]);
			}
		}
	
	}
	
	for (i = 1; i <  S_LENGTH; i++)
	{
		for (j = 1; j < S_WIDTH; j++)
		{
			for (k = 0; k < 3; k++)
			{
			landVertexNormals[i][j][k] = 
				(landFaceNormals[i - 1][2 * j - 1][k] + landFaceNormals[i - 1][2 * j][k] + landFaceNormals[i - 1][j * 2 + 1][k]) +
				(landFaceNormals[i][2 * j - 2][k] + landFaceNormals[i][2 * j - 1][k] + landFaceNormals[i - 1][j * 2][k]);
			
			}
		}
	
	}
	
	//*/
	//normalise everything
	//*
	for (i = 0; i <= S_LENGTH + 1; i++)
	{
		for (j = 0; j <= S_WIDTH + 1; j++)
		{
			normalise(landVertexNormals[i][j]);
		}
	}
	//*/
	//////////
	//for water
	//////////
		for (i = 0; i <= S_LENGTH + 1; i++)
	{
		for (j = 0; j <= S_WIDTH + 1; j++)
		{
			waterVertexNormals[i][j][0] = 0.0;
			waterVertexNormals[i][j][1] = 1.0;
			waterVertexNormals[i][j][2] = 0.0;
		}
	}
	//*/
	//sorting out the vertex in the middle of the triangle
	for (i = 1; i < S_LENGTH; i++)
	{
		for (j = 1; j < S_WIDTH; j++)
		{
			for (k = 0; k < 3; k++)
			{
			waterVertexNormals[i][j][k] = 
				(waterFaceNormals[i - 1][2 * j - 1][k] + waterFaceNormals[i - 1][2 * j][k] + waterFaceNormals[i - 1][j * 2 + 1][k]) +
				(waterFaceNormals[i][2 * j - 2][k] + waterFaceNormals[i][2 * j - 1][k] + waterFaceNormals[i - 1][j * 2][k]);
			
			}
		}
	
	}
	//*/
	//normalise everything
	//*
	for (i = 0; i <= S_LENGTH + 1; i++)
	{
		for (j = 0; j <= S_WIDTH + 1; j++)
		{
			normalise(waterVertexNormals[i][j]);
		}
	}

}

GLvoid createLandscapeList(GLvoid)
{
	int i, j;
	//setting up parameters for material here
	GLfloat amb_diffuse[]	= {0.1, 0.1, 0.1, 1.0};
	GLfloat specular[]		= {0.2, 0.2, 0.2, 1.0};
	GLfloat emission[]		= {0.05, 0.05, 0.05, 1.0};
	GLfloat shininess = 50;
	
	landscape_list = glGenLists(1);

	if(landscape_list != 0)
	{
		if (verbose)	{	printf("generating land...\n"); }
		glNewList(landscape_list, GL_COMPILE);

	/*	material definiton here
	*/
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, amb_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		

		
	/*	actuall generation of ladscape
		Using triangle strip and stepping
		ussing 3d array with 2 dimesions setting the
		strip and 3rd pointing the x,y,z values
	*/
		glBegin(GL_TRIANGLES);
		for (i = 1; i < S_LENGTH; i++)
		{
		
			
			for (j = 1; j < S_WIDTH; j++)
			{

				//triangle 1 normals
				glColor3f(landscape[i][j][1] * 0.2, 0.8 - landscape[i][j][1] * 0.03, 0.1 - landscape[i][j][1] * 0.03);
				//trinagle 1
				glNormal3fv(landVertexNormals[i][j]);
				glVertex3fv(landscape[i][j]);
				glNormal3fv(landVertexNormals[i + 1][j]);
				glVertex3fv(landscape[i + 1][j]);
				glNormal3fv(landVertexNormals[i][j + 1]);
				glVertex3fv(landscape[i][j + 1]);
				//triangle 2 normals
				
				glNormal3fv(landVertexNormals[i][j + 1]);
				glVertex3fv(landscape[i][j + 1]);
				glNormal3fv(landVertexNormals[i + 1][j]);
				glVertex3fv(landscape[i + 1][j]);
				glNormal3fv(landVertexNormals[i + 1][j + 1]);
				glVertex3fv(landscape[i + 1][j + 1]);
			}
		}
		glEnd();
		glEndList();
	} else {
		if (verror)
		printf("Error: landscape is screwed!\n");
	}
	
}

GLvoid createWaterList(GLvoid)
{
	int i, j;
	GLfloat amb_diffuse[]	= {0.4, 0.4, 0.4, 1.0};
	GLfloat diffuse[]	=	{0.0, 0.8, 0.8, 0.6};
	GLfloat specular[]		= {0.4, 0.4, 0.4, 0.2};
	GLfloat emission[]		= {0.1, 0.3, 0.1, 0.6};
	GLfloat ambient[]		= {0.1, 0.3, 0.4, 1.0};
	GLfloat shininess = 50;
	
	water_list = glGenLists(1);
	if(water_list != 0)
	{
		glNewList(water_list, GL_COMPILE);
		if (verbose)	{	printf("generating water...\n"); }
	/*	material for water
	*/
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

		glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);
	/*	identical generation to landscape
	*/
		glBegin(GL_TRIANGLES);
		for (i = 1; i < S_LENGTH; i++)
		{
			for (j = 1; j < S_WIDTH; j++)
			{
				glNormal3fv(waterVertexNormals[i][j]);
				glVertex3fv(water[i][j]);
				glNormal3fv(waterVertexNormals[i + 1][j]);
				glVertex3fv(water[i + 1][j]);
				glNormal3fv(waterVertexNormals[i][j + 1]);
				glVertex3fv(water[i][j + 1]);

				glNormal3fv(waterVertexNormals[i][j + 1]);
				glVertex3fv(water[i][j + 1]);
				glNormal3fv(waterVertexNormals[i + 1][j]);
				glVertex3fv(water[i + 1][j]);
				glNormal3fv(waterVertexNormals[i + 1][j + 1]);
				glVertex3fv(water[i + 1][j + 1]);
			}
		}
		glEnd();
		glEndList();
	} else {
		if (verror)
		printf("Error: water land failed to generate\n");
	}
}

GLvoid drawLandscape(GLvoid)
{
	// in case landscape is not generated, might cause slight delay in slow computers
	if (!landscape_generated) generateLandscape();
	//landscape, using glColorMaterial
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glCallList(landscape_list);
	glDisable(GL_COLOR_MATERIAL);
}

GLvoid drawWater(GLvoid)
{
	//water, using transparency
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthMask(GL_FALSE);
	glEnable( GL_BLEND );
		glCallList(water_list);
	glDisable( GL_BLEND );
	glDepthMask(GL_TRUE);
}
