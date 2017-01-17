//
//  Renderer.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef Renderer_h
#define Renderer_h

#include <stdio.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <RawModel.h>
#include <TexturedModel.h>
#include "Entity.h"
#include "StaticShader.h"

class Renderer {
private:
    glm::mat4 projectionMatrix;
public:
    Renderer(StaticShader *shader);
    void Render(Entity *entity, StaticShader *shader);
    void Prepare();
    
};

#endif /* Renderer_h */
