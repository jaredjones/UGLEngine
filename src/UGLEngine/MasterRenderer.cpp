//
//  MasterRenderer.cpp
//  uglengine
//
//  Created by Jared Jones on 10/15/17.
//

#include "MasterRenderer.h"

void MasterRenderer::Render(Light *sun, Camera *camera)
{
    this->Prepare();
    this->shader->Start();
    this->shader->LoadLight(sun);
    this->shader->LoadViewMatrix(camera->GetViewMatrix());
    this->renderer->Render(this->entities);
    this->shader->Stop();
    
    this->terrainShader->Start();
    this->terrainShader->LoadLight(sun);
    this->terrainShader->LoadViewMatrix(camera->GetViewMatrix());
    this->terrainRenderer->Render(this->terrains);
    this->terrainShader->Stop();
    
    this->CleanUp();
}

void MasterRenderer::ProcessEntity(Entity *entity)
{
    TexturedModel *entityModel = entity->GetTexturedModel();
    
    auto search = this->entities->find(entityModel);
    if(search != this->entities->end()) {
        std::list<Entity *> *batch = search->second;
        batch->push_back(entity);
    } else {
        std::list<Entity *> *newBatch = new std::list<Entity *>();
        newBatch->push_back(entity);
        this->entities->insert(std::make_pair(entityModel, newBatch));
    }
}

void MasterRenderer::ProcessTerrain(Terrain *terrain)
{
    this->terrains->push_back(terrain);
}

void MasterRenderer::Prepare() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::CleanUp()
{
    std::for_each(this->entities->begin(), this->entities->end(),
        [](std::pair<TexturedModel *, std::list<Entity *> *> element)
                  {
                      std::list<Entity *> *e = element.second;
                      delete e;
                  });
    
    this->terrains->clear();
    this->entities->clear();
}
