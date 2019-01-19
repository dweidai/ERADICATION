#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * model * view * vec4(aPos, 1.0);
    //ourColor = aColor;
    TexCoord = aTexCoord;
}
