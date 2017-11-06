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
#include "EntityRenderer.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"

class MasterRenderer {
    const float FOV = 70.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 1000.0f;
    
    StaticShader *shader;
    EntityRenderer *renderer;
    std::unordered_map<TexturedModel*, std::list<Entity*> *> *entities;
    
    TerrainShader *terrainShader;
    TerrainRenderer *terrainRenderer;
    std::list<Terrain *> *terrains;
    
    glm::mat4 projectionMatrix;
    
public:
    MasterRenderer()
    {
        glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        projectionMatrix = glm::perspective(FOV, 800.0f/ 600.0f, NEAR_PLANE, FAR_PLANE);
        shader = new StaticShader();
        renderer = new EntityRenderer(shader, projectionMatrix);
        entities = new std::unordered_map<TexturedModel*, std::list<Entity*> *>();
        
        terrainShader = new TerrainShader();
        terrainRenderer = new TerrainRenderer(terrainShader, projectionMatrix);
        terrains = new std::list<Terrain *>();
        
    }
    
    ~MasterRenderer()
    {
        delete shader;
        delete renderer;
        delete entities;
        delete terrainShader;
        delete terrainRenderer;
    }
    
    void Render(Light *sun, Camera* camera);
    void ProcessEntity(Entity *entity);
    void ProcessTerrain(Terrain *terrain);
    void Prepare();
    void CleanUp();
};


#endif /* MasterRenderer_h */
