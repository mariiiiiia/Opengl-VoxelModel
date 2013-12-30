#define SOLID 1
#define WIREFRAME 2
#define LIGHT1_ON_OFF 3
#define LIGHT2_ON_OFF 4


class Point{
	public:
		float x;
		float y;
		float z;
};

class Triangle {
	public:
		int p1;  // pointers to vertices
		int p2;
		int p3;
};

extern std::vector<Point> vertices;
extern std::vector<Triangle> triangles;
extern std::vector<Triangle> hornTriangles;

extern std::string obj_file;
//static std::string obj_file;

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

Point CalcNormal( Triangle triangle);

int loadTexture(const char *filename);

void drawBackground();

void drawFloor();

void avgNormals(std::vector<Triangle> triangle, std::vector<Point> vertice);