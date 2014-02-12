#include <stdio.h>     // - Just for some ASCII messages
#include <vector>
#include <math.h>
#include "gl/glut.h"   // - An interface and windows management library
#include "structs.h"
#include "math3.h"
#include "voxelModel.h"
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"
#include "Show_Obj.h"
#include "soil/SOIL.h"
#include "FreeFall.h"
#include "triangulateVoxelModel.h"
#include "counter.h"


static std::vector<Triangle> hornTriangles;
//------ normals -------------
static std::vector<Vector> normal;      // normal per face
static std::vector<Vector> vertNormal;  // normal per vertice
//------- mouse ---------
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
static float light1_x=0, light1_y=-1, light1_z=0,light2_x=0, light2_y=-1, light2_z=0;
static float light_angle=0.03;
//---------- TEXTURES ------------------
static int horseTexture, hornTexture, handTexture, floorTexture;
static int backgroundTexture1, backgroundTexture2;
//---------- load object ------
static bool load_obj = true;
//---------- voxel model -------
static bool voxel_model_state=false, test_voxel=false;
//--------- free fall ---------
static bool sphere_voxels=false, free_fall=false;
static float dt=0.01;
static bool rewind_free_fall=false;
static std::vector< std::vector<Voxel>> voxel_data;
//--------- retriangulate mesh -----
static bool retriangulate=false;

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
		for (int i=0; i<int(triangles.size()); i++){
			Vector n;

			n = CalcTriangleNormal( vertices, triangles.at(i) );
			normal.push_back( n );
		}

		avgNormals(vertNormal, normal, triangles, int(vertices.size()));
		load_obj=false;
	}
	//-------draw background------
	//setRoom();

	//---- bring object at the center of the window so we can see it -----------
	glTranslatef(0,0,-20);
	//------- translations - rotations - scaling ---------
	glPushMatrix();

	glTranslatef(tx,ty,tz);

	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);
	glRotatef(rotz,0,0,1);
		
	glScalef( scalex, scaley, scalez);
	//---------- RENDER OBJECT ----------------
	if ( !sphere_voxels && !free_fall && !rewind_free_fall && !retriangulate){
		if (obj_file=="objects/unicorn_low.obj"){
			glColor4f(1,1,1,1.0);
			glMateriali(GL_FRONT,GL_SHININESS,51.2);
			showObj(vertices, triangles, solid, wireframe, vertNormal, horseTexture);
			glColor4f(1,1,1,1.0);
			glMateriali(GL_FRONT,GL_SHININESS,10);
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
	//======================= VOXEL MODEL =======================================
	//--------------- set voxels -----------------------------------------------
	if ((voxel_model_state==true || sphere_voxels==true || free_fall==true) && test_voxel==false) {
		voxel_model.setVoxels( vertices, triangles, normal);	}
	//-------------------- SHOW VOXEL MODEL -----------------
	if (voxel_model_state && !retriangulate){
		if (test_voxel==false){
			voxel_model.drawVoxel();
			//drawVoxel( voxels.at(0).width());
		}
		// test voxelization
		else {
			glColor4f( 1,0,0.5, 1.0);
	
			triangles.clear();
			hornTriangles.clear();
			Triangle test_triangle;
			test_triangle.p1 = 1;
			test_triangle.p2 = 3; 
			test_triangle.p3 = 2;
			triangles.push_back( test_triangle);

			Point p1,p2,p3;
			p1.insert( vertices.at(test_triangle.p1).x, vertices.at(test_triangle.p1).y, vertices.at(test_triangle.p1).z); 
			p2.insert( vertices.at(test_triangle.p2).x, vertices.at(test_triangle.p2).y, vertices.at(test_triangle.p2).z); 
			p3.insert( vertices.at(test_triangle.p3).x, vertices.at(test_triangle.p3).y, vertices.at(test_triangle.p3).z);
		
			glBegin( GL_TRIANGLES);
			glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p2.x,p2.y,p2.z); glVertex3f(p3.x, p3.y,p3.z);
			glEnd();
			glBegin( GL_TRIANGLES);
			glVertex3f( p1.x,p1.y,p1.z); glVertex3f( p3.x,p3.y,p3.z); glVertex3f(p2.x, p2.y,p2.z);
			glEnd();

			Vector norm;
			std::vector<Vector> test_normal;
			norm = CalcTriangleNormal( vertices, test_triangle);
			test_normal.push_back( norm);
			
			voxel_model.setVoxels( vertices, triangles, test_normal);
			voxel_model.drawVoxel();
		}
	}
	//--------------- FREE FALL -------------------
	if (free_fall){		
		//StartCounter();
		// check floor collisions
		checkFloorCollisions( voxel_model.voxels, tx,ty,tz, dt);	

		//check collisions with each other
		checkVoxelCollisions( voxel_model.voxels);
		//checkVoxelCollisionsInBuckets( voxels, voxelVelocity);

		//simulate free falling
		freeFallOfVoxels( voxel_model.voxels, voxel_data, dt);

	}
	//---------------- JUST SHOW SPHERE VOXELS or LET THEM FALL FREE -----------
	if (sphere_voxels || free_fall){
		drawSphereVoxels( voxel_model.voxels );
		
		//printf("total delay %f\n", GetCounter());
	}
	//----------------- REWIND FREE FALLING -----------------
	else if (rewind_free_fall){
		if (voxel_data.size()>1) {
			drawSphereVoxels( voxel_data.back());
			voxel_data.pop_back();
		}
		else {
			drawSphereVoxels( voxel_data.back());
			voxel_data.pop_back();
			voxel_model.voxels.clear();
			rewind_free_fall = !rewind_free_fall;
			sphere_voxels = true;
			//initiateVelocities( voxels, voxelVelocity);
		}
	}
	//============= RETRIANGULATE MESH THROUGH VOXEL DATA =========
	if (retriangulate)	{  
		//StartCounter();
		marchingCubes( voxel_model, vertices, newTriangles);
		//printf("cnt=%f\n", GetCounter());
	}
	glPopMatrix();

	//-------- draw spheres for light --------
	if (light1_state==true){
		glPushMatrix();
		glColor3f( 0.1, 0.8, 0.0);
		glTranslatef(-3,5,30);
		glutSolidSphere(0.5,20,20);
		glPopMatrix();}

	if (light2_state==true){
		glPushMatrix();
		glColor3f( 1.0, 0.0, 0.0);
		glTranslatef(3,5,30);
		glutSolidSphere(0.5,20,20);
		glPopMatrix();}

	
	glutSwapBuffers();             // All drawing commands applied to the 
									 // hidden buffer, so now, bring forward
									 // the hidden buffer and hide the visible one
}

