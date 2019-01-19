#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4(ourColor, 0.0) + vec4(0.1f,0.1f,0.1f,1.0);
}
