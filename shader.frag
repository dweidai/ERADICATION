#version 330 core

in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D tex;
uniform sampler2D shadowMap;
uniform float type;
uniform float shadowOn;
uniform vec3 color;

out vec4 fragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = 0.005;
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow * 0.5;
} 

void main()
{	

	float shadow = ShadowCalculation(FragPosLightSpace);

	if(type == 0.0)
		fragColor = texture(tex, TexCoord);
	else
		fragColor = vec4(color, 1.0);

	if(shadowOn == 1.0)
		fragColor = vec4(vec3(fragColor) * (1-shadow), 1.0);
}
