#version 460 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D image;
uniform float opacityMod;
void main()
{
	
	FragColor = texture(image, TexCoord);
	
}