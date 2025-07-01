#ifndef MESH_H
#define MESH_H
#include <vector>
#include "GpuBuffer.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::unique_ptr<GpuBuffer> gpuBuffer;
};

#endif //MESH_H
