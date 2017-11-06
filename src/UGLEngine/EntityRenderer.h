//
//  EntityRenderer.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

#include <stdio.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <RawModel.h>
#include <TexturedModel.h>
#include "Entity.h"
#include "StaticShader.h"

class EntityRenderer {
private:
    StaticShader *shader;
public:
    EntityRenderer(StaticShader *shader, glm::mat4 projectionMatrix);
    void Render(std::unordered_map<TexturedModel *, std::list<Entity *> *> *entities);
    void PrepareTexturedModel(TexturedModel *model);
    void UnbindTexturedModel();
    void PrepareInstance(Entity *entity);
    
};

#endif /* EntityRenderer_h */
