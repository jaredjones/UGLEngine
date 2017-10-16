//
//  TexturedModel.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef TexturedModel_h
#define TexturedModel_h

#include "RawModel.h"
#include "ModelTexture.h"
#include <stdio.h>
#include <cstdlib>


class TexturedModel {
private:
    RawModel *rawModel;
    ModelTexture *texture;
    
public:
    int hashRand;
    TexturedModel(RawModel *model, ModelTexture *texture) {
        
        this->hashRand = std::rand();
        this->rawModel = model;
        this->texture = texture;
    }
    
    RawModel* getRawModel() {
        return rawModel;
    }
    
    ModelTexture* getModelTexture() {
        return texture;
    }
    
    // Bullshit for implementing hashing for unordered_map on a custom class
    bool operator ==(const TexturedModel & obj) const
    {
        return hashRand == obj.hashRand;
    }
};


// Bullshit for implementing hashing for unordered_map on a custom class
namespace std
{
    template<>
    struct std::hash<TexturedModel>
    {
        size_t
        operator()(const TexturedModel & obj) const
        {
            return std::hash<int>()(obj.hashRand);
        }
    };
}

#endif /* TexturedModel_h */
