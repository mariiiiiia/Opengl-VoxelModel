#include <stdio.h>     // - Just for some ASCII messages
#include <array>
#include <vector>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "gl/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"
#include "Show_Obj.h"
#include "soil/SOIL.h"
 


static int left_button_state=0;
//-------------- variables for scaling -------------------
static float scalex = 1, scaley = 1, scalez = 1;
static bool scalex_state = true, scaley_state=true, scalez_state=true;
static float tempx=0, tempy=0, dx=0, dy=0;
static int click_event=0;
static int incr_scale = 0, decr_scale=0;
//-------------- variables for moving/rotatin ---------------
static float tx = 0.0, ty=0.0, tz=0.0;
static float rotx = 0.0, roty=0.0, rotz=0.0;

//------------- variables for object rendering and lights -----------
static bool solid=true, wireframe=false;
static bool light1_state = false, light2_state = false;
float light1_x=0, light1_y=0, light1_z=0,light2_x=0, light2_y=0, light2_z=0;
float light_angle=0.03;

static int render_object=0;    
std::vector<Point> normal;
std::vector<Point> vertNormal;


// ----------TEXTURES ------------------
static int horseTexture, hornTexture, floorTexture;
static int backgroundTexture1, backgroundTexture2;


void Render()
{    
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  
  //----- select which object will be loaded and calculate its normals-------
  if (render_object==0){
	  obj_file = "objects/unicorn_low.obj";
	  loadObj(obj_file, vertices, triangles, hornTriangles);
	  render_object=3;
	  
	  normal.clear();
	  for (int i=0; i<triangles.size(); i++)
	  {
		  Point n;

		  n = CalcNormal( triangles.at(i) );
		  normal.push_back( n );
	  }

	  avgNormals(triangles, vertices);
  }
  else if (render_object==1){
	  obj_file = "objects/hand.obj";
	  loadObj(obj_file, vertices, triangles, hornTriangles);
	  render_object=3;

	  normal.clear();
	  for (int i=0; i<triangles.size(); i++)
	  {
		  Point n;

		  n = CalcNormal( triangles.at(i));
		  normal.push_back(n);
	  }
	  avgNormals(triangles, vertices);
  }
  else if (render_object==2){
	  obj_file = "objects/unicorn.obj";
	  loadObj(obj_file, vertices, triangles, hornTriangles);
	  render_object=3;

	  normal.clear();
	  for (int i=0; i<triangles.size(); i++)
	  {
		  Point n;

		  n = CalcNormal( triangles.at(i));
		  normal.push_back(n);
	  }
	  avgNormals(triangles, vertices);
  }

  //-------draw background------
  drawBackground();
  drawFloor();


  //---- bring object at the center of the window show we can see it -----------
  if (obj_file=="objects/unicorn.obj"){
	  glTranslatef(0,0,-100);
	  glRotatef(90,0,1,0);
  }
  else if (obj_file=="objects/unicorn_low.obj"){
	  glTranslatef(0,0,-15);
	  glRotatef(90,0,1,0);
  }
  else if (obj_file=="objects/hand.obj"){
	  glTranslatef(0,0,-50);
  }
  //---------------------------------------------------------

  //---------- render object ----------------
  glPushMatrix();
	  glTranslatef(tx,ty,tz);

	  glRotatef(rotx,1,0,0);
	  glRotatef(roty,0,1,0);
	  glRotatef(rotz,0,0,1);

	  glScalef( scalex, scaley, scalez);

	  showObj(vertices, triangles, solid, wireframe, normal, horseTexture);
	  showObj(vertices, hornTriangles, solid, wireframe, normal, hornTexture);
  glPopMatrix();

	   // draw spheres for light
  if (light1_state==true){
	  glPushMatrix();
		  glColor3f(0.8,0.8,0.0);
		  glTranslatef( tx+light1_x, ty+light1_y, tz+light1_z);
		  printf(" %f  %f  %f\n", light1_x,light1_y,light1_z);
		  glutSolidSphere(1,20,20);
	  glPopMatrix();}

  if (light2_state==true){
	  glPushMatrix();
		  glColor3f(0.0, 0.4,0.4);
		  glTranslatef(tx+light2_x,ty+light2_y,tz+light2_z);
		  glutSolidSphere(1,20,20);
	  glPopMatrix();}

  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void avgNormals(std::vector<Triangle> triangle,std::vector<Point> vertice)
{
	std::vector<int> cnt;

	vertNormal.clear();

	for (int i=0; i<vertice.size(); i++) {
		Point zero;
		zero.x=0; zero.y=0; zero.z=0;

		vertNormal.push_back(zero);
		cnt.push_back(0);
	}
	
	for (int i=0; i<triangle.size();i++){
		vertNormal.at( triangle.at(i).p1).x += normal.at(i).x;
		vertNormal.at( triangle.at(i).p1).y += normal.at(i).y;
		vertNormal.at( triangle.at(i).p1).z += normal.at(i).z;

		cnt.at( triangle.at(i).p1)++;

		vertNormal.at( triangle.at(i).p2).x += normal.at(i).x;
		vertNormal.at( triangle.at(i).p2).y += normal.at(i).y;
		vertNormal.at( triangle.at(i).p2).z += normal.at(i).z;

		cnt.at( triangle.at(i).p2)++;

		vertNormal.at( triangle.at(i).p3).x += normal.at(i).x;
		vertNormal.at( triangle.at(i).p3).y += normal.at(i).y;
		vertNormal.at( triangle.at(i).p3).z += normal.at(i).z;

		cnt.at( triangle.at(i).p3)++;
	}

	for (int i=0; i<vertNormal.size(); i++){
		vertNormal.at(i).x= vertNormal.at(i).x/cnt.at(i);
		vertNormal.at(i).y= vertNormal.at(i).y/cnt.at(i);
		vertNormal.at(i).z= vertNormal.at(i).z/cnt.at(i);
	}
}

Point CalcNormal( Triangle triangle)
{
	Point v1,v2;
	v1.x=vertices.at(triangle.p2).x-vertices.at(triangle.p1).x; 
	v1.y=vertices.at(triangle.p2).y-vertices.at(triangle.p1).y; 
	v1.z=vertices.at(triangle.p2).z-vertices.at(triangle.p1).z;

	v2.x=vertices.at(triangle.p3).x-vertices.at(triangle.p1).x; 
	v2.y=vertices.at(triangle.p3).y-vertices.at(triangle.p1).y; 
	v2.z=vertices.at(triangle.p3).z-vertices.at(triangle.p1).z;

	Point normal;

	normal.x = v1.y*v2.z - v1.z*v2.y;		
	normal.y = -v1.x*v2.z + v2.x*v1.z;
	normal.z = v1.x*v2.y - v2.x*v1.y;

	float dist1 = sqrt( pow(v1.x,2) + pow(v1.y,2) + pow(v1.z,2));
	float dist2 = sqrt( pow(v2.x,2) + pow(v2.y,2) + pow(v2.z,2));
	float dist = dist1*dist2;

	normal.x = normal.x/dist;
	normal.y = normal.y/dist;
	normal.z = normal.z/dist;

	return normal;
}

int loadTexture(const char *filename)
{
	/* load an image file directly as a new OpenGL texture */
	GLuint myIm = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	
	/* check for an error during the load process */
	if( 0 == myIm )  printf( "texture: %s .SOIL loading error: '%s'\n", filename, SOIL_last_result() );

	glBindTexture(GL_TEXTURE_2D, myIm);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBindTexture(GL_TEXTURE_2D, 0);

	return myIm;
}

void drawBackground()
{
	//load background texture with snowman
	if (backgroundTexture1==NULL){
	backgroundTexture1 = loadTexture("images/snowman.png");
	}
	
	// draw background
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture1);

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0, 0.0); glVertex3f( 150, 60, -200);
		glTexCoord2f(0.0, 0.0); glVertex3f( 200, 60, -200);
		glTexCoord2f(0.0, 1.0); glVertex3f( 200, 100, -200);
		glTexCoord2f(1.0, 1.0); glVertex3f( 150, 100, -200);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//load background texture with rainbow
	if (backgroundTexture2==NULL){
	backgroundTexture2 = loadTexture("images/rainbow.png");
	}
	
	//// draw background with rainbow
	//glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glBindTexture(GL_TEXTURE_2D, backgroundTexture2);

	//glBegin(GL_QUADS);		
	//	glTexCoord2f(1.0, 0.0); glVertex3f( -250, -200, -300);
	//	glTexCoord2f(0.0, 0.0); glVertex3f( 250, -200, -300);
	//	glTexCoord2f(0.0, 1.0); glVertex3f( 250, 100, -300);
	//	glTexCoord2f(1.0, 1.0); glVertex3f( -250, 100, -300);
	//glEnd();

	//glDisable(GL_TEXTURE_2D);
}

