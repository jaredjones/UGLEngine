//
//  RawModel.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef RawModel_h
#define RawModel_h

#include <stdio.h>

class RawModel {
public:
    RawModel(int vaoID, int vertexCount) {
        this->vaoID = vaoID;
        this->vertexCount = vertexCount;
    }
    
    int getVaoID();
    int getVertexCount();
    
private:
    int vaoID;
    int vertexCount;
};

#endif /* RawModel_h */
