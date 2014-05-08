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

#include "WorldModelContainer.h"
#include "ImageLoader.h"
#include "Model3D.h"
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
    
    sWMC.Init();
    sWMC.CompileAndStoreShader("simpleshader", "Resources/Shaders/SimpleVertexShader.vs", "Resources/Shaders/SimpleFragShader.fs");
    
    GLuint programID;
    
    bool shaderExists = sWMC.GetShader("simpleshader", programID);
    shaderExists = shaderExists;

    
    //Model3D *myTest = new Model3D("Resources/Models/trashcan.wvf", false);
    
    //GLuint Texture = loadDDS("Resources/Images/uvmap.DDS");
    GLuint Texture = loadBMP_custom("Resources/Images/Trash_Can.bmp");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    //Create a handle for the uniforms
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    
    //OBJ BEGIN TEST
    vec3Storage vertices;
    vec2Storage uvs;
    vec3Storage normals;
    bool hasQuads;
    
    loadOBJ("Resources/Models/suzanne.wvf", vertices, uvs, normals, hasQuads);
    
    uShortStorage indicesSuzanne;
    vec3Storage indexedVertices;
    vec2Storage indexedUvs;
    vec3Storage indexedNormals;
    indexVBO(vertices, uvs, normals, indicesSuzanne, indexedVertices, indexedUvs, indexedNormals);
    
    GLuint vertexBufferSuzanne;
    glGenBuffers(1, &vertexBufferSuzanne);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferSuzanne);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
    
    GLuint uvBufferSuzanne;
    glGenBuffers(1, &uvBufferSuzanne);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferSuzanne);
    glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
    
    GLuint normalBufferSuzanne;
    glGenBuffers(1, &normalBufferSuzanne);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferSuzanne);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
    
    GLuint elementBufferSuzanne;
    glGenBuffers(1, &elementBufferSuzanne);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferSuzanne);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSuzanne.size() * sizeof(unsigned short), &indicesSuzanne[0], GL_STATIC_DRAW);
    
    //OBJ END TEST
    
    // OBJ FLOOR
    vertices.clear();
    uvs.clear();
    normals.clear();
    loadOBJ("Resources/Models/floor.wvf", vertices, uvs, normals, hasQuads);

    uShortStorage indicesFloor;
    indexedVertices.clear();
    indexedUvs.clear();
    indexedNormals.clear();
    indexVBO(vertices, uvs, normals, indicesFloor, indexedVertices, indexedUvs, indexedNormals);

    GLuint vertexBufferFloor;
    glGenBuffers(1, &vertexBufferFloor);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferFloor);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
    
    GLuint uvBufferFloor;
    glGenBuffers(1, &uvBufferFloor);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferFloor);
    glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
    
    GLuint normalBufferFloor;
    glGenBuffers(1, &normalBufferFloor);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferFloor);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
    
    GLuint elementBufferFloor;
    glGenBuffers(1, &elementBufferFloor);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferFloor);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesFloor.size() * sizeof(unsigned short), &indicesFloor[0], GL_STATIC_DRAW);
    
    // OBJ END FLOOR

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    // attribute. No particular reason for 1, but must match the layout in the shader.
    // size : U+V => 2
    // type
    // normalized?
    // stride
    // array buffer offset
    glEnableVertexAttribArray(15);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferSuzanne);
    glVertexAttribPointer(15,3,GL_FLOAT,GL_FALSE,0,(void *)0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferSuzanne);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferSuzanne);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    
    
    float rotDeg = 0.0f;
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float lightZ = 4;
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
        glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
        
        glUseProgram(programID);
        
        rotDeg++;
        glm::mat4 ModelCube = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), glm::vec3(1.0f, 1.0f, 1.0f));
        //glm::mat4 ModelCube = glm::mat4(1.0f);
        glm::mat4 MVPCube = getProjectionMatrix() * getViewMatrix() * ModelCube;
        
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPCube[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelCube[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &getViewMatrix()[0][0]);
        
        glm::vec3 lightPos = glm::vec3(4*cos(lightZ),4,4*sin(lightZ));
        lightZ +=.03;
        
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
     
        
        //Index Buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferSuzanne);
        
        glDrawElements(GL_TRIANGLES, static_cast<int>(indicesSuzanne.size()), GL_UNSIGNED_SHORT, (void *)0);
        
//        
//        //Begin Second Model
//		glm::mat4 ModelMatrix2 = glm::mat4(1.0);
//		ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(1.5f, 2.0f, 0.0f));
//		glm::mat4 MVP2 = getProjectionMatrix() * getViewMatrix() * ModelMatrix2;
//        
//		//Send our transformation to the currently bound shader, in the "MVP" uniform
//		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
//		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);
//        
//    
//		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferSuzanne);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glBindBuffer(GL_ARRAY_BUFFER, uvBufferSuzanne);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glBindBuffer(GL_ARRAY_BUFFER, normalBufferSuzanne);
//		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferSuzanne);
//        
//		glDrawElements(GL_TRIANGLES, static_cast<int>(indicesSuzanne.size()), GL_UNSIGNED_SHORT, (void *)0);
//        
//        
//        // Begin Floor
//        glm::mat4 ModelFloor = glm::translate(glm::mat4(1.0), glm::vec3(0, -2.5f, 0));
//        glm::mat4 MVPFloor = getProjectionMatrix() * getViewMatrix() * ModelFloor;
//        
//        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPFloor[0][0]);
//        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelFloor[0][0]);
//        
//        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferFloor);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glEnableVertexAttribArray(1);
//		glBindBuffer(GL_ARRAY_BUFFER, uvBufferFloor);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glEnableVertexAttribArray(2);
//		glBindBuffer(GL_ARRAY_BUFFER, normalBufferFloor);
//		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferFloor);
//        
//		glDrawElements(GL_TRIANGLES, static_cast<int>(indicesFloor.size()), GL_UNSIGNED_SHORT, (void *)0);
        
//        glDisableVertexAttribArray(0);
//        glDisableVertexAttribArray(1);
//        glDisableVertexAttribArray(2);
        
        //END RENDERING
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &vertexBufferSuzanne);
    glDeleteBuffers(1, &uvBufferSuzanne);
    glDeleteBuffers(1, &normalBufferSuzanne);
    glDeleteBuffers(1, &elementBufferSuzanne);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &vertexArrayID);
    
    glfwTerminate();
    return 0;
}

