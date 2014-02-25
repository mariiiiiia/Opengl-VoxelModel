#define SOLID 1
#define WIREFRAME 2
#define LIGHT1_ON_OFF 3
#define LIGHT2_ON_OFF 4

extern std::vector<Point> vertices;
extern std::vector<Triangle> triangles;
extern VoxelModel voxel_model;
//extern std::vector<Vector> voxelVelocity;
extern std::string obj_file;
//extern std::vector< std::vector< std::vector<Point> >> buckets;
static std::vector< Triangle> newTriangles;
static std::vector< Point> newVertices;

//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void Keyboard(unsigned char key,int x,int y);
// Function for handling keyboard events.

void KeyboardUp(unsigned char key,int x,int y);
// Function for handling key-release events.

void Mouse(int button,int state,int x,int y); 
// Function for handling mouse events

void Motion(int x, int y);

void MenuSelect(int choice);

void lightSources();

int loadTexture(const char *filename);

void setRoom();