void Idle()
{
	glutPostRedisplay();
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
		glTexCoord2f(0.0, 1.0); glVertex3f( RIGHT_WALL, FLOOR, FAR_WALL);
		glTexCoord2f(1.0, 1.0); glVertex3f( LEFT_WALL, FLOOR, FAR_WALL);
		glTexCoord2f(1.0, 0.0); glVertex3f( LEFT_WALL, FLOOR, NEAR_WALL);
		glTexCoord2f(0.0, 0.0); glVertex3f( RIGHT_WALL, FLOOR, NEAR_WALL);
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
		glTexCoord2f(1.0, 0.0); glVertex3f( LEFT_WALL, FLOOR, FAR_WALL);
		glTexCoord2f(0.0, 0.0); glVertex3f( RIGHT_WALL, FLOOR, FAR_WALL);
		glTexCoord2f(0.0, 1.0); glVertex3f( RIGHT_WALL, FLOOR+70, FAR_WALL);
		glTexCoord2f(1.0, 1.0); glVertex3f( LEFT_WALL, FLOOR+70, FAR_WALL);
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
				hornTexture = loadTexture("images/horn2.png");
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
		case '1': {obj_file = "objects/unicorn_low.obj"; load_obj=true; voxel_model.voxels.clear(); newTriangles.clear(); break;}
		case '2': {obj_file = "objects/hand.obj"; load_obj=true; voxel_model.voxels.clear(); newTriangles.clear(); break;}
		case '3': {obj_file = "objects/3D_1.obj"; load_obj=true; voxel_model.voxels.clear(); newTriangles.clear(); break;}
		case '4': {obj_file = "objects/3D_2.obj"; load_obj=true; voxel_model.voxels.clear(); newTriangles.clear(); break;}
		case '5': {obj_file = "objects/head.obj"; load_obj=true; voxel_model.voxels.clear(); newTriangles.clear(); break;}
		//------------ voxelize odr not -------------
		case 'D': 
			voxel_model_state = true; test_voxel = true; voxel_model.voxels.clear(); 
			initiateVelocities( voxel_model.voxels);
			break;
		case 'd': 
			voxel_model_state = true; test_voxel=false; free_fall=false;
			voxel_model.voxels.clear();
			initiateVelocities( voxel_model.voxels);
			break;
		case 'r': 
			sphere_voxels=true; voxel_model_state=false; free_fall=false; 
			voxel_model.voxels.clear();
			initiateVelocities( voxel_model.voxels);
			break;
		case 'f': 
			if ( sphere_voxels){
				free_fall = true; rewind_free_fall=false; voxel_model_state = false; sphere_voxels = false; 
				initiateVelocities( voxel_model.voxels);
			}
			break;
		case 'e': 
			voxel_model_state = false; sphere_voxels=false; free_fall=false; 
			voxel_model.voxels.clear();
			initiateVelocities( voxel_model.voxels);
			break;
		//====== rewind =======
		case 32:
			if ( free_fall){
				rewind_free_fall = true; free_fall=false; voxel_model_state = false; sphere_voxels=false;
			}
			break;
		//======= triangulate voxel model =============
		case 'j': 
			if (voxel_model_state){
				retriangulate = true; sphere_voxels=false; free_fall=false;}
			break;
		case 'i':
			retriangulate = false;
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
		//GLfloat light_position1[] = { -8, 5, -10, 1.0 };
		GLfloat light_position1[] = { 0, 0, -30, 1.0 };
		GLfloat specularLight1[] = { 0.3, 1.0, 0.0, 1.0 };
		//GLfloat spotDir1[] = { 0, 0, 0, 1.0};

		glLightfv( GL_LIGHT1, GL_POSITION, light_position1);		
		glLightfv( GL_LIGHT1, GL_SPECULAR, specularLight1 );

		//glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90.0);
		//glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,128.0f);
		//glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDir1);
		
		glEnable(GL_LIGHT1);
	}

	if (light2_state==false) glDisable(GL_LIGHT2);
	else if (light2_state==true){
		//GLfloat light_position2[] = { 8, 5, -10, 1.0 };
		GLfloat light_position2[] = { 0, 0, 40, 1.0 };
		GLfloat specularLight2[] = { 1.0, 0.2, 0.0, 1.0 };
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
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0 };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0 };

	glLightfv( GL_LIGHT0, GL_POSITION, light_position);
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	
	glEnable(GL_LIGHT0);

	// material identities
	//float specReflection[] = { 1,1,1, 1.0f };
	float specReflection[] = { 0.581f, 0.223f, 0.070f, 1.0f };
	float ambReflection[] = { 0.23, 0.089, 0.028, 1.0f };
	float diffReflection[] = { 0.551, 0.212, 0.066, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambReflection);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffReflection);

	// polygon rendering mode
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	
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