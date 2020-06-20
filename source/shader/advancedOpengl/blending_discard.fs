#version 330 core
out vec4 FragColor;


uniform sampler2D texture1;
in vec2 TexCoords;


//void main()
//{
//	vec4 texColor = texture(texture1, TexCoords);
//	if(texColor.w < 0.1)
//	{
//		discard;
//	}
//
//	FragColor = texColor;
//}

void main()
{
	FragColor = texture(texture1, TexCoords);
}