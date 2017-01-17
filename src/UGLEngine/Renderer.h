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
#include <TexturedModel.h>

class Renderer {
public:
    Renderer();
    void Render(TexturedModel *model);
    void Prepare();
private:
    
};

#endif /* Renderer_h */
