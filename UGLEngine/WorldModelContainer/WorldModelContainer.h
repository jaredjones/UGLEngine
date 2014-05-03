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

#define sWMC WorldModelContainer::getInstance()

class WorldModelContainer
{
public:
    
    WorldModelContainer(){}
    ~WorldModelContainer(){}
    
    void Init();
    bool CompileAndStoreShader(std::string name, std::string vertexPath, std::string fragPath);
    bool GetShader(std::string name, GLuint &programID);
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
    
    GLuint _compileShader(std::string vertexPath, std::string fragPath, bool &error);
    
};

#endif /* defined(__UGLEngine__WorldModelContainer__) */
