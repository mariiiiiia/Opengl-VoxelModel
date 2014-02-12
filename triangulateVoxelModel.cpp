#include <vector>
#include "gl\glut.h"
#include <math.h>
#include "structs.h"
#include "math3.h"
#include "voxelModel.h"
#include "triangulateVoxelModel.h"
#include "counter.h"

//static std::vector< Triangle> newTriangles;
static std::vector< Point> newVertices;
static std::vector< Vector> newVertNormals;
static std::vector< Vector> newNormals;


int edgeTable[256]={
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };

int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};


void marchingCubes( VoxelModel &voxmod, std::vector< Point> vertices, std::vector<Triangle> &newTriangles){
	//std::vector< MarchingCube> mc = voxmod.mc;
	//std::vector< Voxel> voxels = voxmod.voxels;
	std::vector< Point> vert_list;
	Point null;
	null.insert( 100,100,100);
	for (int i=0; i<12; i++){
		vert_list.push_back( null);
	}
	std::vector<Point> cube;

	if (newTriangles.empty() ){
		newVertices.clear();
		newNormals.clear();
		newVertNormals.clear();

		int size = int(voxmod.mc.size());
		for (int i=0; i<int(voxmod.mc.size()); i++){
			cube.clear();
			Point cb;
			float d=VOXEL_WIDTH;
			cb.insert( voxmod.mc.at(i).x-d/2, voxmod.mc.at(i).y-d/2, voxmod.mc.at(i).z+d/2);								//cube0
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).y-d/2, voxmod.mc.at(i).z+d/2);				//cube1
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).y-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).z+d/2);	//cube2
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2, voxmod.mc.at(i).y-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).z+d/2);				//cube3
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2, voxmod.mc.at(i).y-d/2, voxmod.mc.at(i).z+d/2- VOXEL_WIDTH);				//cube4
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).y-d/2, voxmod.mc.at(i).z+d/2- VOXEL_WIDTH);	//cube5
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).y-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).z+d/2- VOXEL_WIDTH);  //cube6
			cube.push_back( cb);
			cb.insert( voxmod.mc.at(i).x-d/2, voxmod.mc.at(i).y-d/2+ VOXEL_WIDTH, voxmod.mc.at(i).z+d/2- VOXEL_WIDTH);	//cube7
			cube.push_back( cb);

			int corner_index = constructCornerIndex( voxmod.mc.at(i));
			
			int edges;
			edges = edgeTable[ corner_index];

			if (corner_index==0) printf(" corner_index=0. something wrong with voxel model.\n");
			
			// update vertlist
			listMCIntersectionPoints( edges, vertices, voxmod.mc.at(i).tr, cube, vert_list);
			

			/* Create the triangle */
			for (int j=0; triTable[corner_index][j]!=-1; j+=3) {
				Point v1,v2,v3;
				Triangle tri;
				v1 = vert_list.at( triTable[corner_index][j  ]);
				v2 = vert_list.at( triTable[corner_index][j+1]);
				v3 = vert_list.at( triTable[corner_index][j+2]);

				if (!v1.equals( null) && \
					!v2.equals( null) && \
					!v3.equals( null) ){
					
					ccwTriangle(v1, v2, v3);

					int pointerToVertice;
					if ( findPointInVertices( v1, newVertices, pointerToVertice)){
						// the point is already in the newVertices list, so just set the triangle pointer
						tri.p1 = pointerToVertice;}
					else {
						// we have new point. Set the newVertices and the pointer
						newVertices.push_back( v1);
						tri.p1 = int(newVertices.size())-1;}
		
					if ( findPointInVertices( v2, newVertices, pointerToVertice)){
						// the point is already in the newVertices list, so just set the triangle pointer
						tri.p2 = pointerToVertice;}
					else {
						// we have new point. Set the newVertices and the pointer
						newVertices.push_back( v2);
						tri.p2 = int(newVertices.size())-1;}

					if ( findPointInVertices( v3, newVertices, pointerToVertice)){
						// the point is already in the newVertices list, so just set the triangle pointer
						tri.p3 = pointerToVertice;}
					else {
						// we have new point. Set the newVertices and the pointer
						newVertices.push_back( v3);
						tri.p3 = int(newVertices.size())-1;}

					newTriangles.push_back( tri );					
				}
			}
		}

		for (int i=0; i<int(newTriangles.size()); i++){
			Vector normal = CalcTriangleNormal( newVertices, newTriangles.at(i));
			newNormals.push_back( normal);
		}

		avgNormals( newVertNormals, newNormals, newTriangles, int(newVertices.size()));
	}
	

	for (int i=0; i<int(newTriangles.size()); i++)
	{
		glColor3f( 0.4, 0.7, 0.2);

		glBegin(GL_TRIANGLES);
			glNormal3f( newVertNormals.at( newTriangles.at(i).p1).x, newVertNormals.at(newTriangles.at(i).p1).y, newVertNormals.at(newTriangles.at(i).p1).z);
			glVertex3f( newVertices.at( newTriangles.at(i).p1).x, newVertices.at(newTriangles.at(i).p1).y, newVertices.at(newTriangles.at(i).p1).z);
			
			glNormal3f( newVertNormals.at( newTriangles.at(i).p2).x, newVertNormals.at(newTriangles.at(i).p2).y, newVertNormals.at(newTriangles.at(i).p2).z);
			glVertex3f( newVertices.at( newTriangles.at(i).p2).x, newVertices.at(newTriangles.at(i).p2).y, newVertices.at(newTriangles.at(i).p2).z);
			
			glNormal3f( newVertNormals.at( newTriangles.at(i).p3).x, newVertNormals.at(newTriangles.at(i).p3).y, newVertNormals.at(newTriangles.at(i).p3).z);
			glVertex3f( newVertices.at( newTriangles.at(i).p3).x, newVertices.at(newTriangles.at(i).p3).y, newVertices.at(newTriangles.at(i).p3).z);
		glEnd();
	}


}

