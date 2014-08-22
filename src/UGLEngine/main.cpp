//
//  main.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#define GLM_FORCE_RADIANS

#include <Common.h>

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <trigonometric.hpp>
#include <gtx/transform.hpp>

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
    sWMC.CompileAndStoreShader("lit", "Shaders/Lit.vsh", "Shaders/Lit.fsh");
    sWMC.CompileAndStoreShader("skybox", "Shaders/Skybox.vsh", "Shaders/Skybox.fsh");
    
    GLuint programID = sWMC.GetShader("lit");
    //Model3D *myTest = new Model3D("Resources/Models/trashcan.wvf", false);
    
    //GLuint TrashcanTexture = loadDDS("Resources/Images/uvmap.DDS");
    GLuint TrashcanTexture = loadBMP_custom("Resources/Images/trashcan.bmp");
    
    //Create a handle for the uniforms
    GLuint Lit_MVPID = glGetUniformLocation(programID, "MVP");
    GLuint Lit_ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint Lit_ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint Lit_LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint Lit_TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    programID = sWMC.GetShader("skybox");
    GLuint SkyboxTexture = loadBMP_custom("Resources/Images/JaredSkybox.bmp");
    //GLuint SkyboxTexture = loadDDS("Resources/Images/skybox.dds");
    
    //Create a handle for the uniforms
    GLuint Skybox_MVPID = glGetUniformLocation(programID, "MVP");
    GLuint Skybox_TextureID = glGetUniformLocation(programID, "myTextureSampler");
    
    //OBJ BEGIN TEST
    vec3Storage vertices;
    vec2Storage uvs;
    vec3Storage normals;
    bool hasQuads;
    uShortStorage vboIndices;
    vec3Storage indexedVertices;
    vec2Storage indexedUvs;
    vec3Storage indexedNormals;
    
    loadOBJ("Resources/Models/trashcan.wvf", vertices, uvs, normals, hasQuads);
    indexVBO(vertices, uvs, normals, vboIndices, indexedVertices, indexedUvs, indexedNormals);
    
    GLuint trashVertexBuffer;
	glGenBuffers(1, &trashVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trashVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
    
    GLuint trashUvBuffer;
	glGenBuffers(1, &trashUvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trashUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
    
    GLuint trashNormalBuffer;
	glGenBuffers(1, &trashNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trashNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
    
    GLuint trashIndexBuffer;
	glGenBuffers(1, &trashIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trashIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned short), &vboIndices[0], GL_STATIC_DRAW);
    
    GLsizei TrashcanIndiciesSize = (GLsizei)vboIndices.size();
    //OBJ END TEST

    
    //Building VAO
    GLuint trashCanVAO;
    glGenVertexArrays(1, &trashCanVAO);
    glBindVertexArray(trashCanVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, trashVertexBuffer);
    glEnableVertexAttribArray(0);//Open up this Attribute Array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, trashUvBuffer);
    glEnableVertexAttribArray(1);//Open up this Attribute Array
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, trashNormalBuffer);
    glEnableVertexAttribArray(2);//Open up this Attribute Array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trashIndexBuffer);
    
    glBindVertexArray(0);//Nulls out bound vertex array
    
    vertices.clear();
    normals.clear();
    uvs.clear();
    vboIndices.clear();
    indexedVertices.clear();
    indexedNormals.clear();
    indexedUvs.clear();
    
    
    //Skybox
    loadOBJ("Resources/Models/JaredSkybox.wvf", vertices, uvs, normals, hasQuads);
    indexVBO(vertices, uvs, normals, vboIndices, indexedVertices, indexedUvs, indexedNormals);
    
	GLuint skyboxVertexBuffer;
	glGenBuffers(1, &skyboxVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);

	GLuint skyboxUvBuffer;
	glGenBuffers(1, &skyboxUvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);
    
	GLuint skyboxNormalBuffer;
	glGenBuffers(1, &skyboxNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);

	GLuint skyboxIndexBuffer;
	glGenBuffers(1, &skyboxIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned short), &vboIndices[0], GL_STATIC_DRAW);
    
    GLsizei SkyboxIndiciesSize = (GLsizei)vboIndices.size();
    
    //Building VAO
    GLuint SkyboxVAO;
    glGenVertexArrays(1, &SkyboxVAO);
    glBindVertexArray(SkyboxVAO);
    
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
    glEnableVertexAttribArray(0);//Open up this Attribute Array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
	glBindBuffer(GL_ARRAY_BUFFER, skyboxUvBuffer);
    glEnableVertexAttribArray(1);//Open up this Attribute Array
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
	glBindBuffer(GL_ARRAY_BUFFER, skyboxNormalBuffer);
    glEnableVertexAttribArray(2);//Open up this Attribute Array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIndexBuffer);
    
    glBindVertexArray(0);//Nulls out bound vertex array
    
    vertices.clear();
    normals.clear();
    uvs.clear();
    vboIndices.clear();
    indexedVertices.clear();
    indexedNormals.clear();
    indexedUvs.clear();
    
    
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
            printf("FPS:%f ms/F:%f\n", 1/(1.0/double(nbFrames)), 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        if (glfwWindowShouldClose(window))
            break;
        
        computeMatricesFromInputs(window);
        
        //BEGIN RENDERING
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
        
        glUseProgram(sWMC.GetShader("skybox"));
        glm::mat4 ModelSkybox = glm::mat4(1.0f);
        glm::mat4 MVPSkybox = getProjectionMatrix() * getViewMatrixWithoutTranslation() * ModelSkybox; //Don't use ViewMatrix for Skybox
        
        glUniformMatrix4fv(Skybox_MVPID, 1, GL_FALSE, &MVPSkybox[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Skybox_TextureID, 0);
        
        //BindVAO
        glBindVertexArray(SkyboxVAO);
        glDepthMask(GL_FALSE);
        glFrontFace(GL_CW);
        glDrawElements(GL_TRIANGLES, SkyboxIndiciesSize, GL_UNSIGNED_SHORT, 0);
        glFrontFace(GL_CCW);
        glDepthMask(GL_TRUE);
        
        glUseProgram(sWMC.GetShader("lit"));
        
        rotDeg++;
        glm::mat4 ModelCube = glm::mat4(1.0f);
        ModelCube = glm::rotate(ModelCube, glm::radians(rotDeg), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 MVPCube = getProjectionMatrix() * getViewMatrix() * ModelCube;
        
        
        glUniformMatrix4fv(Lit_MVPID, 1, GL_FALSE, &MVPCube[0][0]);
        glUniformMatrix4fv(Lit_ModelMatrixID, 1, GL_FALSE, &ModelCube[0][0]);
        glUniformMatrix4fv(Lit_ViewMatrixID, 1, GL_FALSE, &getViewMatrix()[0][0]);
        
        glm::vec3 lightPos = glm::vec3(4*cos(lightZ),4,4*sin(lightZ));
        lightZ +=.03f;
        
        glUniform3f(Lit_LightID, lightPos.x, lightPos.y, lightPos.z);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TrashcanTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Lit_TextureID, 0);
     
        
        //BindVAO
        glBindVertexArray(trashCanVAO);
        //DrawElements
        glDrawElements(GL_TRIANGLES, TrashcanIndiciesSize, GL_UNSIGNED_SHORT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
	glDeleteBuffers(1, &trashVertexBuffer);
	glDeleteBuffers(1, &trashUvBuffer);
	glDeleteBuffers(1, &trashNormalBuffer);
	glDeleteBuffers(1, &trashIndexBuffer);
    glDeleteBuffers(1, &skyboxVertexBuffer);
    glDeleteBuffers(1, &skyboxUvBuffer);
    glDeleteBuffers(1, &skyboxNormalBuffer);
    glDeleteBuffers(1, &skyboxIndexBuffer);
    glDeleteTextures(1, &TrashcanTexture);
    glDeleteVertexArrays(1, &trashCanVAO);
    glDeleteProgram(programID);
    
    
    glfwTerminate();
    return 0;
}

