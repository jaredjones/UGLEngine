//
//  main.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#define GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/transform.hpp>
#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "StaticTmpShit.h"

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
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    window = glfwCreateWindow(800, 600, "UGLEngine", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW failed to create window!" << std::endl;
        return -1;
    }
    
    //Makes a window the receiver for OpenGL Commands
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    
    /*The version is formatted as <version number><space><vendor-specific information>, 
     where <version number> is a MAJOR.MINOR format, with an optional release number.
     */
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    //OpenGL Settings Here
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    
    
    GLuint programID = LoadShaders("/Users/jaredjones/Developer/UGLEngine/UGLEngine/SimpleVertexShader.vs", "/Users/jaredjones/Developer/UGLEngine/UGLEngine/SimpleFragShader.fs");
    
    GLuint Texture = loadBMP_custom("/Users/jaredjones/Developer/UGLEngine/UGLEngine/numbers.bmp");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    GLuint vertexBuffer1;
    glGenBuffers(1, &vertexBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint colorBuffer1;
    glGenBuffers(1, &colorBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    GLuint vertexBuffer2;
    glGenBuffers(1, &vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
    
    GLuint colorBuffer2;
    glGenBuffers(1, &colorBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data2), g_color_buffer_data2, GL_STATIC_DRAW);
    
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
    
    float rotDeg = 0.0f;
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    while (1)
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("FPS:%f ms/F:%f\n", 1/(1.0/double(nbFrames)), 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        if (glfwWindowShouldClose(window))
            break;
        
        //BEGIN RENDERING
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glm::mat4 Projection = glm::perspective(45.0f, 800.0f / 600.f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(
                                     glm::vec3(0,0,3), //camera position
                                     glm::vec3(0,0,0), //Camera looking at
                                     glm::vec3(0,1,0) //Head is up
                                     );
        rotDeg++;
        glm::mat4 ModelCube = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), glm::vec3(1.0f, 1.0f, 0.0f));
        glm::mat4 MVPCube = Projection * View * ModelCube;
        
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPCube[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
        
        glUseProgram(programID);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
        );
        
        /*glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer2);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
        );*/
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                              1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                              2,                                // size : U+V => 2
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        glDrawArrays(GL_TRIANGLES, 0, 3*12);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        glm::mat4 ModelTri = glm::translate(glm::vec3(2.0f, 0.0f, -2.0f));
        glm::mat4 MVPTri = Projection * View * ModelTri;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPTri[0][0]);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
        glVertexAttribPointer(
                              0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void *)0
                              );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer1);
        glVertexAttribPointer(
                              1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void *)0
                              );
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        //END RENDERING
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

