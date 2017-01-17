//
//  Loader.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef Loader_h
#define Loader_h

#include <stdio.h>
#include <string>
#include <glm.hpp>
#include "RawModel.h"

class Loader {
public:
    Loader() {
        
    }
    
    ~Loader() {
    }
    
    RawModel* LoadToVao(std::vector<glm::vec3> positions, std::vector<glm::vec2>textureCoords, std::vector<uint> indices);
    int LoadTexture(std::string fileName);
    int createVAO();
    void StoreDataInAttributeList(int attributeIndex, std::vector<glm::vec3> data);
    void StoreDataInAttributeList(int attributeIndex, std::vector<glm::vec2> data);
    void UnbindVAO();
    void BindIndicesBuffer(std::vector<uint> indices);
    void CleanUp();
private:
    std::list<GLuint> vaos;
    std::list<GLuint> vbos;
    std::list<GLuint> textures;
};

#endif /* Loader_h */
