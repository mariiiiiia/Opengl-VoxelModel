#include <stdio.h>     // - Just for some ASCII messages
#include <array>
#include <vector>
#include <string.h>
#include <math.h>
#include "gl/glut.h"   // - An interface and windows management library
#include "structs.h"
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"
#include "Show_Obj.h"
#include "soil/SOIL.h"
#include "voxelModel.h"
#include "FreeFall.h"
 


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
float light1_x=0, light1_y=-1, light1_z=0,light2_x=0, light2_y=-1, light2_z=0;
float light_angle=0.03;

//------ normals -------------
std::vector<Vector> normal;      // normal per face
std::vector<Vector> vertNormal;  // normal per vertice

//---------- TEXTURES ------------------
static int horseTexture, hornTexture, handTexture, floorTexture;
static int backgroundTexture1, backgroundTexture2;

//---------- load object ------
static bool load_obj = true;
//---------- voxel model -------
static bool voxelize=false, test_voxel=false;
float voxel_width=0.5;
//--------- free fall ---------
static bool sphere_voxels=false, free_fall=false;
static float dt=0.02, g=1;
static Vector voxel_a;
static int voxel_quantity=0;
//---------- check collisions ------
float floor_coord_y=-20;


void Render()
{    
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  
  //----- select which object will be loaded and calculate its normals-------
 // if (obj_file == "objects/unicorn_low.obj" && load_obj==true){
  if (load_obj==true){
	  loadObj(obj_file, vertices, triangles, hornTriangles);
	  
	  normal.clear();
	  for (int i=0; i<int(triangles.size()); i++)
	  {
		  Vector n;

		  n = CalcNormal( triangles.at(i) );
		  normal.push_back( n );
	  }

	  avgNormals(triangles, vertices);
	  load_obj=false;
  }

  //-------draw background------
  setRoom();


  //---- bring object at the center of the window so we can see it -----------
  if (obj_file=="objects/unicorn_low.obj"){
	  glTranslatef(0,0,-40);
	  //glRotatef(90,0,1,0);
  }
  else {
	  glTranslatef(0,0,-40);
	  //glRotatef(-90,0,1,0);
  }
  //---------------------------------------------------------

  //---------- RENDER OBJECT ----------------
  glPushMatrix();
  if (voxelize==false && sphere_voxels==false && free_fall==false){
		  glTranslatef(tx,ty,tz);

		  glRotatef(rotx,1,0,0);
		  glRotatef(roty,0,1,0);
		  glRotatef(rotz,0,0,1);

		  glScalef( scalex, scaley, scalez);

		  if (obj_file=="objects/unicorn_low.obj"){
			  glColor4f(1,1,1,1.0);
			  glMateriali(GL_FRONT,GL_SHININESS,50);
			  showObj(vertices, triangles, solid, wireframe, vertNormal, horseTexture);
			  glColor4f(1,1,1,1.0);
			  glMateriali(GL_FRONT,GL_SHININESS,128);
			  showObj(vertices, hornTriangles, solid, wireframe, vertNormal, hornTexture);
		  }
		  else if (obj_file=="objects/hand.obj"){
			  glColor4f(1,1,1,1.0);
			  glMateriali(GL_FRONT,GL_SHININESS,60);
			  showObj(vertices, triangles, solid, wireframe, vertNormal, handTexture);
		  }
		  else {
			  glColor4f(1,1,1,1.0);
			  showObj(vertices, triangles, solid, wireframe, vertNormal, handTexture);		  
		  }
	  }
  glPopMatrix();

  //-------- draw spheres for light --------
  if (light1_state==true){
	  glPushMatrix();
		  glColor3f( 0.1, 0.8, 0.0);
		  glTranslatef(-8,5,4);
		  glutSolidSphere(0.5,20,20);
	  glPopMatrix();}

  if (light2_state==true){
	  glPushMatrix();
		  glColor3f( 1.0, 0.0, 0.0);
		  glTranslatef(8,5,4);
		  glutSolidSphere(0.5,20,20);
	  glPopMatrix();}

  //--------------------- VOXELIZE -----------------------
  if ((voxelize==true || sphere_voxels==true || free_fall==true) && test_voxel==false) {
	  setVoxels( voxels, vertices, triangles, normal, voxel_width);
	  if ( obj_file=="objects/unicorn_low.obj") setVoxels( voxels, vertices, hornTriangles, normal, voxel_width);
  }
  //-------------------- SHOW VOXEL MODEL -----------------
  if (voxelize==true){
	  glPushMatrix();
		  glTranslatef(tx,ty,tz);

		  glRotatef(rotx,1,0,0);
		  glRotatef(roty,0,1,0);
		  glRotatef(rotz,0,0,1);

		  glScalef( scalex, scaley, scalez);

		  if (test_voxel==false){
			  drawVoxel(voxels, voxel_width);
		  }
		  // test voxelization
		  else {
			  	glColor4f( 1,0,0.5, 1.0);
	
				Triangle triangle;
				triangle.p1 = 1;
				triangle.p2 = 3; 
				triangle.p3 = 2;

				std::vector<Triangle> test_triangle;
				test_triangle.push_back( triangle);

				Point p1,p2,p3;
				p1.insert( vertices.at(triangle.p1).x, vertices.at(triangle.p1).y, vertices.at(triangle.p1).z); 
				p2.insert( vertices.at(triangle.p2).x, vertices.at(triangle.p2).y, vertices.at(triangle.p2).z); 
				p3.insert( vertices.at(triangle.p3).x, vertices.at(triangle.p3).y, vertices.at(triangle.p3).z);
		
				glBegin( GL_TRIANGLES);
				glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p2.x,p2.y,p2.z); glVertex3f(p3.x, p3.y,p3.z);
				glEnd();
				glBegin( GL_TRIANGLES);
				glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p3.x,p3.y,p3.z); glVertex3f(p2.x, p2.y,p2.z);
				glEnd();

				Vector norm;
				std::vector<Vector> test_normal;
				norm = CalcNormal( triangle);
				test_normal.push_back( norm);
				
				setVoxels( voxels, vertices, test_triangle, test_normal, voxel_width);
				drawVoxel( voxels,voxel_width);
			}
      glPopMatrix();
  }

  //---------------- SHOW SPHERE VOXELS or LET THEM FALL -----------
  if (sphere_voxels==true || free_fall==true){
	  glPushMatrix();
	  glTranslatef(tx,ty,tz);

	  glRotatef(rotx,1,0,0);
	  glRotatef(roty,0,1,0);
	  glRotatef(rotz,0,0,1);

	  glScalef( scalex, scaley, scalez);

	  drawSphereVoxels( voxels, voxel_width);
	  glPopMatrix();
  }

  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Idle()
{
	if (free_fall==true){
		if (voxel_a.x==NULL && voxel_a.y==NULL && voxel_a.z==NULL) voxel_a.insert(0,-g,0);
		freeFallOfVoxels( voxels, voxelVelocity, voxel_a, dt);		

		//check collisions with floor
		for (int i=0; i<int(voxels.size());i++){
			if (voxels.at(i).y<=floor_coord_y){
				voxelVelocity.at(i).y= -voxelVelocity.at(i).y;
			}
		}
	}

	glutPostRedisplay();
}


