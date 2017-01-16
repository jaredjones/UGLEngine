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
#include "RawModel.h"

class Loader {
public:
    Loader() {
        
    }
    
    ~Loader() {
    }
    
    RawModel* LoadToVao(std::vector<float> positions, std::vector<int> indices);
    int createVAO();
    void StoreDataInAttributeList(int attributeIndex, std::vector<float> data);
    void UnbindVAO();
    void BindIndicesBuffer(std::vector<int> indices);
    void CleanUp();
private:
    std::list<GLuint> vaos;
    std::list<GLuint> vbos;
};

#endif /* Loader_h */
