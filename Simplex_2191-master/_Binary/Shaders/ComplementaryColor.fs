#version 330

in vec3 Color;

uniform vec3 ComplementaryColor = vec3(-1,-1,-1);

out vec4 Fragment;

void main()
{
	Fragment = vec4(vec3(1 - Color.r, 1 - Color.g, 1 - Color.b),1);

	if(ComplementaryColor.r != -1.0 && ComplementaryColor.g != -1.0 && ComplementaryColor.b != -1.0)
	{		
		Fragment = vec4(vec3(1 - ComplementaryColor.r, 1 - ComplementaryColor.g, 1 - ComplementaryColor.b), 1);
	}

	return;
}