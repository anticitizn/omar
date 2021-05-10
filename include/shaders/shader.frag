#version 330 core
in vec3 textColor;
in vec3 tileColor;

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D fontTexture;

void main()
{
	fragColor = vec4(tileColor, 1.0f) + texture(fontTexture, texCoords) * vec4(textColor, 1.0f);
}
