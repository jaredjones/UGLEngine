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


bool loadOBJ(const char *path, uIntStorage &out_indices, vec3Storage &out_verticies, vec2Storage &out_uvs, vec3Storage &out_normals, vec3Storage &out_tangents, vec3Storage &out_bitangents, bool transthorpulate, bool triangulate)
{
    std::vector<unsigned int> vIndices, uvIndices, normalIndices, tangentIndices, bitangentIndices;
    vec3Storage tmpVerts;
    vec2Storage tmpUvs;
    vec3Storage tmpNorms;
    vec3Storage tmpTangs;
    vec3Storage tmpBitangs;
    
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
    int lineCounter = 0;
    while (std::getline(tmpFile, line))
    {
        lineCounter++; //most text editors count lines starting at one
        if (line[0] == '#')
            continue;
        
        std::istringstream lStream(line);
        
        std::string word;
        lStream >> word;
        
        if (word == "v")
        {
            std::string triad;
            triad = lStream.str();
            
            float componentsOfVector[3];
            
            int mark = 2;
            int current = 0;
            for (int i = 2; i < triad.length(); i++)
            {
                if (current > 3)
                    break;
                if (isspace(triad[i]) || i == triad.length() - 1)
                {
                    componentsOfVector[current] = atof(triad.substr(mark, (i - mark)).c_str());
                    
                    mark = i + 1;
                    current++;
                }
            }
            tmpVerts.push_back(glm::vec3(componentsOfVector[0], componentsOfVector[1], componentsOfVector[2]));
        }
        
        else if (word == "vt")
        {
            float u,v;
            lStream >> u >> v;
            tmpUvs.push_back(glm::vec2(u, v));
        }
        
        else if (word == "vn")
        {
            float x, y, z;
            lStream >> x >> y >> z;
            tmpNorms.push_back(glm::vec3(x, y, z));
        }
        
        else if (word == "f")
        {
            std::vector<unsigned int> fvIndices, fvtIndices, fvnIndices; //face data
            bool abortLine = false;
            
            // Load from string into above arrays
            while (lStream.good())
            {
                std::string triad;
                lStream >> triad;
                
                int indices[3] = { -1, -1, -1 };
                
                int mark = 0;
                int current = 0;
                
                for (int i = 0; i < triad.length();i++)
                {
                    if (triad[i] == '/')
                    {
                        if (mark != i)
                            indices[current] = atoi(triad.substr(mark, (i-mark)).c_str());
                        mark = i+1;
                        current++;
                    }
                }
                
                // Get last element
                indices[current] = atoi(triad.substr(mark, (triad.length())-mark).c_str());
                
                if (indices[0] != -1) fvIndices.push_back(indices[0]);
                if (indices[1] != -1) fvtIndices.push_back(indices[1]);
                if (indices[2] != -1) fvnIndices.push_back(indices[2]);
                
            }
            
            if (abortLine)
                continue;
            
            if ((fvtIndices.size() > 0 && fvtIndices.size() != fvIndices.size()) ||
                (fvnIndices.size() > 0 && fvnIndices.size() != fvIndices.size()) )
            {
                printf("Face with malformed vertices on line:%d in %s : %s\n", lineCounter, path, line.c_str());
                continue;
            }
            
            
            if (fvIndices.size() < 3)
            {
                printf("Face with less than 3 verticies on line:%d in %s : %s\n", lineCounter, path, line.c_str());
                continue;
            }
            
            if (triangulate)
            {
                // We have an n-gon
                if (fvIndices.size() > 3)
                {
                    std::vector<unsigned int> newfvIndices;
                    std::vector<unsigned int> newfvtIndices;
                    std::vector<unsigned int> newfvnIndices;
                    for (int i = 2; i < fvIndices.size(); i++)
                    {
                        newfvIndices.push_back(fvIndices[0]);
                        newfvIndices.push_back(fvIndices[i - 1]);
                        newfvIndices.push_back(fvIndices[i]);
                    }
                    if (fvtIndices.size() > 3)
                    {
                        for (int i = 2; i < fvtIndices.size(); i++)
                        {
                            newfvtIndices.push_back(fvtIndices[0]);
                            newfvtIndices.push_back(fvtIndices[i - 1]);
                            newfvtIndices.push_back(fvtIndices[i]);
                        }
                    }
                    if (fvnIndices.size() > 3)
                    {
                        for (int i = 2; i < fvnIndices.size(); i++)
                        {
                            newfvnIndices.push_back(fvnIndices[0]);
                            newfvnIndices.push_back(fvnIndices[i - 1]);
                            newfvnIndices.push_back(fvnIndices[i]);
                        }
                    }
                    
                    fvIndices = newfvIndices;
                    fvtIndices = newfvtIndices;
                    fvnIndices = newfvnIndices;
                }
            }
            
            if (transthorpulate)
            {
                //no texture coords were provided
                if (fvtIndices.size() == 0)
                {
                    for (int i = 0; i < fvIndices.size(); i++)
                    {
                        tmpUvs.push_back(glm::vec2(1, 1));
                        int dummyTextureCoordVectorIndex = tmpUvs.size(); //remember, all indexes are +1
                        fvtIndices.push_back(dummyTextureCoordVectorIndex);
                    }
                }
                
                //no normals were provided
                if (fvnIndices.size() == 0)
                {
                    glm::vec3 calculatedFaceNormal =
                    glm::cross(tmpVerts[fvIndices[1] - 1] - tmpVerts[fvIndices[0] - 1],
                               tmpVerts[fvIndices[2] - 1] - tmpVerts[fvIndices[0] - 1]);
                    for (int i = 0; i < fvIndices.size(); i++)
                    {
                        tmpNorms.push_back(calculatedFaceNormal);
                        int index = tmpNorms.size();
                        fvnIndices.push_back(index);
                    }
                }
            }
            
            //push face verts to model
            for (unsigned int i = 0; i < fvIndices.size(); i++)
                vIndices.push_back(fvIndices[i]);
            
            for (unsigned int i = 0; i < fvtIndices.size(); i++)
                uvIndices.push_back(fvtIndices[i]);
            
            for (unsigned int i = 0; i < fvnIndices.size(); i++)
                normalIndices.push_back(fvnIndices[i]);
            
            //generate tangents and bitangents
            for (unsigned int i = 0; i < fvIndices.size(); i += 3)
            {
                // Shortcuts for vertices
                glm::vec3 & v0 = tmpVerts[fvIndices[i] - 1];
                glm::vec3 & v1 = tmpVerts[fvIndices[i+1] - 1];
                glm::vec3 & v2 = tmpVerts[fvIndices[i+2] - 1];
                
                // Shortcuts for UVs
                glm::vec2 & uv0 = tmpUvs[fvtIndices[i] - 1];
                glm::vec2 & uv1 = tmpUvs[fvtIndices[i+1] - 1];
                glm::vec2 & uv2 = tmpUvs[fvtIndices[i+2] - 1];
                
                // Edges of the triangle : postion delta
                glm::vec3 deltaPos1 = v1 - v0;
                glm::vec3 deltaPos2 = v2 - v0;
                
                // UV delta
                glm::vec2 deltaUV1 = uv1 - uv0;
                glm::vec2 deltaUV2 = uv2 - uv0;
                
                float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
                glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
                glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;
                
                for (int i = 0; i < 3; i++)
                {
                    tangentIndices.push_back(i + 1);
                    tmpTangs.push_back(tangent);
                    bitangentIndices.push_back(i + 1);
                    tmpBitangs.push_back(bitangent);
                }
            }
        }
        else
        {
            printf("Unsupported wavefront command on line %d in %s : %s\n", lineCounter, path, line.c_str());
        }     
    }
    /*if (vIndices.size() >= 65535)
        printf("WARNING: This mesh is over UINT16 in size, rendering issues may arrise if you use VBOIndexing! This warning isn't checking to see if VBOIndexing is used. However if this warning ever fires please contact Jared immediately, as it is likely an appropriate time to fix the weakness in the engine.\n");
    */
    /*for (unsigned int i = 0; i < vIndices.size(); i++)
     out_verticies.push_back(tmpVerts[vIndices[i] - 1]);
     
     for (unsigned int i = 0; i < uvIndices.size(); i++)
     out_uvs.push_back(tmpUvs[uvIndices[i] - 1]);
     
     for (unsigned int i = 0; i < normalIndices.size(); i++)
     out_normals.push_back(tmpNorms[normalIndices[i] - 1]);*/
    
    for (unsigned int i = 0; i < vIndices.size(); i++)
    {
        out_indices.push_back(i);
        out_verticies.push_back(tmpVerts[vIndices[i] - 1]);
        out_uvs.push_back(tmpUvs[uvIndices[i] - 1]);
        out_normals.push_back(tmpNorms[normalIndices[i] - 1]);
        out_tangents.push_back(tmpTangs[bitangentIndices[i] - 1]);
        out_bitangents.push_back(tmpBitangs[bitangentIndices[i] - 1]);
    }
    
    return true;
}

/*bool loadOBJ(const char *path, uShortStorage &out_indices, vec3Storage &out_verticies, vec2Storage &out_uvs, vec3Storage &out_normals, vec3Storage &out_tangents, vec3Storage &out_bitangents, bool transthorpulate, bool triangulate)
{
    uIntStorage vboIndices;
    
    bool out = loadOBJ(path, vboIndices, out_verticies, out_uvs, out_normals, out_tangents, out_bitangents,transthorpulate, triangulate);
    
    for (unsigned int i : vboIndices)
        out_indices.push_back((unsigned short)i);
    
    return out;
}*/
