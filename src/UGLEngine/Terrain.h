//
//  Terrain.h
//  uglengine
//
//  Created by Jared Jones on 11/5/17.
//

#ifndef Terrain_h
#define Terrain_h

#include <stdio.h>
#include "RawModel.h"
#include "ModelTexture.h"
#include "Loader.h"

class Terrain
{
private:
    const float SIZE = 800;
    const int32_t VERTEX_COUNT = 128;
    float x;
    float z;
    RawModel *model;
    ModelTexture *texture;
    
    RawModel * _generateTerrain(Loader *loader)
    {
        int count = VERTEX_COUNT * VERTEX_COUNT;
        
        std::vector<glm::vec3> vertices = std::vector<glm::vec3>(count);
        std::vector<glm::vec3> normals = std::vector<glm::vec3>(count);
        std::vector<glm::vec2> uvs = std::vector<glm::vec2>(count);
        std::vector<uint> indices = std::vector<uint>( 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT * 1) );
        
        int vertexPointer = 0;
        for (int i = 0; i < VERTEX_COUNT; i++)
        {
            for (int j = 0; j < VERTEX_COUNT; j++)
            {
                vertices[vertexPointer].x = -(float)j/((float) VERTEX_COUNT - 1) * SIZE;
                vertices[vertexPointer].y = 0;
                vertices[vertexPointer].z = -(float)i/((float) VERTEX_COUNT - 1) * SIZE;
                normals[vertexPointer].x = 0;
                normals[vertexPointer].y = 1;
                normals[vertexPointer].z = 0;
                uvs[vertexPointer].x = (float)j/((float)VERTEX_COUNT - 1);
                uvs[vertexPointer].y = (float)i/((float)VERTEX_COUNT - 1);
                vertexPointer++;
            }
        }
        
        int pointer = 0;
        for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
        {
            for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
            {
                int topLeft = (gz * VERTEX_COUNT) + gx;
                int topRight = topLeft + 1;
                int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
                int bottomRight = bottomLeft + 1;
                indices[pointer++] = topLeft;
                indices[pointer++] = bottomLeft;
                indices[pointer++] = topRight;
                indices[pointer++] = topRight;
                indices[pointer++] = bottomLeft;
                indices[pointer++] = bottomRight;
            }
        }
        return loader->LoadToVao(vertices, uvs, normals, indices);
    }
    
public:
    Terrain(int gridX, int gridZ, Loader *loader, ModelTexture *texture)
    {
        this->texture = texture;
        this->x = gridX * SIZE;
        this->z = gridZ * SIZE;
        this->model = _generateTerrain(loader);
    }
    float GetX();
    float GetZ();
    RawModel * GetModel();
    ModelTexture * GetTexture();
};

#endif /* Terrain_h */
