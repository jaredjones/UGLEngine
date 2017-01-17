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

class TexturedModel {
private:
    RawModel *rawModel;
    ModelTexture *texture;
    
public:
    TexturedModel(RawModel *model, ModelTexture *texture) {
        this->rawModel = model;
        this->texture = texture;
    }
    
    RawModel* getRawModel() {
        return rawModel;
    }
    
    ModelTexture* getModelTexture() {
        return texture;
    }
};

#endif /* TexturedModel_h */
