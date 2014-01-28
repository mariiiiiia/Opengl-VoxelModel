#include <vector>
#include "gl\glut.h"
#include "structs.h"
#include "triangulateVoxelModel.h"

static std::vector< Triangle> newTriangles;

static std::vector< std::vector< Voxel> > voxel_neighbors;
static std::vector< std::vector< Point> > lines;			//every point and the lines it's related to

void triangulateVoxelModel(std::vector<Voxel> voxels){
	
	//----- find neighbors -------
	float sqr_dist;
	std::vector<Voxel> vox_neighb;
	
	for (int i=0; i<int(voxels.size()); i++){
		for (int j=i+1; j<int(voxels.size()); j++){
			sqr_dist = pow( voxels.at(i).x-voxels.at(j).x,2) + \
					   pow( voxels.at(i).y-voxels.at(j).y,2) + \
					   pow( voxels.at(i).z-voxels.at(j).z,2);

			if ((sqr_dist>=(voxels.at(0).width()*voxels.at(0).width()- voxels.at(0).width()/10) && (sqr_dist<=voxels.at(0).width()*voxels.at(0).width()+ voxels.at(0).width()/10)) ||
				(sqr_dist>=(2*voxels.at(0).width()*voxels.at(0).width()- voxels.at(0).width()/10) && (sqr_dist<=2*voxels.at(0).width()*voxels.at(0).width()+ voxels.at(0).width()/10) )){
				vox_neighb.push_back( voxels.at(j) );
			}
		}
		voxel_neighbors.push_back(vox_neighb);
		vox_neighb.clear();
	}

	for (int i=0; i<int(voxels.size()); i++){
		for (int j=0; j<int(voxel_neighbors.at(i).size()); j++){

			//checkLineIntersection( voxels.at(i), voxel_neighbors.at(i));

			glBegin( GL_LINES);
			glVertex3f( voxels.at(i).x, voxels.at(i).y, voxels.at(i).z);
			glVertex3f( voxel_neighbors.at(i).at(j).x, voxel_neighbors.at(i).at(j).y, voxel_neighbors.at(i).at(j).z );
			glEnd();
		}
	}

}