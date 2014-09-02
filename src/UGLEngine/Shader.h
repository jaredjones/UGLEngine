#pragma once

#include <Common.h>
#include <fstream>

#include "FileUtils.h"

class Shader{
public:
	GLuint vertShaderHandle;
	GLuint fragShaderHandle;

	GLuint programHandle; //the handle for the final, linked program

	Shader(std::string name, std::string location);
	~Shader();

	GLuint getUniformHandle(std::string name);

	void bind(glm::mat4x4 matrix, std::string name);
	void bind(glm::mat4x4 matrix, GLuint handle);
	void bind(glm::vec3 vector, std::string name);
	void bind(glm::vec3 vector, GLuint handle);


	void mount(); //makes the required GL calls to put the shader into the state machine
};