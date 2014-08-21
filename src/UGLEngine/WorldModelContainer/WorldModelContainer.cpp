//
//  WorldModelContainer.cpp
//  UGLEngine
//
//  Created by Jared Jones on 5/2/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include <Common.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "WorldModelContainer.h"

void WorldModelContainer::Init()
{
    //OpenGL Settings Here
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
}

GLuint WorldModelContainer::_compileShader(std::string vertexPath, std::string fragPath, bool &error)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexPath, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragPath, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertexPath.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    if (&VertexShaderErrorMessage[0] != nullptr)
    {
        error = true;
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragPath.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    if (&FragmentShaderErrorMessage[0] != nullptr)
    {
        error = true;
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    // Link the program
    fprintf(stdout, "Linking shader program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    
    if (InfoLogLength != 0)
        error = true;
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

bool WorldModelContainer::CompileAndStoreShader(std::string name, std::string vertexPath, std::string fragPath)
{
    GuardType guard(_wMCLock);
    std::unordered_map<std::string,GLuint>::const_iterator got = _shaderMap.find (name);
    if (got != _shaderMap.end())
    {
        printf("WARNING: The shader:%s already exists under programID:%d, therefore we're NOT compiling this shader! If you want to recompile this shader, delete it first!\n",
               name.c_str(), _shaderMap[name]);
        return false;
    }
    bool compileFailed = false;
    GLuint programID = _compileShader(vertexPath, fragPath, compileFailed);
    
    if (compileFailed)
    {
        printf("WARNING: The shader:%s with programID:%d failed to compile.\n", name.c_str(), programID);
        return false;
    }
    _shaderMap.insert(std::make_pair(name, programID));
    return true;
}

GLuint WorldModelContainer::GetShader(std::string name)
{
    GuardType guard(_wMCLock);
    std::unordered_map<std::string,GLuint>::const_iterator got = _shaderMap.find (name);
    if (got == _shaderMap.end())
    {
        printf("WARNING: The shader:%s does not exist in program memory! Please CompileAndStoreShader first!\n", name.c_str());
        return 0;
    }
    return _shaderMap[name];
}

void WorldModelContainer::RenderACycle()
{
    
}

bool WorldModelContainer::MeshInMemory(std::string path)
{
    GuardType guard(_wMCLock);
    std::unordered_map<std::string,ObjectRawHolder*>::const_iterator got = _objectMap.find (path);
    if (got == _objectMap.end())
        return false;
    
    return true;
}

ObjectRawHolder* WorldModelContainer::GetMeshInMemory(std::string path)
{
    GuardType guard(_wMCLock);
    std::unordered_map<std::string,ObjectRawHolder*>::const_iterator got = _objectMap.find (path);
    if (got == _objectMap.end())
    {
        printf("WARNING: You attempted to get a mesh in program memory that does not exist!\n");
        return NULL;
    }
    return _objectMap[path];
}

//This method is public ONLY for future advanced features. DO NOT use this unless you know what you're doing. Use Model3D instead for creating meshes!
//Misuse of this method could cause memory errors and could devistatingly place the application in a corrupt state.
bool WorldModelContainer::AddMeshToMemory(std::string path, ObjectRawHolder* holder)
{
    GuardType guard(_wMCLock);
    std::unordered_map<std::string,ObjectRawHolder*>::const_iterator got = _objectMap.find (path);
    if (got != _objectMap.end())
    {
        printf("WARNING: You're attempting to add a mesh into memory that already exists. Delete it first! This method is advanced and should not be called manually, use Model3D for implementing meshes.\n");
        return false;
    }
    _objectMap.insert(std::make_pair(path, holder));
    return true;
}