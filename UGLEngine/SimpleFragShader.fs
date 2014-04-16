#version 410 core

in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec2 UV;

uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

out vec4 color;


void main()
{
    vec3 text = texture(myTextureSampler, UV).rgb;
    
    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    
    float cosTheta = clamp(dot(n, l),0,1);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    color = vec4( lightColor *cosTheta, 1.0);
}