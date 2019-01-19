#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;


out vec2 TexCoord;

void main()
{	
	gl_Position = vec4(position.x * 0.2 - 0.8, -position.z * 0.2 - 0.8, 0.0, 1.0);
	TexCoord = texCoord;
}
