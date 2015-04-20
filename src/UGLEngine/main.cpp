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

std::atomic<bool> Closing;

// Function prototypes
void logic();
void render(GLFWwindow *w);

int main(int argc, const char * argv[])
{
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
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "UGLEngine", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW failed to create window!" << std::endl;
        return -1;
    }
    
    //Start Render and Logic threads.
    std::thread ren(render, window);
    std::thread log(logic);
    
    
#ifdef DEBUG
    chdir(CURRENT_WORKING_DIRECTORY);
#endif
    char * dir = getcwd(NULL, 0);
    std::cout << "Current dir: " << dir << std::endl;
    
    while(!glfwWindowShouldClose(window))
    {
        //Poll events for stuff like the keyboard, mouse, trackpad, etc.
        glfwPollEvents();
        
        //Slow the main thread down since not much happens here at the moment
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    //Main thread has finished, so we must die.
    Closing = true;
    
    log.join();
    ren.join();
    
    glfwTerminate();
    return 0;
}

void render(GLFWwindow *w)
{
    bool inited = false;
    
    //Renderer Loop
    while (!Closing)
    {
        //Perform Initial Renderer Setup
        if (!inited)
        {
            //Make this thread and window writable for OpenGL Calls
            glfwMakeContextCurrent(w);
            
            //Enable GLEW and allow Expiremental Features (Important)
            glewExperimental = GL_TRUE;
            glewInit();
            
            //Enable Vertical Syncronization
            glfwSwapInterval(1);
            
            std::cout << glGetString(GL_VENDOR) << std::endl;
            std::cout << glGetString(GL_VERSION) << std::endl;
            std::cout << glGetString(GL_RENDERER) << std::endl;
            
            // The version is formatted as <version number><space><vendor-specific information>,
            // where <version number> is a MAJOR.MINOR format, with an optional release number.
            std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
            
            //Initalize the World Model Container
            sWMC.Init();
            sWMC.CompileAndStoreShader("lit", "Shaders/Lit.vsh", "Shaders/Lit.fsh");
            sWMC.CompileAndStoreShader("skybox", "Shaders/Skybox.vsh", "Shaders/Skybox.fsh");
            
            inited = true;
        }
        //Set the background color that is used when glClear is called
        glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
        //Clear both the depth and color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //RENDERING STARTS HERE
        
        //Bring everything we just drew onto the screen
        glfwSwapBuffers(w);
    }
}

void logic()
{
    while (!Closing)
    {
        // Slow down thread since it's not doing anything
        // WARNING: If you halt this thread the program will not close until this thread exits.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

