#version 410 core
in vec2 UV;

uniform sampler2D myTextureSampler;

out vec4 color;
void main()
{
    vec3 MaterialDiffuseColor = texture(myTextureSampler, UV).rgb;
    color.rgb = MaterialDiffuseColor;
    color.a = 1.0;
}