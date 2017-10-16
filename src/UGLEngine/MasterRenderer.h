//
//  MasterRenderer.h
//  uglengine
//
//  Created by Jared Jones on 10/15/17.
//

#ifndef __UGLENGINE_MasterRenderer_h
#define __UGLENGINE_MasterRenderer_h

#include <stdio.h>
#include <unordered_map>
#include <list>
#include "StaticShader.h"
#include "Renderer.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"

class MasterRenderer {
    StaticShader *shader;
    Renderer *renderer;
    std::unordered_map<TexturedModel*, std::list<Entity*>> *entities;
    
public:
    MasterRenderer()
    {
        shader = new StaticShader();
        renderer = new Renderer(shader);
        entities = new std::unordered_map<TexturedModel*, std::list<Entity*>>();
    }
    
    ~MasterRenderer()
    {
        delete shader;
        delete renderer;
        delete entities;
    }
    
    void Render(Light *sun, Camera* camera);
    void ProcessEntity(Entity *entity);
    void CleanUp();
};


#endif /* MasterRenderer_h */
