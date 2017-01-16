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
#include "ImageLoader.h"
#include "Model3D.h"
#include "CameraController.h"
#include "OBJLoader.h"
#include "VBOIndexer.h"
#include "Loader.h"
#include "Renderer.h"
#include "StaticShader.h"

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
    Renderer *renderer = new Renderer();
    StaticShader *staticShader = new StaticShader();
    
    float vertices[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    
    int indices[] = {
        0,1,3,3,1,2
    };
    
    
    RawModel *model = loader->LoadToVao(std::vector<float>(std::begin(vertices), std::end(vertices)),
                                        std::vector<int>(std::begin(indices), std::end(indices)));
    
    while(!glfwWindowShouldClose(window))
    {
        //Poll events for stuff like the keyboard, mouse, trackpad, etc.
        glfwPollEvents();
        
        renderer->Prepare();
        
        staticShader->Start();
        renderer->Render(model);
        staticShader->Stop();
        
        glfwSwapBuffers(window);
    }
    
    loader->CleanUp();
    staticShader->CleanUp();
    
    delete loader;
    delete renderer;
    
    glfwTerminate();
    return 0;
}

