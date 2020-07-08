#version 330 core
//layout(location = 0) out vec3 gPosition;
//layout(location =1 ) out vec3 gNormal;
//layout(location = 2) out vec4 gAlbedoSpec;
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in ;

uniform  sampler2D	texture_diffuse1;
uniform  sampler2D	texture_specular1;


void main()
{
	FragColor = vec4(fs_in.FragPos, 1.0);
//	//记住必须是在世界空间
//	gPosition = fs_in.FragPos;
//
//	gNormal = normalize(fs_in.Normal);
//
//	gAlbedoSpec.rgb = texture(texture_diffuse1, fs_in.TexCoords).rgb;
//
//	gAlbedoSpec.a = texture(texture_specular1, fs_in.TexCoords).r;
}
