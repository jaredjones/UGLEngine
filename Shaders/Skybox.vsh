#version 410 core

layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV;
layout(location=2) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;

out vec2 UV;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    UV = vertexUV;
}