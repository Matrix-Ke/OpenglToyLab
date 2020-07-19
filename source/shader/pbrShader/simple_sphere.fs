#version 330 core
layout (location = 0) out vec4 FragColor;

uniform vec3 lightColor;

in VS_OUT
{
	vec3 WorldPos;
	vec3 Normal;
} fs_in;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;


void main()
{  
	vec3  baseColor = lightColor;
	//环境光
	vec3 ambient = light.ambient * baseColor;
	
	//漫反射光
	vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * baseColor; 

	vec3 color = ambient + diffuse;

	FragColor = vec4(lightColor, 1.0);
}
