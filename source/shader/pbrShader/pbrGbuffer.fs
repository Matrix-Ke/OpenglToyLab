#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec4 gNormalAo;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 gNormalmapRoughness;


in VS_OUT
{
	vec3 WorldPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in ;



uniform  sampler2D	texture_albedo;
uniform  sampler2D	texture_metallic;
uniform  sampler2D  texture_normalMap;
uniform  sampler2D	texture_roughness;
uniform  sampler2D	texture_Ao;



void main()
{

	//记住必须是在世界空间
	gPosition = fs_in.WorldPos;

	//物体法线和AO图
	gNormalAo.rgb = normalize(fs_in.Normal);
	gNormalAo.a = texture(texture_Ao, fs_in.TexCoords).r;

	//漫反射贴图和高光贴图(金属贴图）
	gAlbedoSpec.rgb = texture(texture_albedo, fs_in.TexCoords).rgb;
	gAlbedoSpec.a = texture(texture_metallic, fs_in.TexCoords).r;

	//法线贴图和粗糙度贴图
	gNormalmapRoughness.rgb = texture(texture_normalMap, fs_in.TexCoords).rgb;
	gNormalmapRoughness.a = texture(texture_roughness, fs_in.TexCoords).r;


}
