//
//  Light.cpp
//  uglengine
//
//  Created by Jared Jones on 10/15/17.
//

#include "Light.h"

glm::vec3 Light::GetPosition()
{
    return this->position;
}

glm::vec3 Light::GetColor()
{
    return this->color;
}

void Light::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void Light::SetColor(glm::vec3 color)
{
    this->color = color;
}

