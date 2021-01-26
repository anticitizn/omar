#version 330 core
in vec4 textColor;
in vec4 tileColor;

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D fontTexture;

void main()
{
	fragColor = tileColor + texture(fontTexture, texCoords) * textColor;
}
