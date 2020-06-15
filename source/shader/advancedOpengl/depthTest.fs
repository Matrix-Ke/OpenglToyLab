#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct S_Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct S_Light
{
	vec3 position;
    vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform bool blinn;
uniform vec3 viewPos;
uniform S_Material material;
uniform S_Light  light;

void main()
{
	vec3 norm = normalize(Normal);

	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb ;
	
	//diffuse
	vec3 lightDir = normalize(light.position - FragPos);
	float diff  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb ;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = 0.0;
	if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
    }
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;


    FragColor = vec4(ambient + diffuse + specular, 1.0);
}