Vector CalcNormal( Triangle triangle)
{
	Vector v1,v2;
	v1.x=vertices.at(triangle.p2).x-vertices.at(triangle.p1).x; 
	v1.y=vertices.at(triangle.p2).y-vertices.at(triangle.p1).y; 
	v1.z=vertices.at(triangle.p2).z-vertices.at(triangle.p1).z;

	v2.x=vertices.at(triangle.p3).x-vertices.at(triangle.p1).x; 
	v2.y=vertices.at(triangle.p3).y-vertices.at(triangle.p1).y; 
	v2.z=vertices.at(triangle.p3).z-vertices.at(triangle.p1).z;

	Vector normal;

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

void avgNormals(std::vector<Triangle> triangle,std::vector<Point> vertice)
{
	std::vector<int> cnt;

	vertNormal.clear();

	for (int i=0; i<int(vertice.size()); i++) {
		Vector zero;
		zero.x=0; zero.y=0; zero.z=0;

		vertNormal.push_back(zero);
		cnt.push_back(0);
	}
	
	for (int i=0; i<int(triangle.size());i++){
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

	for (int i=0; i<int(vertNormal.size()); i++){
		vertNormal.at(i).x= vertNormal.at(i).x/cnt.at(i);
		vertNormal.at(i).y= vertNormal.at(i).y/cnt.at(i);
		vertNormal.at(i).z= vertNormal.at(i).z/cnt.at(i);
	}
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

void setRoom()
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
		glTexCoord2f(0.0, 1.0); glVertex3f( 200, floor_coord_y, -200);
		glTexCoord2f(1.0, 1.0); glVertex3f( -200, floor_coord_y, -200);
		glTexCoord2f(1.0, 0.0); glVertex3f( -200, floor_coord_y, 0);
		glTexCoord2f(0.0, 0.0); glVertex3f( 200, floor_coord_y, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//load background texture happy_new_year
	if (backgroundTexture1==NULL){
	backgroundTexture1 = loadTexture("images/wall1.png");
	}
	
	// draw background
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture1);

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0, 0.0); glVertex3f( -200, -30, -200);
		glTexCoord2f(0.0, 0.0); glVertex3f( 200, -30, -200);
		glTexCoord2f(0.0, 1.0); glVertex3f( 200, 170, -200);
		glTexCoord2f(1.0, 1.0); glVertex3f( -200, 170, -200);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
		case 't': 
		{
			if (obj_file=="objects/unicorn_low.obj"){
				horseTexture = loadTexture("images/horse3.png");
				hornTexture = loadTexture("images/horn3.png");
			}
			else if (obj_file=="objects/hand.obj") handTexture = loadTexture("images/hand1.png");
			break;
		}
		case 'u': 
		{
			horseTexture = NULL;
			hornTexture = NULL;	
			handTexture = NULL;
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
		case '1': {obj_file = "objects/unicorn_low.obj"; load_obj=true; voxels.clear(); break;}
		case '2': {obj_file = "objects/hand.obj"; load_obj=true; voxels.clear(); break;}
		case '3': {obj_file = "objects/3D_1.obj"; load_obj=true; voxels.clear(); break;}
		case '4': {obj_file = "objects/3D_2.obj"; load_obj=true; voxels.clear(); break;}
		//------------ voxelize odr not -------------
		case 'D': {voxelize = true; test_voxel = true; voxels.clear(); break;}
		case 'd': {voxelize = true; test_voxel=false; free_fall=false; break;}
		case 'r': {sphere_voxels=true; voxelize=false; free_fall=false; break;}
		case 'f': 
			free_fall = true; voxelize = false; sphere_voxels = false; 
			initiateVelocities( voxels, voxelVelocity);
			break;
		case 'e': 
			voxelize = false; sphere_voxels=false; free_fall=false; 
			voxels.clear();
			break;
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
			light1_state = !light1_state; lightSources(); break;
		case LIGHT2_ON_OFF :
			light2_state = !light2_state; lightSources(); break;
	}
}

void lightSources()
{
	if (light1_state==false) glDisable(GL_LIGHT1);
	else if (light1_state==true){
		GLfloat light_position1[] = { -8,5, 4, 1.0 };
		GLfloat specularLight1[] = { 0.5, 1.0, 0.0, 1.0 };
		//GLfloat spotDir1[] = {0.0, 0.0, 0.0, 1.0};

		glLightfv( GL_LIGHT1, GL_POSITION, light_position1);		
		glLightfv( GL_LIGHT1, GL_SPECULAR, specularLight1 );

		//glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90.0);
		//glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,128.0f);
		//glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDir1);
		
		glEnable(GL_LIGHT1);
	}

	if (light2_state==false) glDisable(GL_LIGHT2);
	else if (light2_state==true){
		GLfloat light_position2[] = { 8,5,4, 1.0 };
		GLfloat specularLight2[] = { 1.0, 0.2, 0.1, 1.0 };
		//GLfloat spotDir2[] = {0.0, 0.0, 0.0};

		glLightfv( GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv( GL_LIGHT2, GL_SPECULAR, specularLight2 );
	
		glEnable(GL_LIGHT2);
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
	GLfloat light_position[] = { 0.0, 20.0, 8.0, 1.0 };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0 };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0 };

	glLightfv( GL_LIGHT0, GL_POSITION, light_position);
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	
	glEnable(GL_LIGHT0);

	// material identities
	float specReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambReflection[] = { 0.5, 0.5, 0.5, 1.0f };
	float diffReflection[] = { 0.5, 0.5, 0.5, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambReflection);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffReflection);

	// polygon rendering mode
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	/////
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