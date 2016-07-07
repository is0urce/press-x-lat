#version 330

smooth in vec4 theColor;
smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D font;

void main()
{
	outputColor = theColor;
	outputColor.a *= texture(font, theTexture.xy).r;
}