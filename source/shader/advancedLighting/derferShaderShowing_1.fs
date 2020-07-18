#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;



void main()
{             
	float spec = texture(texture_diffuse1, TexCoords).a;
	FragColor = vec4(spec * vec3(texture(texture_diffuse1, TexCoords).r), 1.0);
}
