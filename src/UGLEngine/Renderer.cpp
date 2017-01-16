//
//  Renderer.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#include <iostream>
#include <fstream>
#include "Renderer.h"

Renderer::Renderer() {
    glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
}

void Renderer::Render(RawModel *model) {
    glBindVertexArray(model->getVaoID());
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::Prepare() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
