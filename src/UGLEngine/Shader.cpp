#include "Shader.h"

Shader::Shader(std::string name, std::string location)
{
	// LOAD SHADER CODE
	std::string vertShaderCode;
	bool vertShaderRead = readFileIntoString(location + "/" + name + ".vsh", vertShaderCode);
	if (!vertShaderRead){}; //ignore this error condition
	const char* vertShaderCodePointer = vertShaderCode.c_str();

	std::string fragShaderCode;
	bool fragShaderRead = readFileIntoString(location + "/" + name + ".fsh", fragShaderCode);
	if (!fragShaderRead){}; //ignore this error condition
	const char* fragShaderCodePointer = fragShaderCode.c_str();


	// COMPILE SHADERS

	//compile vertex shader
	printf("Compiling shader : %s\n", (location + "/" + name + ".vsh").c_str());
	vertShaderHandle = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertShaderHandle, 1, &vertShaderCodePointer, NULL);
	glCompileShader(vertShaderHandle);

	//compile fragment shader
	printf("Compiling shader : %s\n", (location + "/" + name + ".fsh").c_str());
	fragShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShaderHandle, 1, &fragShaderCodePointer, NULL);
	glCompileShader(fragShaderHandle);



	// CHECK SHADERS
	int  compilerInfoLogLength;
	

	//check vertex shader
	glGetShaderiv(vertShaderHandle, GL_INFO_LOG_LENGTH, &compilerInfoLogLength);
	std::vector<char> VcompilerInfoMessage(compilerInfoLogLength);
	glGetShaderInfoLog(vertShaderHandle, compilerInfoLogLength, NULL, &VcompilerInfoMessage[0]);
	if (&VcompilerInfoMessage[0] != nullptr)
	{
		fprintf(stdout, "%s\n", &VcompilerInfoMessage[0]);
	}

	//check fragment shader
	glGetShaderiv(fragShaderHandle, GL_INFO_LOG_LENGTH, &compilerInfoLogLength);
	std::vector<char> FcompilerInfoMessage(compilerInfoLogLength);
	glGetShaderInfoLog(fragShaderHandle, compilerInfoLogLength, NULL, &FcompilerInfoMessage[0]);
	if (&FcompilerInfoMessage[0] != nullptr)
	{
		fprintf(stdout, "%s\n", &FcompilerInfoMessage[0]);
	}



	// LINK PROGRAM
	fprintf(stdout, "Linking shader program\n");
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertShaderHandle);
	glAttachShader(programHandle, fragShaderHandle);
	glLinkProgram(programHandle);

	//check program
	glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &compilerInfoLogLength);
	std::vector<char> PcompilerInfoMessage(compilerInfoLogLength);
	glGetProgramInfoLog(programHandle, compilerInfoLogLength, NULL, &PcompilerInfoMessage[0]);
	if (&PcompilerInfoMessage[0] != nullptr)
	{
		fprintf(stdout, "%s\n", &PcompilerInfoMessage[0]);
	}
}

Shader::~Shader()
{
	glDeleteShader(vertShaderHandle);
	glDeleteShader(fragShaderHandle);
	glDeleteProgram(programHandle);
}

void Shader::mount()
{
	glUseProgram(programHandle);
}

GLuint Shader::getUniformHandle(std::string name)
{
	return glGetUniformLocation(programHandle, name.c_str());
}

void Shader::bind(glm::mat4x4 matrix, GLuint handle)
{
	glUniformMatrix4fv(handle, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::bind(glm::vec3 vector, GLuint handle)
{
	glUniform3fv(handle, 1, &vector[0]);
}


//wrappers
void Shader::bind(glm::mat4x4 matrix, std::string name)
{
	bind(matrix, getUniformHandle(name));
}
void Shader::bind(glm::vec3 vector, std::string name)
{
	bind(vector, getUniformHandle(name));
}