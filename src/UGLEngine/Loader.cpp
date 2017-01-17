//
//  Loader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//

#include "Loader.h"

RawModel* Loader::LoadToVao(std::vector<glm::vec3> positions, std::vector<glm::vec2>textureCoords, std::vector<uint> indices) {
    int vaoID = createVAO();
    BindIndicesBuffer(indices);
    StoreDataInAttributeList(0, positions);
    StoreDataInAttributeList(1, textureCoords);
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
void Loader::StoreDataInAttributeList(int attributeIndex, std::vector<glm::vec3> data) {
    GLuint vboID;
    vbos.push_back(vboID);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, false, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Loader::StoreDataInAttributeList(int attributeIndex, std::vector<glm::vec2> data) {
    GLuint vboID;
    vbos.push_back(vboID);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, false, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::UnbindVAO() {
    glBindVertexArray(0);
}

void Loader::BindIndicesBuffer(std::vector<uint> indices) {
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
    for (std::list<GLuint>::iterator it = textures.begin(); it != textures.end(); ++it) {
        GLuint id = *it;
        glDeleteTextures(1, &id);
    }
}

int Loader::LoadTexture(std::string fileName) {
    
    const char *imagepath = fileName.c_str();
    printf("Reading image %s\n", imagepath);
    
    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;
    
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;
    }
    
    // Read the header, i.e. the 54 first bytes
    
    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 )
    {
        printf("Not a correct BMP file: Header is less than 54 bytes\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' )
    {
        printf("Not a correct BMP file: Does not begin with BM\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )
    {
        printf("Not a correct BMP file: Compression was used at level: %d\n", *(int*)&(header[0x1E]));
        return 0;
    }
    if ( *(int*)&(header[0x1C])!=24 )
    {
        printf("Not a correct BMP file: Not 24bpp\n");
        return 0;
    }
    
    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)
        imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)
        dataPos=54; // The BMP header is done that way
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    // Everything is in memory now, the file wan be closed
    fclose (file);
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    // OpenGL has now copied the data. Free our own version
    delete [] data;
    
    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Return the ID of the texture we just created
    return textureID;
}
