#version 410 core

layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV;
layout(location=2) in vec3 vertexNormal_modelspace;
layout(location=3) in vec3 vertexTangent_modelspace;
layout(location=4) in vec3 vertexBitangent_modelspace;
//layout(location=1) in vec3 vertexColor;

//Constant Values for each runthrough of the shader.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

//Outputted data that will be interpolated for each fragment.
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 Tangent_cameraspace;
out vec3 Bitangent_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec2 UV;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    
    Position_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;
    
    vec3 vertexPosition_cameraspace = (V * M *vec4(vertexPosition_modelspace, 1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    
    vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    
    Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;
    Tangent_cameraspace = (V * M * vec4(vertexTangent_modelspace, 0)).xyz;
    Bitangent_cameraspace = (V * M * vec4(vertexBitangent_modelspace, 0)).xyz;
    
    UV = vertexUV;
}