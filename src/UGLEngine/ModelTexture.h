//
//  ModelTexture.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef ModelTexture_h
#define ModelTexture_h

#include <stdio.h>

class ModelTexture {
private:
    GLuint textureID;
    
public:
    ModelTexture(GLuint id) {
        textureID = id;
    }
    GLuint getTextureID() {
        return textureID;
    }
     
    
};
#endif /* ModelTexture_h */
