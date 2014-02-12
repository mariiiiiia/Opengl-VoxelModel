#include <stdio.h>     // - Just for some ASCII messages
#include <vector>
#include <math.h>
#include "structs.h"


Vector CalcTriangleNormal(std::vector< Point> vertices, Triangle triangle)
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

	if (dist!=0){
		normal.x = normal.x/dist;
		normal.y = normal.y/dist;
		normal.z = normal.z/dist;
	}
	return normal;
}

Vector CalcTriangleNormal( TriangleCoord triangle)
{
	Vector v1,v2;
	v1.x=triangle.p2.x-triangle.p1.x; 
	v1.y=triangle.p2.y-triangle.p1.y;
	v1.z=triangle.p2.z-triangle.p1.z;

	v2.x=triangle.p3.x-triangle.p1.x; 
	v2.y=triangle.p3.y-triangle.p1.y; 
	v2.z=triangle.p3.z-triangle.p1.z;

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

void avgNormals( std::vector< Vector> &vertNormal, std::vector< Vector> normal, std::vector<Triangle> triangle, int vertices_quantity)
{
	std::vector<int> cnt;

	vertNormal.clear();

	for (int i=0; i<vertices_quantity; i++) {
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
