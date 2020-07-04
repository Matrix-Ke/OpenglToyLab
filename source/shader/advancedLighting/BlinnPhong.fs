#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform bool blinn;
uniform bool gamma;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;


void main()
{    
	vec3  baseColor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	//环境光
	vec3 ambient = light.ambient * baseColor;
	
	//漫反射光
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * baseColor; 

	//高光，镜面光
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	float spec = 0.0;
	if (blinn)
	{
	//半角和法线夹角
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	}
	else
	{
	//反射光线和视线的夹角
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}

	vec3 specular = light.specular * spec ; // assuming bright white light color
	vec3 color = ambient + diffuse + specular;
	if(gamma)
		color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);

}
