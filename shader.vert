#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

out vec2 TexCoord;
out vec4 FragPosLightSpace;

void main()
{	
	vec3 FragPos = vec3(model * vec4(position, 1.0));
	gl_Position = projection * view * model * vec4(position, 1.0);
	TexCoord = texCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
