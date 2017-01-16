//
//  Loader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#include "Loader.h"

RawModel* Loader::LoadToVao(std::vector<float> positions, std::vector<int> indices) {
    int vaoID = createVAO();
    BindIndicesBuffer(indices);
    StoreDataInAttributeList(0, positions);
    UnbindVAO();
    return new RawModel(vaoID, indices.size());
}
int Loader::createVAO() {
    GLuint vaoID;
    vaos.push_back(vaoID);
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
    
}
void Loader::StoreDataInAttributeList(int attributeIndex, std::vector<float> data) {
    GLuint vboID;
    vbos.push_back(vboID);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, false, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Loader::UnbindVAO() {
    glBindVertexArray(0);
}

void Loader::BindIndicesBuffer(std::vector<int> indices) {
    GLuint vboID;
    vbos.push_back(vboID);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
}

void Loader::CleanUp() {
    for (std::list<GLuint>::iterator it = vaos.begin(); it != vaos.end(); ++it) {
        GLuint id = *it;
        glDeleteVertexArrays(1, &id);
    }
    for (std::list<GLuint>::iterator it = vbos.begin(); it != vbos.end(); ++it) {
        GLuint id = *it;
        glDeleteBuffers(1, &id);
    }
}
