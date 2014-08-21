//
//  CameraController.h
//  UGLEngine
//
//  Created by Jared Jones on 4/14/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__CameraController__
#define __UGLEngine__CameraController__

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrixWithoutTranslation();

#endif /* defined(__UGLEngine__CameraController__) */
