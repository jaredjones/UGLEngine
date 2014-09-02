#pragma once 
#include <iostream>
#include <vector>

#include <Common.h>

#include "ScenegraphNode.h"
#include "OBJLoader.h"
#include "Shader.h"

struct SceneGlobals
{
	glm::mat4x4 viewProjectionMatrix; //premultiplied
	glm::mat4x4 viewMatrix;
	glm::vec3 lightPos;
};

class Model : public ScenegraphNode
{
private:
	//handles for mesh data
	GLuint vertexBufHandle;
	GLuint uvBufHandle;
	GLuint normalBufHandle;
	GLuint tangentBufHandle;
	GLuint bitangentBufHandle;
	GLuint indexBufHandle; //face's vertex indices, each group of 3 is a triangle
	GLuint vertexArrayObjectHandle; //the handle for the GPU object that contains all our other buffers

	GLsizei numberOfIndices;

	Shader* shader;
	SceneGlobals* sceneGlobals;

	//these are specific to the vertex shader we are using
	GLuint shaderModelMatrixUniformHandle;
	GLuint shaderModelViewProjectionMatrixUniformHandle;
	GLuint shaderViewMatrixUniformHandle;
	GLuint shaderLightPositionUniformHandle;
	

public:
	Model(std::string path, Shader* shader, SceneGlobals* sceneGlobals);
    ~Model();

	virtual void drawSelf(glm::mat4x4 absoluteMatrix);
};