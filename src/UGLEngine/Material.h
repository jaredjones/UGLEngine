#pragma once
#include <vector>

#include "shader.h"

struct TextureHandleUniformHandlePair{
	GLuint textureHandle;
	GLuint uniformHandle;
};

class Material
{
public:
	Shader* shader;
	std::vector<TextureHandleUniformHandlePair> pipes;

	Material();

	void mount();
};