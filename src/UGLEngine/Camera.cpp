//
//  Camera.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Camera.h"

void Camera::Move() {
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
        this->position.z -= 0.02f;
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->position.z += 0.02f;
    if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
        this->position.x += 0.02f;
    if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
        this->position.x -= 0.02f;
    
    this->_calculateViewMatrix();
}

void Camera::_calculateViewMatrix() {
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->GetPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->GetYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->GetRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 cameraPos = this->GetPosition();
    glm::vec3 negCameraPos = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
    viewMatrix = glm::translate(viewMatrix, negCameraPos);
    this->viewMatrix = viewMatrix;
    
    
//    this->viewMatrix = glm::lookAt(this->position,    //Camera Position
//                                   glm::vec3(0,0,-10), //Camera Looking At
//                                   glm::vec3(0,1,0));    //Head is up
}

glm::mat4 Camera::GetViewMatrix() {
    return this->viewMatrix;
}

glm::vec3 Camera::GetPosition() {
    return this->position;
}

float Camera::GetPitch() {
    return this->pitch;
}

float Camera::GetYaw() {
    return this->yaw;
}

float Camera::GetRoll() {
    return this->roll;
}
