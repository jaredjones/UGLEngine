#version 410 core

in vec2 UV;
in vec3 position_eye;
in vec3 normal_eye;

uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

out vec4 color;


// fixed point light
vec3 light_position_world = vec3(0.0, 0.0, 7.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 La = vec3 (0.2, 0.2, 0.2); // grey ambient colour

// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 100.0; // specular 'power'

void main()
{
    //ambient intensity
    vec3 la = La * Ka;
    
    //difuse intensity
    vec3 ld = vec3 (0.0, 0.0, 0.0);//replace later
    
    //specular intensity
    vec3 ls = vec3 (0.0, 0.0, 0.0);//replace later
    
    vec3 text = texture(myTextureSampler, UV).rgb;
    
    color = vec4(ls + ld + la * text, 1.0);
}