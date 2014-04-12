//
//  main.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"

int main(int argc, const char * argv[])
{
    GLFWwindow *window;
    
    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialize!" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(800, 600, "UGLEngine", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW failed to create window!" << std::endl;
        return -1;
    }
    
    //Makes a window the receiver for OpenGL Commands
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    
    /*The version is formatted as <version number><space><vendor-specific information>, 
     where <version number> is a MAJOR.MINOR format, with an optional release number.
     */
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    
    
    
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        0.0f,  1.0f, 1.0f,
    };
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    
    GLuint programID = LoadShaders("/Users/jaredjones/Desktop/UGLEngine/UGLEngine/SimpleVertexShader.vs", "/Users/jaredjones/Desktop/UGLEngine/UGLEngine/SimpleFragShader.fs");
    
    
    while (1)
    {
        if (glfwWindowShouldClose(window))
            break;
        
        //BEGIN RENDERING
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(programID);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
        );
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        //END RENDERING
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    std::cout << "Hello, World!\n";
    return 0;
}

