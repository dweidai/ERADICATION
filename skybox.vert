#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 TexCoord;

void main()
{	
	gl_Position = projection * view * model * vec4(position, 1.0);
	TexCoord = position;
}
