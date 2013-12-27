#include <stdio.h>     // - Just for some ASCII messages
#include <string>
#include <array>
#include <vector>
#include <array>
#include <math.h>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"   // Header file for our OpenGL functions
#include "Load_obj.h"



void loadObj(std::string filename, std::vector<Point> &vertices, std::vector<Triangle> &triangles, std::vector<Triangle> &hornTriangles)
{
	bool ccw =true;
    Point v;	
	Triangle tr;

    int f1, f2, f3;		//pointers from triangles to their points
    char line[128];
    FILE *objfile;

	triangles.clear();
	vertices.clear();
	hornTriangles.clear();

	float avg=0, cnt=0;

    if (!(objfile = fopen(filename.c_str(), "rt"))) return;


    while (fgets(line, 128, objfile)) {
        switch (line[0]) {
		// create an array with the point cloud of the object
        case 'v':
            sscanf(&line[1],"%f %f %f", &v.x, &v.y, &v.z);

			// bring object (unicorn - high resolution) at the center of x-y-z axes
			if (obj_file=="objects/unicorn.obj"){
				v.x=v.x-21;
				v.y=v.y-51;
				v.z=v.z-65;
			}
			avg+=v.x;
			cnt++;

            vertices.push_back(v);
            break;
		// create an array with the triangles
        case 'f':
			sscanf(&line[1],"%d%d%d", &f1, &f2, &f3);

			int temp1,temp2,temp3;			
			//temp1=vertices.at(--f1);
			//temp2=vertices.at(--f2);
			//temp3=vertices.at(--f3);

			temp1=f1-1;				
			temp2=f2-1;
			temp3=f3-1;

			// define if the triangles are ccw or not
			//ccw = ccw_or_cw(temp1,temp2,temp3);				
			ccw = true; 

			if (ccw) {
				tr.p1=temp1;
				tr.p2=temp3;
				tr.p3=temp2;
			}
			else {
				tr.p1=temp1;
				tr.p2=temp2;
				tr.p3=temp3;
			}


			if (obj_file=="objects/unicorn.obj"){
				if ( vertices.at(tr.p1).z<-40 && vertices.at(tr.p1).y>29) hornTriangles.push_back(tr);
				else triangles.push_back(tr);
			}
			else triangles.push_back(tr);
            break;
        default:
            continue;
        };
    }
	avg=avg/cnt;
	printf("avg=%f\n",avg);
    fclose(objfile);
}


bool ccw_or_cw(Point p1, Point p2, Point p3)
{
	bool ccw; 

	float cross_prod_p1_p2 = (p1.y*p2.z-p1.z*p2.y)-(p1.x*p2.z-p1.z*p2.x)+(p1.x*p2.y-p1.y*p2.x);
	float cross_prod_p2_p3 = (p2.y*p3.z-p2.z*p3.y)-(p2.x*p3.z-p2.z*p3.x)+(p2.x*p3.y-p2.y*p3.x);
	float cross_prod_p3_p1 = (p3.y*p1.z-p3.z*p1.y)-(p3.x*p1.z-p3.z*p1.x)+(p3.x*p1.y-p3.y*p1.x);

	if (cross_prod_p1_p2+cross_prod_p2_p3+cross_prod_p3_p1 >0) ccw= false;
	else ccw = true;

	return ccw;
}
