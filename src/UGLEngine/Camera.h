//
//  Camera.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include <glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
    GLFWwindow *window = nullptr;
    glm::mat4 viewMatrix;
    glm::vec3 position;
    float pitch;    //Angle of Y (Up/Down)
    float yaw;      //Angle of X (Left/Right)
    float roll;     //How tilted the camera is. (Rotation along Z)
    
    void _calculateViewMatrix();
public:
    Camera(GLFWwindow *window) {
        this->window = window;
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->pitch = 0.0f;
        this->yaw = 0.0f;
        this->roll = 0.0f;
    }
    
    glm::mat4 GetViewMatrix();
    
    void Move();
    
    glm::vec3 GetPosition();
    float GetPitch();
    float GetYaw();
    float GetRoll();
};

#endif /* Camera_h */
