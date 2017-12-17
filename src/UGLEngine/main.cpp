//
//  main.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#define GLM_FORCE_RADIANS

#include <iostream>
#include <thread>
#include <atomic>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <trigonometric.hpp>
#include <gtx/transform.hpp>

#include <Common.h>
#include <Utilities.h>
#include "WorldModelContainer.h"
#include "Model3D.h"
#include "CameraController.h"
#include "OBJLoader.h"
#include "VBOIndexer.h"
#include "Loader.h"
#include "EntityRenderer.h"
#include "StaticShader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "Camera.h"
#include "Light.h"
#include "MasterRenderer.h"
#include "Terrain.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

int main(int argc, const char * argv[])
{
    printf("%s-%s-%s\n", CONFIG, PLATFORM_TEXT, ARCH);
    ApplePathSetup();
    
    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialize!" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "UGLEngine", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW failed to create window!" << std::endl;
        return -1;
    }
    
    
    if (strcmp(CONFIG, "Debug") == 0)
    {
        chdir(CURRENT_WORKING_DIRECTORY);
    }
    
    char * dir = getcwd(NULL, 0);
    std::cout << "Current dir: " << dir << std::endl;
    free(dir);
    
    //Make this thread and window writable for OpenGL Calls
    glfwMakeContextCurrent(window);
    
    //Enable GLEW and allow Expiremental Features (Important)
    glewExperimental = GL_TRUE;
    glewInit();
    
    
    //Enable Vertical Syncronization
    //glfwSwapInterval(1);
    
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    
    // The version is formatted as <version number><space><vendor-specific information>,
    // where <version number> is a MAJOR.MINOR format, with an optional release number.
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    Loader *loader = new Loader();
    
 
    uIntStorage indices;
    vec3Storage vertices;
    vec2Storage uvs;
    vec3Storage normals;
    vec3Storage tangents;
    vec3Storage bitangents;
    
    loadOBJ("Resources/dragon.obj", indices, vertices, uvs, normals, tangents, bitangents);
    
    
    RawModel *dragonModel = loader->LoadToVao(vertices,
                                              uvs,
                                              normals,
                                              indices);
    
    uIntStorage indices1;
    vec3Storage vertices1;
    vec2Storage uvs1;
    vec3Storage normals1;
    vec3Storage tangents1;
    vec3Storage bitangents1;
    loadOBJ("Resources/grassModel.obj", indices1, vertices1, uvs1, normals1, tangents1, bitangents1);
    RawModel *grassModel = loader->LoadToVao(vertices1,
                                                uvs1,
                                                normals1,
                                                indices1);
    
    
    ModelTexture *dwarfTex = new ModelTexture(loader->LoadTexture("Resources/dwarf.bmp"));
    dwarfTex->SetShineDamper(10.0f);
    dwarfTex->SetReflectivity(1.0f);
    TexturedModel *texturedDragon = new TexturedModel(dragonModel, dwarfTex);
    
    std::vector<Entity *> allDragonEntities;
    
    for (int i = 0; i < 2; i++)
    {
        Entity *entity = new Entity(texturedDragon,
                                    glm::vec3(0.0f, 0.0f, -3.0f - i),    //position
                                    glm::vec3(0.0f, 0.0f, 0.0f),    //rotation
                                    glm::vec3(0.1f, 0.1f, 0.1f));   //scale
        allDragonEntities.push_back(entity);
    }
    
    ModelTexture *grassTex = new ModelTexture(loader->LoadTexture("Resources/grass.dds"));
    grassTex->SetShineDamper(10.0f);
    grassTex->SetReflectivity(1.0f);
    grassTex->SetForcedUpdatedNormals(true);
    grassTex->SetTransparency(true);
    TexturedModel *texturedGrass = new TexturedModel(grassModel, grassTex);
    Entity *grass = new Entity(texturedGrass, glm::vec3(1.0f, 0.12f, -1.0f),    //position
                               glm::vec3(180.0f, 0.0f, 0.0f),    //rotation
                               glm::vec3(0.1f, 0.1f, 0.1f));   //scale
    
    
    Terrain *terrain = new Terrain(0, 0, loader, new ModelTexture(loader->LoadTexture("Resources/stone.bmp")));
    Terrain *terrain2 = new Terrain(1, 0, loader, new ModelTexture(loader->LoadTexture("Resources/stone.bmp")));
    
    Light *light = new Light(glm::vec3(2, 1, -2), glm::vec3(1,1,1));
    Camera *camera = new Camera(window);
    
    MasterRenderer *renderer = new MasterRenderer();
    while(!glfwWindowShouldClose(window))
    {
        //Poll events for stuff like the keyboard, mouse, trackpad, etc.
        glfwPollEvents();
        
        camera->Move();
        
        for(std::vector<Entity *>::size_type i = 0; i != allDragonEntities.size(); i++) {
            renderer->ProcessEntity(allDragonEntities[i]);
            allDragonEntities[i]->IncreateRotation(glm::vec3(0,1,0));
        }
        renderer->ProcessTerrain(terrain);
        renderer->ProcessTerrain(terrain2);
        renderer->ProcessEntity(grass);
        
        renderer->Render(light, camera);
        //entity->IncreasePosition(glm::vec3(0.002, 0, 0));
        //entity->IncreateRotation(glm::vec3(0,1,0));
        glfwSwapBuffers(window);
    }
    
    loader->CleanUp();
    renderer->CleanUp();
    
    delete loader;
    
    glfwTerminate();
    return 0;
}

