
void drawSphereVoxels(std::vector<Voxel> voxels, float voxel_width);

void freeFallOfVoxels( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, Vector voxel_a, float dt);

void checkFloorCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity, float floor_coord_y);

void checkVoxelCollisions( std::vector<Voxel> &voxels, std::vector<Vector> &voxelVelocity);

void initiateVelocities( std::vector<Voxel> voxels, std::vector<Vector> &voxelVelocity);