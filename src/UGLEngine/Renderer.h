//
//  Renderer.h
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#ifndef Renderer_h
#define Renderer_h

#include <stdio.h>
#include <RawModel.h>

class Renderer {
public:
    Renderer();
    void Render(RawModel *model);
    void Prepare();
private:
    
};

#endif /* Renderer_h */