void drawFloor()
{
	//load floor texture 
	if (floorTexture==NULL){
	floorTexture = loadTexture("images/floor.png");
	}
	
	// draw floor
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glBegin(GL_QUADS);		
		glTexCoord2f(0.0, 1.0); glVertex3f( 200, -60, -300);
		glTexCoord2f(1.0, 1.0); glVertex3f( -200, -60, -300);
		glTexCoord2f(1.0, 0.0); glVertex3f( -200, -60, -50);
		glTexCoord2f(0.0, 0.0); glVertex3f( 200, -60, -50);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
		case 't': 
		{
			if (horseTexture==NULL) 
			{
				horseTexture = loadTexture("images/horse1.png");
				hornTexture = loadTexture("images/horn1.png");
			}
			break;
		}
		case 'u': 
		{
			if (horseTexture!=NULL) 
			{
				horseTexture = NULL;
				hornTexture = NULL;				
			}
			break;
		}
		//---------- movement on x-y-z ------------------
		case 'q' : tx-=1.0f;
			break;
		case 'w': tx+=1.0f;
			break;
		case 'a' : ty-=1.0f;
			break;
		case 's': ty+=1.0f;
			break;
		case 'z' : tz-=1.0f;
			break;
		case 'x': tz+=1.0f;
			break;
		//---------- rotation --------------------------
		case 'o' : rotx-=2.0f;
			break;
		case 'p': rotx+=2.0f;
			break;
		case 'k' : roty-=2.0f;
			break;
		case 'l': roty+=2.0f;
			break;
		case 'n' : rotz-=2.0f;
			break;
		case 'm': rotz+=2.0f;
			break; 
		//---------- scaling ----------------------------------------------
		case 'c':
			scalex_state=true, scaley_state=false, scalez_state=false;
			break;
		case 'v':
			scaley_state=true, scalex_state=false, scalez_state=false;
			break;
		case 'b':
			scalez_state=true, scaley_state=false, scalex_state=false;
			break;
		//----------- select object to render -----------------------
		case '1': render_object = 0; break;
		case '2': render_object = 1; break;
		case '3': render_object = 2; break;
		default : break;
		}	

	glutPostRedisplay();
	
}

