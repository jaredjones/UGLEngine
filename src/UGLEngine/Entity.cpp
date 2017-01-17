//
//  Entity.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "Entity.h"

void Entity::IncreasePosition(glm::vec3 delta) {
    this->position += delta;
}
void Entity::IncreateRotation(glm::vec3 delta) {
    this->rotation += delta;
}

void Entity::SetTexturedModel(TexturedModel *model) {
    this->model = model;
}

void Entity::SetPosition(glm::vec3 position) {
    this->position = position;
}

void Entity::SetRotation(glm::vec3 rot) {
    this->rotation = rot;
}

void Entity::SetScale(glm::vec3 scale) {
    this->scale = scale;
}

TexturedModel* Entity::GetTexturedModel() {
    return model;
}

glm::vec3 Entity::GetPosition() {
    return position;
}

glm::vec3 Entity::GetRotation() {
    return rotation;
}

glm::vec3 Entity::GetScale() {
    return scale;
}
