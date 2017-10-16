//
//  MasterRenderer.cpp
//  uglengine
//
//  Created by Jared Jones on 10/15/17.
//

#include "MasterRenderer.h"

void MasterRenderer::Render(Light *sun, Camera *camera)
{
    this->renderer->Prepare();
    this->shader->Start();
    this->shader->LoadLight(sun);
    this->shader->LoadViewMatrix(camera->GetViewMatrix());
    this->renderer->Render(this->entities);
    this->shader->Stop();
    this->entities->clear();
}

void MasterRenderer::ProcessEntity(Entity *entity)
{
    TexturedModel *entityModel = entity->GetTexturedModel();
    
    auto search = this->entities->find(entityModel);
    if(search != this->entities->end()) {
        std::list<Entity *> batch = search->second;
        batch.push_back(entity);
    } else {
        std::list<Entity *> *newBatch = new std::list<Entity *>();
        newBatch->push_back(entity);
        this->entities->insert(std::make_pair(entityModel, *newBatch));
    }
}

void MasterRenderer::CleanUp()
{
    
}
