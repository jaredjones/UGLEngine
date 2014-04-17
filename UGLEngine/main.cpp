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
#include "CameraController.h"
#include "OBJLoader.h"
#include "VBOIndexer.h"

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
    
    
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    
    GLuint programID = LoadShaders("/Users/jaredjones/Developer/UGLEngine/UGLEngine/SimpleVertexShader.vs", "/Users/jaredjones/Developer/UGLEngine/UGLEngine/SimpleFragShader.fs");
    
    GLuint Texture = loadDDS("/Users/jaredjones/Developer/UGLEngine/UGLEngine/uvmap.DDS");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    //Create a handle for the uniforms
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    
    //OBJ BEGIN TEST
    vec3Storage verticies;
    vec2Storage uvs;
    vec3Storage normals;
    
    loadOBJ("/Users/jaredjones/Developer/UGLEngine/UGLEngine/suzanne.obj", verticies, uvs, normals);
    
    uShortStorage indices;
    vec3Storage indexedVertices;
    vec2Storage indexedUvs;
    vec3Storage indexedNormals;
    indexVBO(verticies, uvs, normals, indices, indexedVertices, indexedUvs, indexedNormals);
    
    GLuint vertexBuffer2;
    glGenBuffers(1, &vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
    
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
    
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
    
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    
    //OBJ END TEST
    
    
    GLuint colorBuffer2;
    glGenBuffers(1, &colorBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data2), g_color_buffer_data2, GL_STATIC_DRAW);
    
    
    float rotDeg = 0.0f;
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    while (1)
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            //printf("FPS:%f ms/F:%f\n", 1/(1.0/double(nbFrames)), 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        if (glfwWindowShouldClose(window))
            break;
        
        computeMatricesFromInputs(window);
        
        //BEGIN RENDERING
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        
        glUseProgram(programID);
        
        rotDeg++;
        glm::mat4 ModelCube = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), glm::vec3(1.0f, 1.0f, 1.0f));
        //glm::mat4 ModelCube = glm::mat4(1.0f);
        glm::mat4 MVPCube = getProjectionMatrix() * getViewMatrix() * ModelCube;
        
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPCube[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelCube[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &getViewMatrix()[0][0]);
        
        glm::vec3 lightPos = glm::vec3(4,4,4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
        
        
        
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
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(
                              2,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
        );
        
        //Index Buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_SHORT, (void *)0);
        
        //glDrawArrays(GL_TRIANGLES, 0, static_cast<GLuint>(verticies.size()));
        
        
        
        //Begin Second Model
		glm::mat4 ModelMatrix2 = glm::mat4(1.0);
		ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(1.5f, 2.0f, 0.0f));
		glm::mat4 MVP2 = getProjectionMatrix() * getViewMatrix() * ModelMatrix2;
        
		//Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);
        
    
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        
		glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_SHORT, (void *)0);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        
        //END RENDERING
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &vertexBuffer2);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &vertexArrayID);
    
    glfwTerminate();
    return 0;
}

