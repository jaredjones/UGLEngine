//
//  Light.h
//  uglengine
//
//  Created by Jared Jones on 10/15/17.
//

#ifndef __UGLEngine__Light_h
#define __UGLEngine__Light_h

#include <stdio.h>
#include <glm.hpp>

class Light {
    glm::vec3 position;
    glm::vec3 color;
    
public:
    Light(glm::vec3 position, glm::vec3 color)
    {
        this->position = position;
        this->color = color;
    }
    glm::vec3 GetPosition();
    glm::vec3 GetColor();
    void SetPosition(glm::vec3 position);
    void SetColor(glm::vec3 color);
};

#endif /* Light_h */
