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
    vec3 lightColor = vec3(1,1,1);
	float lightPower = 50.0f;
    
    vec3 text = texture(myTextureSampler, UV).rgb;
    
    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    
    float cosTheta = clamp(dot(n, l),0,1);
    
    float distance = length( LightPosition_worldspace - Position_worldspace );
    
    vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * text;
    
    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    
    color = vec4(MaterialAmbientColor +
                 text * lightColor * lightPower * cosTheta / (distance*distance) +
                 vec3(0.2,0.2,0.2) * lightColor * lightPower * pow(cosAlpha,5) / (distance*distance), 1.0);
}