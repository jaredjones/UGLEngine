//
//  CameraController.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/14/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include "Common.h"
#include "CameraController.h"
#include <math.h>
#include <GLFW/glfw3.h>

glm::mat4 ViewMatrix = glm::lookAt(
                                   glm::vec3(0,0,0), //camera position
                                   glm::vec3(0,0,0), //Camera looking at
                                   glm::vec3(0,1,0) //Head is up
                                   );
glm::mat4 ViewMatrixWithoutTranslation = ViewMatrix;
glm::mat4 ProjectionMatrix = glm::perspective(90.0f, 800.0f / 600.f, 0.1f, 100.0f);

// Initial position : on +Z
glm::vec3 position = glm::vec3( 1, 1, 4 );
// Initial horizontal angle : toward -Z
double horizontalAngle = -2.8f;
// Initial vertical angle : none
double verticalAngle = 0.0f;
// Initial Field of View
double initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

double lastXPos;
double lastYPos;


void computeMatricesFromInputs(GLFWwindow* window)
{
    //printf("H:%f V:%f\n", horizontalAngle, verticalAngle);
    //printf("X:%fY:%fZ:%f\n", position.x, position.y, position.z);
    static double lastTime = glfwGetTime();
    
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    //glfwSetCursorPos(window, 800/2, 600/2);
    
    horizontalAngle += mouseSpeed * 0;
    verticalAngle += mouseSpeed * 0;//0 makes no angle change. Modify 0 to take on angle change.
    
    glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle));
    
    glm::vec3 right = glm::vec3(sin(horizontalAngle - M_PI_2), 0, cos(horizontalAngle - M_PI_2));
    
    glm::vec3 up = glm::cross(right, direction);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)//Strafe Right
    {
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)//Strafe Left
    {
        position -= right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        //BUG: Not only going up on Y, it's also going up based on direction of verticalangle
        position += up *deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        //BUG: Not only going down on Y, it's also going down based on direction of verticalangle
        position -= up *deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        horizontalAngle += mouseSpeed * 4;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        horizontalAngle -= mouseSpeed * 4;
    }
    
    double curxpos, curypos;
    glfwGetCursorPos(window, &curxpos, &curypos);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        //printf("XC:%f, YC:%f\n", curxpos - lastXPos,curypos-lastYPos);
        //printf("Vert:%f\n", verticalAngle);
        horizontalAngle += mouseSpeed * (curxpos - lastXPos);
        verticalAngle += mouseSpeed * (curypos-lastYPos);
        
        //Vertical View should be between -PI/2 and PI/2
        if (verticalAngle >= M_PI_2)
            verticalAngle = M_PI_2;
        if (verticalAngle <= -M_PI_2)
            verticalAngle = -M_PI_2;
        if (horizontalAngle >= M_PI)
            horizontalAngle = -M_PI;
        else if (horizontalAngle <= -M_PI)
            horizontalAngle = M_PI;
    }
    
    float FoV = initialFoV;// - 5 * glfwGetMouseWheel();
    ProjectionMatrix = glm::perspective(FoV, 800.0f/600.0f, 0.1f, 100.0f);
    ViewMatrix = glm::lookAt(position, position+direction, up);
    ViewMatrixWithoutTranslation = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), direction, up);
    
    lastXPos = curxpos;
    lastYPos = curypos;
    lastTime = currentTime;
    
}
glm::mat4 getViewMatrixWithoutTranslation()
{
    return ViewMatrixWithoutTranslation;
}
glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}