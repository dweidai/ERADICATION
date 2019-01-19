#version 330 core

in vec2 TexCoord;

uniform sampler2D shadowMap;

out vec4 fragColor;

void main()
{	
	//fragColor = vec4(1,1,1,1);
	fragColor = texture(shadowMap, TexCoord);
}
