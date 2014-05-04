//
//  OBJLoader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/15/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "OBJLoader.h"

bool loadOBJ(const char *path, vec3Storage &out_verticies, vec2Storage &out_uvs, vec3Storage &out_normals, bool &hasQuads)
{
    std::vector<unsigned int> vIndices, uvIndices, normalIndices;
    vec3Storage tmpVerts;
    vec2Storage tmpUvs;
    vec3Storage tmpNorms;
    
    std::ifstream tmpFile;
    if (!tmpFile.good())
    {
        printf("The OBJ file does not exist!\n");
        return false;
    }
    
    tmpFile.open(path, std::ios::in);
    if (!tmpFile.is_open())
    {
        printf("The OBJ file exists but failed to open!\n");
        return false;
    }
    
    std::string line;
    while (std::getline(tmpFile, line))
    {
        if (line[0] == '#')
            continue;
        
        std::istringstream lStream(line);
        
        std::string word;
        lStream >> word;
        
        if (word == "v")
        {
            float x, y, z;
            lStream >> x >> y >> z;
            tmpVerts.push_back(glm::vec3(x, y, z));
            continue;
        }
        
        if (word == "vt")
        {
            float u,v;
            lStream >> u >> v;
            tmpUvs.push_back(glm::vec2(u, v));
            continue;
        }
        
        if (word == "vn")
        {
            float x, y, z;
            lStream >> x >> y >> z;
            tmpNorms.push_back(glm::vec3(x, y, z));
            continue;
        }
        
        if (word == "f")
        {
            std::string v1, v2, v3, v4;
            lStream >> v1 >> v2 >> v3 >> v4;
            std::istringstream vS1(v1);
            std::istringstream vS2(v2);
            std::istringstream vS3(v3);
            std::istringstream vS4(v4);
            
            int x;
            char a;
            int y;
            char b;
            int z;
            
            vS1 >> x >> a >> y >> b >> z;
            vIndices.push_back(x);
            uvIndices.push_back(y);
            normalIndices.push_back(z);
            
            vS2 >> x >> a >> y >> b >> z;
            vIndices.push_back(x);
            uvIndices.push_back(y);
            normalIndices.push_back(z);
            
            vS3 >> x >> a >> y >> b >> z;
            vIndices.push_back(x);
            uvIndices.push_back(y);
            normalIndices.push_back(z);
            
            if (!v4.empty())
            {
                vS4 >> x >> a >> y >> b >> z;
                vIndices.push_back(x);
                uvIndices.push_back(y);
                normalIndices.push_back(z);
                hasQuads = true;
            }
            continue;
        }
        
    }
    if (vIndices.size() >= 65535)
        printf("WARNING: This mesh is over UINT16 in size, rendering issues may arrise if you use VBOIndexing! This warning isn't checking to see if VBOIndexing is used. However if this warning ever fires please contact Jared immediately, as it is likely an appropriate time to fix the weakness in the engine.\n");
    for (unsigned int i = 0; i < vIndices.size(); i++)
    {
        unsigned int vIndex = vIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normIndex = normalIndices[i];

        glm::vec3 vertex = tmpVerts[vIndex - 1];
        glm::vec2 uv = tmpUvs[uvIndex - 1];
        glm::vec3 norm = tmpNorms[normIndex - 1];
        
        out_verticies.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(norm);
    }
    return true;
}