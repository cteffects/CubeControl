/*
 * bytevoxel.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Charles Trippe
 */

#ifndef BYTEVOXEL_H_
#define BYTEVOXEL_H_

namespace CubeControl
{

template <unsigned int x, unsigned int y, unsigned int z>
struct ByteVoxelBuffer
{
    static constexpr unsigned int voxelCount = x*y*z;

    ///write next byte into voxel array, returns true if reached end
    bool inline writeNextByte(unsigned char nextByte)
    {
        *currentvoxel = nextByte;
        if(currentvoxel == lastvoxel)
        {
            currentvoxel = &voxels[0];
            return true;
        }
        else
        {
            ++currentvoxel;
            return false;
        }
    };

    ///resets write index to start
    void inline resetBufferIndex()
    {
        currentvoxel = &voxels[0];
    }

    unsigned char voxels[voxelCount] = {0};
    unsigned char* currentvoxel = &voxels[0];
    unsigned char* lastvoxel = &voxels[voxelCount-1];
};

}

#endif /* BYTEVOXEL_H_ */
