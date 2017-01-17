//
//  Entity.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>
#include <glm.hpp>
#include "TexturedModel.h"

class Entity {
private:
    TexturedModel *model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
public:
    Entity(TexturedModel *model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        this->model = model;
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }
    
    //Getters/Setters
    void SetTexturedModel(TexturedModel *model);
    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rot);
    void SetScale(glm::vec3 scale);
    TexturedModel* GetTexturedModel();
    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();
    
    void IncreasePosition(glm::vec3 delta);
    void IncreateRotation(glm::vec3 delta);
};

#endif /* Entity_h */
