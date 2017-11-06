//
//  TerrainRenderer.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef TerrainRenderer_h
#define TerrainRenderer_h

#include <stdio.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <RawModel.h>
#include <TexturedModel.h>
#include "Terrain.h"
#include "TerrainShader.h"

class TerrainRenderer {
private:
    TerrainShader *shader;
public:
    TerrainRenderer(TerrainShader *shader, glm::mat4 projectionMatrix);
    void Render(std::list<Terrain *> *terrains);
    void PrepareTerrain(Terrain *terrain);
    void UnbindTexturedModel();
    void LoadModelMatrix(Terrain *terrain);
    
};

#endif /* TerrainRenderer_h */
