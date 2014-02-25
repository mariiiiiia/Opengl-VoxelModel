
//extern std::vector< std::vector< std::vector<Point> >> buckets;


void StartCounter();

double GetCounter();


void drawSphereVoxels(std::vector<Voxel> voxels, float rad);

void freeFallOfVoxels( std::vector<Voxel> &voxels, std::vector< std::vector<Voxel>> &voxel_data, float dt);

void checkFloorCollisions( std::vector<Voxel> &voxels, float voxel_width, float tx, float ty, float tz, float dt);

void checkVoxelCollisions( std::vector<Voxel> &voxels, float voxel_width);

void initiateVelocities( std::vector<Voxel> &voxels);

//void checkVoxelCollisionsInBuckets( std::vector< Voxel> &voxels, std::vector< Vector> &voxelVelocity);
//
//void checkVoxelCollision( Vector &vel1, Vector &vel2, Voxel &vox1, Voxel &vox2);