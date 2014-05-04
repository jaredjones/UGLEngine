//
//  VBOIndexer.h
//  UGLEngine
//
//  Created by Jared Jones on 4/17/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__VBOIndexer__
#define __UGLEngine__VBOIndexer__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>


    
    typedef std::vector <glm::vec3> vec3Storage;
    typedef std::vector <glm::vec2> vec2Storage;
    typedef std::vector <unsigned short> uShortStorage;

    struct PackedVertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        bool operator<(const PackedVertex that) const
        {
            return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
        };
    };
    
    void indexVBO(vec3Storage &vertices, vec2Storage &uvs, vec3Storage &normals,
                  uShortStorage &outIndices, vec3Storage &outVertices, vec2Storage &outUvs, vec3Storage &outNormals);
    void indexVBOandTB(vec3Storage &vertices, vec2Storage &uvs, vec3Storage &normals, vec3Storage &tangents, vec3Storage &bitangents,
                       uShortStorage &outIndices, vec3Storage &outVertices, vec2Storage &outUvs, vec3Storage &outNormals, vec3Storage &outTangents, vec3Storage &outBItangents);
    

#endif /* defined(__UGLEngine__VBOIndexer__) */
