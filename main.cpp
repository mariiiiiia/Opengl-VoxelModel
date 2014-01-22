//--------------------------------------------------//
//													//	
//    Learning OpenGL: Εισαγωγικά Μαθήματα OpenGL   //
//													//
//    Πλατφόρμα ανάπτυξης παραδειγμάτων				//
//													//
//													//
//		Μάθημα 5ο									//
//--------------------------------------------------//


#include <stdio.h>     // - Just for some ASCII messages
#include <array>
#include <vector>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "structs.h"
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"




////////////////// State Variables ////////////////////////

std::vector<Point> vertices;
std::vector<Triangle> triangles;
std::vector<Voxel> voxels;
std::vector<Vector> voxelVelocity;

std::string obj_file;

GLuint texName;

/////////////// Main Program ///////////////////////////

int main(int argc, char* argv[])
{ 
  // initialize GLUT library state
  glutInit(&argc, argv);
	
  // Set up the display using the GLUT functions to 
  // get rid of the window setup details:
  // - Use true RGB colour mode ( and transparency )
  // - Enable double buffering for faster window update
  // - Allocate a Depth-Buffer in the system memory or 
  //   in the video memory if 3D acceleration available	
                      //RGBA//DEPTH BUFFER//DOUBLE BUFFER//
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
 
  
  // Define the main window size and initial position 
  // ( upper left corner, boundaries included )
  glutInitWindowSize(480,480);
  glutInitWindowPosition(50,50);
  
  // Create and label the main window
  glutCreateWindow("Course5");
  
  // Configure various properties of the OpenGL rendering context
  Setup();
  
  // Callbacks for the GL and GLUT events:

  obj_file="objects/unicorn_low.obj";
  // The rendering function 
  glutDisplayFunc(Render);
  glutReshapeFunc(Resize);
  glutIdleFunc(Idle);
  glutKeyboardFunc(Keyboard);
  glutKeyboardUpFunc(KeyboardUp);
  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);

  glutCreateMenu(MenuSelect);
  glutAddMenuEntry("Solid",SOLID);
  glutAddMenuEntry("Wireframe",WIREFRAME);
  glutAddMenuEntry("Light 1 (ON/OFF)", LIGHT1_ON_OFF);
  glutAddMenuEntry("Light 2 (ON/OFF)", LIGHT2_ON_OFF);
	
  // attach the menu to the right button
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  //Enter main event handling loop
  glutMainLoop();
  return 0;	
}  