void KeyboardUp(unsigned char key,int x,int y)
{
	scalex_state=true, scaley_state=true, scalez_state=true;
}

void Mouse(int button,int state,int x,int y)
{
	 if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	 {
		 click_event = 1;
		 left_button_state = 1;		 
	 }
	 else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
	 {
		 left_button_state = 0;
	 }

}

void Motion(int x, int y)
{
	//----------- setup the values of scalex,scaley,scalez ---------------------//
	if (left_button_state==1)
	{		
		//printf("%f - %d\n", scalex, click_event);
		if (click_event == 0)
		{
			dx+=abs(x-tempx);	

			if (x>tempx && decr_scale==1) 
			{
				incr_scale=1, decr_scale=0;
				dx=0;
			}
			else if (x<tempx && incr_scale==1)
			{
				incr_scale=0, decr_scale=1;
				dx=0;
			}
			// increase scaling on x 
			else if (x>tempx && scalex_state==true && scaley_state==false && scalez_state==false)	  
			{
				scalex = 1+dx/1600;
				incr_scale=1;
				decr_scale=0;
			}
			// increase scaling on y
			else if (x>tempx && scaley_state==true && scalex_state==false && scalez_state==false)	  
			{
				scaley = 1+dx/1600;
				incr_scale=1;
				decr_scale=0;
			}
			// increase scaling on z
			else if (x>tempx && scalez_state==true && scaley_state==false && scalex_state==false)	  
			{
				scalez = 1+dx/1600;
				incr_scale=1;
				decr_scale=0;
			}
			// increase scaling on x-y-z
			else if (x>tempx && scaley_state==true && scalex_state==true && scalez_state==true)	  
			{
				scalex = 1+dx/1600, scaley = 1+dx/1600, scalez = 1+dx/1600;
				decr_scale=0;
				incr_scale=1;
			}
			// decrease scaling on x
			else if (x<tempx && scalex_state==true && scaley_state==false && scalez_state==false)	  
			{
				scalex = 1-dx/1600;
				decr_scale=1;
				incr_scale=0;
			}
			// decrease scaling on y
			else if (x<tempx && scaley_state==true && scalez_state==false && scalex_state==false)	  
			{
				scaley = 1-dx/1600;
				decr_scale=1;
				incr_scale=0;
			}
			// decrease scaling on z
			else if (x<tempx && scalez_state==true && scaley_state==false && scalex_state==false) 
			{
				scalez = 1-dx/1600;  
				decr_scale=1;
				incr_scale=0;
			}
			// decrease scaling on x-y-z
			else if (x<tempx && scaley_state==true && scalex_state==true && scalez_state==true)	  
			{
				scalex = 1-dx/1600, scaley = 1-dx/1600, scalez = 1-dx/1600;
				decr_scale=1;
				incr_scale=0;
			}

			tempx=x;
		}
		else if (click_event==1)
		{
			tempx=x;
			click_event=0;
			//scale_state=false;
		}


		glutPostRedisplay();

		
	}
}

