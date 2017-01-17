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

void Renderer::Render(TexturedModel *model) {
    RawModel *rawModel = model->getRawModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->getModelTexture()->getTextureID());
    glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::Prepare() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
