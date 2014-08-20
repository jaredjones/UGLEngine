//
//  WorldModelContainer.h
//  UGLEngine
//
//  Created by Jared Jones on 5/2/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__WorldModelContainer__
#define __UGLEngine__WorldModelContainer__

#include <OpenGL/OpenGL.h>

#include <iostream>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>

#include "VBOIndexer.h"

#define sWMC WorldModelContainer::getInstance()

struct ObjectRawHolder
{
    vec3Storage vertices;
    vec2Storage uvs;
    vec3Storage normals;
    
    uShortStorage indicesVBO;
    vec3Storage indexedVertices;
    vec2Storage indexedUvs;
    vec3Storage indexedNormals;
    
    bool hasQuads;
};

class WorldModelContainer
{
public:
    
    WorldModelContainer(){}
    ~WorldModelContainer(){}
    
    void Init();
    bool CompileAndStoreShader(std::string name, std::string vertexPath, std::string fragPath);
    GLuint GetShader(std::string name);
    bool MeshInMemory(std::string path);
    ObjectRawHolder* GetMeshInMemory(std::string path);
    bool AddMeshToMemory(std::string path, ObjectRawHolder* holder);//DO NOT USE! Use Model3D instead for creating meshes. (*Read implementation file comments.*)
    void RenderACycle();
    
    static WorldModelContainer& getInstance()
    {
        static WorldModelContainer singleObject;
        return singleObject;
    }
    
private:
    typedef std::mutex LockType;
    typedef std::lock_guard<LockType> GuardType;
    
    LockType _wMCLock;
    
    std::unordered_map<std::string, GLuint> _shaderMap;
    std::unordered_map<std::string, ObjectRawHolder*> _objectMap;
    
    
    GLuint _compileShader(std::string vertexPath, std::string fragPath, bool &error);
    
};

#endif /* defined(__UGLEngine__WorldModelContainer__) */
