#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform bool blinn;
uniform vec3 viewPos;

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
};

uniform S_Material material;
uniform S_Light  light;

void main()
{
	vec3 norm = normalize(Normal);

	//ambient
	vec3 ambient = texture(material.diffuse, TexCoords).rgb;
	
	//diffuse
	vec3 lightDir = normalize(light.position - FragPos);

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
	vec3 specular = vec3(0.3) * spec; // assuming bright white light color


    FragColor = vec4(ambient + specular, 1.0);
}