void ccwTriangle( Point &v1, Point &v2, Point &v3){
	Vector v1v2, v2v3, v3v1;
	v1v2.insert( v2.x-v1.x, v2.y-v1.y, v2.z-v1.z);
	v2v3.insert( v3.x-v2.x, v3.y-v2.y, v3.z-v2.z);
	v3v1.insert( v1.x-v3.x, v1.y-v3.y, v1.z-v3.z);
	
	Vector n1,n2,n3;
	n1 = v1v2.crossProduct( v2v3);
	n2 = v2v3.crossProduct( v3v1);
	n3 = v3v1.crossProduct( v1v2);

	Vector zero,temp;
	zero.insert(0,0,0);
	if (((n1+n2)+n3)>zero){
		temp.insert( v2.x, v2.y, v2.z);
		v2.insert( v3.x, v3.y, v3.z);
		v3.insert( temp.x, temp.y, temp.z);
	}

}

int constructCornerIndex( MarchingCube mc){
	int corner_index=0;

	//for (int k=0; k<int(mc.at(i).corner_states.size()); k++){
	//	if (mc.at(i).corner_states.at(k)){
	//		corner_index += pow(2.0, k);
	//	}
	//}

	if (mc.corner_states.at(0)) corner_index |= 1;
	if (mc.corner_states.at(1)) corner_index |= 2;
	if (mc.corner_states.at(2)) corner_index |= 4;
	if (mc.corner_states.at(3)) corner_index |= 8;
	if (mc.corner_states.at(4)) corner_index |= 16;
	if (mc.corner_states.at(5)) corner_index |= 32;
	if (mc.corner_states.at(6)) corner_index |= 64;
	if (mc.corner_states.at(7)) corner_index |= 128;

	return corner_index;
}

