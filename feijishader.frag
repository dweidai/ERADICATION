#version 330 core

out vec4 FragColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(ourColor,1);
    FragColor = texture(ourTexture, TexCoord);
    //FragColor = vec4(0.0f, 0.f,1.0f,1);
}
