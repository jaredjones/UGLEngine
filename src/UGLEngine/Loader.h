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
#include "RawModel.h"

class Loader {
public:
    Loader() {
        
    }
    
    ~Loader() {
    }
    
    RawModel* LoadToVao(std::vector<float> positions, std::vector<float>textureCoords, std::vector<int> indices);
    int LoadTexture(std::string fileName);
    int createVAO();
    void StoreDataInAttributeList(int attributeIndex, int sizePerComponent, std::vector<float> data);
    void UnbindVAO();
    void BindIndicesBuffer(std::vector<int> indices);
    void CleanUp();
private:
    std::list<GLuint> vaos;
    std::list<GLuint> vbos;
    std::list<GLuint> textures;
};

#endif /* Loader_h */