void listMCIntersectionPoints( int edges, std::vector< Point> vertices, std::vector< Triangle> triangles, std::vector< Point> cube, std::vector< Point> &vert_list){
	//initiate flag
	bool intersp_found = false;	
	Point null;
	null.insert( 100,100,100);
	// clear vertlist
	for (int g=0; g<12; g++){
		vert_list.at(g) = null;}

	Point vert;
	TriangleCoord tri;
	for (int n=0; n<int(triangles.size()); n++){
		tri.set( vertices.at(triangles.at(n).p1), vertices.at(triangles.at(n).p2), vertices.at(triangles.at(n).p3));

		if (edges & 1) {
			vert = checkLineTriangleIntersection( tri, cube.at(0), cube.at(1), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(0), cube.at(1), intersp_found);		//edge0(
			if (intersp_found) vert_list.at( 0) = vert;
		}
		if (edges & 2) {
			vert = checkLineTriangleIntersection( tri, cube.at(1), cube.at(2), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(1), cube.at(2), intersp_found);		//edge1
			if (intersp_found) vert_list.at(1) = vert;
		}
		if (edges & 4) {
			vert = checkLineTriangleIntersection( tri, cube.at(2), cube.at(3), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(2), cube.at(3), intersp_found);		//edge2
			if (intersp_found) vert_list.at(2) = vert;
		}
		if (edges & 8) {
			vert = checkLineTriangleIntersection( tri, cube.at(3), cube.at(0), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(3), cube.at(0), intersp_found);		//edge3	
			if (intersp_found) vert_list.at(3) = vert;
		}
		if (edges & 16) {
			vert = checkLineTriangleIntersection( tri, cube.at(4), cube.at(5), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(4), cube.at(5), intersp_found);		//edge4
			if (intersp_found) vert_list.at(4) = vert;
		}
		if (edges & 32) {
			vert = checkLineTriangleIntersection( tri, cube.at(5), cube.at(6), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(5), cube.at(6), intersp_found);		//edge5
			if (intersp_found) vert_list.at(5) = vert;
		}
		if (edges & 64) {
			vert = checkLineTriangleIntersection( tri, cube.at(6), cube.at(7), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(6), cube.at(7), intersp_found);		//edge6
			if (intersp_found) vert_list.at(6) = vert;
		}
		if (edges & 128) {
			vert = checkLineTriangleIntersection( tri, cube.at(7), cube.at(4), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(7), cube.at(4), intersp_found);		//edge7
			if (intersp_found) vert_list.at(7) = vert;
		}
		if (edges & 256) {
			vert = checkLineTriangleIntersection( tri, cube.at(0), cube.at(4), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(0), cube.at(4), intersp_found);		//edge8
			if (intersp_found) vert_list.at(8) = vert;
		}
		if (edges & 512) {
			vert = checkLineTriangleIntersection( tri, cube.at(1), cube.at(5), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(1), cube.at(5), intersp_found);		//edge9
			if (intersp_found) vert_list.at(9) = vert;
		}
		if (edges & 1024) {
			vert = checkLineTriangleIntersection( tri, cube.at(2), cube.at(6), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(2), cube.at(6), intersp_found);		//edge10
			if (intersp_found) vert_list.at(10) = vert;
		}
		if (edges & 2048)	{
			vert = checkLineTriangleIntersection( tri, cube.at(3), cube.at(7), intersp_found);		//edge0
			//vert = rayTriangleIntersection( tri, cube.at(3), cube.at(7), intersp_found);		//edge11
			if (intersp_found) vert_list.at(11) = vert;
		}
	}

	//for (int n=0; n<int(triangles.size()); n++){
	//	tri.set( vertices.at(triangles.at(n).p1), vertices.at(triangles.at(n).p2), vertices.at(triangles.at(n).p3));

	//	if (edges & 1) {
	//		if (vert_list.at(0).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 2) {
	//		if (vert_list.at(1).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 4) {
	//		if (vert_list.at(2).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 8) {
	//		if (vert_list.at(3).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 16) {
	//		if (vert_list.at(4).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 32) {
	//		if (vert_list.at(5).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 64) {
	//		if (vert_list.at(6).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 128) {
	//		if (vert_list.at(7).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 256) {
	//		if (vert_list.at(8).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 512) {
	//		if (vert_list.at(9).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 1024) {
	//		if (vert_list.at(10).equals( null)) printf("not found\n");
	//	}
	//	if (edges & 2048) {
	//		if (vert_list.at(11).equals( null)) printf("not found\n");
	//	}
	//}
}

