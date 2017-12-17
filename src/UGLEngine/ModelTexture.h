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
    float shineDamper = 1.0;
    float reflectivity = 0.0;
    bool hasTransparency = false;
    bool forceUpwardNormals = false;
    
public:
    ModelTexture(GLuint id) {
        textureID = id;
    }
    GLuint getTextureID() {
        return textureID;
    }
    
    float GetShineDamper();
    float GetReflectivity();
    bool HasTransparency();
    bool HasForcedUpwardNormals();
    void SetShineDamper(float damper);
    void SetReflectivity(float reflectivity);
    void SetTransparency(bool transparent);
    void SetForcedUpdatedNormals(bool enabled);
    
};
#endif /* ModelTexture_h */
