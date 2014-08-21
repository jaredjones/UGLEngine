//
//  Model3D.cpp
//  UGLEngine
//
//  Created by Jared Jones on 5/3/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include "Model3D.h"
#include "WorldModelContainer.h"
#include "VBOIndexer.h"
#include "OBJLoader.h"

Model3D::Model3D(std::string path, bool vboIndexing)
{
    if(sWMC.MeshInMemory(path))
    {
        ObjectRawHolder *tmpObject;
        tmpObject = sWMC.GetMeshInMemory(path);
        _quadBased = tmpObject->hasQuads;
        _path = path;
        
        if (tmpObject->indicesVBO.empty() && vboIndexing)
        {
            uShortStorage indicesVBO;
            vec3Storage indexedVertices;
            vec2Storage indexedUvs;
            vec3Storage indexedNormals;
            indexVBO(tmpObject->vertices, tmpObject->uvs, tmpObject->normals, indicesVBO, indexedVertices, indexedUvs, indexedNormals);
            
            tmpObject->indicesVBO = indicesVBO;
            tmpObject->indexedVertices = indexedVertices;
            tmpObject->indexedUvs = indexedUvs;
            tmpObject->indexedNormals = indexedNormals;
        }
        if (tmpObject->vertices.empty() && !vboIndexing)
        {
            vec3Storage vertices;
            vec2Storage uvs;
            vec3Storage normals;
            bool hasQuads;
            loadOBJ(path.c_str(), vertices, uvs, normals, hasQuads);
            tmpObject->vertices = vertices;
            tmpObject->uvs = uvs;
            tmpObject->normals = normals;
            tmpObject->hasQuads = hasQuads;
            _quadBased = hasQuads;
        }
    }
    else
    {
        ObjectRawHolder *tmpObject = new ObjectRawHolder;
        vec3Storage vertices;
        vec2Storage uvs;
        vec3Storage normals;
        bool hasQuads;
        
        loadOBJ(path.c_str(), vertices, uvs, normals, hasQuads);
        if (vboIndexing)
        {
            uShortStorage indicesVBO;
            vec3Storage indexedVertices;
            vec2Storage indexedUvs;
            vec3Storage indexedNormals;
            indexVBO(vertices, uvs, normals, indicesVBO, indexedVertices, indexedUvs, indexedNormals);
            
            tmpObject->indicesVBO = indicesVBO;
            tmpObject->indexedVertices = indexedVertices;
            tmpObject->indexedUvs = indexedUvs;
            tmpObject->indexedNormals = indexedNormals;
            tmpObject->hasQuads = hasQuads;
        }
        else
        {
            tmpObject->vertices = vertices;
            tmpObject->uvs = uvs;
            tmpObject->normals = normals;
            tmpObject->hasQuads = hasQuads;
        }
        _quadBased = hasQuads;
        _path = path;
        sWMC.AddMeshToMemory(path, tmpObject);
    }

    

    
    

}

Model3D::~Model3D()
{
    
}