bool findPointInVertices( Point v, std::vector< Point> vertices, int &pointerToVertice){
	for (int i=0; i<int(vertices.size()); i++){
		if ( vertices.at(i).equals( v)){
			pointerToVertice = i;
			return true;
		}
	}
	return false;
}

Point rayTriangleIntersection( TriangleCoord T, Point c1, Point c2, bool &inters_found){
	Vector u,v,n;		//triangle vectors
	Vector dir, w0, w;	// ray vectors
	float r, a, b;		//params to calc intersections point

	// get triangle edge vectors and plane normal
	u.insert(T.p2.x - T.p1.x, T.p2.y - T.p1.y, T.p2.z - T.p1.z);
	v.insert(T.p3.x - T.p1.x, T.p3.y - T.p1.y, T.p3.z - T.p1.z);
	n = u.crossProduct(v);
	
	dir.insert( c2.x - c1.x, c2.y - c1.y, c2.z - c1.z);
	w0.insert( c1.x - T.p1.x, c1.y - T.p1.y, c1.z - T.p1.z);
	a = -n.dotproduct(dir);
	b = n.dotproduct(dir);
	//if (abs(b)<0.000000001) {
	//	printf( "div by zero\n");
	//	inters_found = false;
	//	return c1;
	//}

	// get intersect point of ray with triangle plane
	r = a/b;
	//if ( r<-1.k0){
	//	inters_found = false;
	//	return c1;}
	//else if (r>1.0){
	//	inters_found = false;
	//	return c1;}
	
	//float normDotRayDir = n.dotproduct( dir);
	//// calculate D
	//float d = n.dotproduct(T.p1);
	//// line parametric form c1+line_dir*t 
	//// compute t
	//r= (d - n.dotproduct(c1))/ normDotRayDir;


	Point intersp;
	intersp.insert( c1.x + r*dir.x,c1.y + r*dir.y,c1.z + r*dir.z);

	//is intersp inside T?
	float uu, uv, vv, wu, wv, D;
	uu = u.dotproduct(u);
    uv = u.dotproduct(v);
	vv = v.dotproduct(v);
	w.insert( intersp.x - T.p1.x,intersp.y - T.p1.y,intersp.z - T.p1.z);
	wu = w.dotproduct(u);
	wv = w.dotproduct(v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
  //  float s, t;
  //  s = (uv * wv - vv * wu) / D;
  //  if (s < -0.5 || s > 1.5) {       // I is outside T
		//inters_found=false;
		//return c1;}
  //  t = (uv * wu - uu * wv) / D;
  //  if (t < -0.5 || (s + t) > 1.5){	// I is outside T
  //      inters_found=false;
		//return c1;}

	inters_found = true;
    return intersp;                // I is in T

}

Point checkLineTriangleIntersection( TriangleCoord tri, Point lp1, Point lp2, bool &intersp_found){
	Point tp1, tp2, tp3;
	tp1.insert(tri.p1);
	tp2.insert(tri.p2);
	tp3.insert(tri.p3);

	// plane equation: Ax+By+Cz=D
	// compute triangle vectors
	Vector ab,bc,ca,ac, line_dir, normal;
	ab.insert(tp2.x-tp1.x, tp2.y-tp1.y, tp2.z-tp1.z);  // vector tp1->tp2
	bc.insert(tp3.x-tp2.x, tp3.y-tp2.y, tp3.z-tp2.z);  // vector tp2->tp3
	ca.insert(tp1.x-tp3.x, tp1.y-tp3.y, tp1.z-tp3.z);  // vector tp3->tp1
	ac.insert(tp3.x-tp1.x, tp3.y-tp1.y, tp3.z-tp1.z);  // vector tp1->tp3
	// calculate NOT normalized normal
	//normal = ab.crossProduct(ac);
	// calculate line direction 
	line_dir.insert( lp2.x-lp1.x, \
					 lp2.y-lp1.y, \
					 lp2.z-lp1.z);
	//float dist =  sqrt( pow(line_dir.x,2)+ pow(line_dir.y,2)+ pow(line_dir.z,2));
	//line_dir.insert( line_dir.x/dist, line_dir.y/ dist, line_dir.z/ dist);
	// check if line and triangle plane are parallel
	float NDotRayDir = normal.dotproduct( line_dir);
	if (NDotRayDir==0) { intersp_found=false; return lp1;}	// line and triangle plane are parallel!
	// calculate D
	float D = -normal.dotproduct(tp1);
	// line parametric form p+n*t,where p=(p.x,p.y,p.z) and n=triangle normal
	// compute t
	float t= -( D + normal.dotproduct(lp1))/ NDotRayDir;
	if (t<0) { intersp_found=false; return lp1;}	
	// compute the intersection point 
	Point interp;
	interp.insert( lp1.x+t*line_dir.x, \
				   lp1.y+t*line_dir.y, \
				   lp1.z+t*line_dir.z);
	// check if the intersection point interp is inside or outside the triangle
	// that means we check if the point is at the right side of the triangle vectors 
	// that's legal if triangles are counter-clockwise
	//------------------------------------------------------
	//Vector ap,bp,cp;
	//ap.insert( interp.x-tp1.x, \
	//		   interp.y-tp1.y, \
	//		   interp.z-tp1.z);     //vector tp1->interp
	//Vector n1;
	//n1 = ab.crossProduct(ap);
	//float dotpr0 = normal.dotproduct(n1);
	//if (dotpr0<0) { intersp_found=false; return lp1;}	 // interp is on the right side 

	//bp.insert( interp.x-tp2.x, \
	//		   interp.y-tp2.y, \
	//		   interp.z-tp2.z);
	//Vector n2;
	//n2 = bc.crossProduct(bp);
	//float dotpr1 = normal.dotproduct(n2);
	//if (dotpr1<0) { intersp_found=false; return lp1;}	  // interp is on the right side
	//	
	//cp.insert( interp.x-tp3.x, \
	//		   interp.y-tp3.y, \
	//		   interp.z-tp3.z);
	//Vector n3;
	//n3 = ca.crossProduct(cp);
	//float dotpr2 = normal.dotproduct(n3);
	//if (dotpr2<0) { intersp_found=false; return lp1;}	  // interp is on the right side


	if (line_dir.y!=0){
		if ( (interp.y<=lp1.y && interp.y>=lp2.y) || (interp.y>=lp1.y && interp.y<=lp2.y) ) { intersp_found=true; return interp;}	
		else { intersp_found=false; return lp1;}		
	}
	if (line_dir.z!=0){
		if ( (interp.z<=lp1.z && interp.z>=lp2.z) || (interp.z>=lp1.z && interp.z<=lp2.z) ) { intersp_found=true; return interp;}
		else { intersp_found=false; return lp1;}			
	}
	if (line_dir.x!=0){
		if ( (interp.x<=lp1.x && interp.x>=lp2.x) || (interp.x>=lp1.x && interp.x<=lp2.x) ) { intersp_found=true; return interp;}
		else { intersp_found=false; return lp1;}			
	}

	//glColor3f( 1, 0.3,1);
	//glBegin( GL_POLYGON);
	//glVertex3f( interp.x, interp.y, interp.z);
	//glVertex3f( interp.x+0.1, interp.y, interp.z);
	//glVertex3f( interp.x, interp.y+0.1, interp.z);
	//glEnd();
	intersp_found=false; 
	return lp1;			
}