void Idle()
{
	if (light1_state == true || light2_state == true ) 
	{
		if (light_angle<360) light_angle+=4;
		else light_angle=0.0; 

		light1_x = 10*cos(light_angle*3.14/180);
		light1_z = 15*sin(light_angle*3.14/180);	

		light2_x = -10*cos(light_angle*3.14/180);
		light2_z = -15*sin(light_angle*3.14/180);

		GLfloat light_position1[] = { light1_x, light1_y, light1_z, 1.0 };
		GLfloat light_position2[] = { light2_x, light2_y, light2_z, 1.0 };

		glLightfv( GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv( GL_LIGHT2, GL_POSITION, light_position2);

		GLfloat specularLight1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specularLight2[] = { 1.0, 1.0, 0.8, 1.0 };

		glLightfv( GL_LIGHT1, GL_SPECULAR, specularLight1 );
		glLightfv( GL_LIGHT2, GL_SPECULAR, specularLight2 );

		if (light1_state==true) glEnable(GL_LIGHT1);
		else if (light1_state==false) glDisable(GL_LIGHT1);

		if (light2_state==true) glEnable(GL_LIGHT2);
		else if (light2_state==false) glDisable(GL_LIGHT2);
	}
	else if (light1_state==false && light2_state==false ) glDisable(GL_LIGHT1), glDisable(GL_LIGHT2);

	glutPostRedisplay();
}

void MenuSelect(int choice)
{
	switch (choice) {
		case SOLID : 
			solid = true;
			wireframe = false; break;
		case WIREFRAME : 
			solid = false;
			wireframe = true; break;
		case LIGHT1_ON_OFF :
			light1_state = !light1_state; break;
		case LIGHT2_ON_OFF :
			light2_state = !light2_state; break;
	}
}

void Setup()  // TOUCH IT !! 
{ 
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel (GL_SMOOTH);
	//glEnable(GL_NORMALIZE); 
	
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
      
	//Set up light source
	GLfloat light_position[] = { 20.0, 0.0, 0.0, 0.0 };
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0 };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0 };
	
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );

	
	glEnable(GL_LIGHT0);

	// polygon rendering mode
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	// material identities
	float specReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	float ambReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	float diffReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, ambReflection);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffReflection);
	glMateriali(GL_FRONT,GL_SHININESS,60);


	//// about texture
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void Resize(int w, int h)
{ 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}