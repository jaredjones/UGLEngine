#include "Model.h"
#include "WorldModelContainer.h"
#include "VBOIndexer.h"
#include "OBJLoader.h"

Model::Model(std::string path, Shader* shader, SceneGlobals* sceneGlobals)
{
	//set up shader handles
	
	this->shader = shader;
	this->sceneGlobals = sceneGlobals;

	shaderViewMatrixUniformHandle = shader->getUniformHandle("V");
	shaderLightPositionUniformHandle = shader->getUniformHandle("LightPosition_worldspace");
	shaderModelMatrixUniformHandle = shader->getUniformHandle("M");
	shaderModelViewProjectionMatrixUniformHandle = shader->getUniformHandle("MVP");


	vec3Storage indexedVertices;
	vec2Storage indexedUvs;
	vec3Storage indexedNormals;
	vec3Storage indexedTangents;
	vec3Storage indexedBitangents;

	uIntStorage vboIndices;

	loadOBJ(path, vboIndices, indexedVertices, indexedUvs, indexedNormals, indexedTangents, indexedBitangents);

	//Create vertex buffer objects
	glGenBuffers(1, &vertexBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufHandle);
	glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufHandle);
	glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufHandle);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &tangentBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufHandle);
	glBufferData(GL_ARRAY_BUFFER, indexedTangents.size() * sizeof(glm::vec3), &indexedTangents[0], GL_STATIC_DRAW);
	glGenBuffers(1, &bitangentBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentBufHandle);
	glBufferData(GL_ARRAY_BUFFER, indexedBitangents.size() * sizeof(glm::vec3), &indexedBitangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, indexBufHandle);
	glBufferData(GL_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned int), &vboIndices[0], GL_STATIC_DRAW);

	numberOfIndices = (GLsizei)vboIndices.size();

	//Bind vertex buffer objects to vertex array object
	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufHandle);
	glEnableVertexAttribArray(0);//Open up this Attribute Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvBufHandle);
	glEnableVertexAttribArray(1);//Open up this Attribute Array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufHandle);
	glEnableVertexAttribArray(2);//Open up this Attribute Array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufHandle);
	glEnableVertexAttribArray(3);//Open up this Attribute Array
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, bitangentBufHandle);
	glEnableVertexAttribArray(4);//Open up this Attribute Array
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufHandle);

	glBindVertexArray(0);//done building this array
}

Model::~Model()
{
	glDeleteBuffers(1, &vertexBufHandle);
	glDeleteBuffers(1, &uvBufHandle);
	glDeleteBuffers(1, &normalBufHandle);
	glDeleteBuffers(1, &tangentBufHandle);
	glDeleteBuffers(1, &bitangentBufHandle);
	glDeleteBuffers(1, &indexBufHandle);

	glDeleteVertexArrays(1, &vertexArrayObjectHandle);
}

void Model::drawSelf(glm::mat4x4 absoluteMatrix)
{
	shader->mount();

	glm::mat4 modelViewProjection = sceneGlobals->viewProjectionMatrix * absoluteMatrix;

	shader->bind(absoluteMatrix, shaderModelMatrixUniformHandle);
	shader->bind(modelViewProjection, shaderModelViewProjectionMatrixUniformHandle);
	shader->bind(sceneGlobals->lightPos, shaderLightPositionUniformHandle);
	shader->bind(sceneGlobals->viewMatrix, shaderViewMatrixUniformHandle);

	//BindVAO
	glBindVertexArray(vertexArrayObjectHandle);
	//DrawElements
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
}