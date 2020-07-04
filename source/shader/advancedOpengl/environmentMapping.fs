#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

//uniform sampler2D texture1;


uniform samplerCube skyBox;


uniform vec3 viewPos;

////使用漫反射贴图
//void main()
//{
//	FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);	
//}

//反射
void main()
{
	vec3 inRay = normalize(FragPos - viewPos);

	vec3 reflectRay = reflect( inRay, normalize(Normal));

	FragColor = vec4(texture(skyBox, reflectRay).rgb, 1.0);
}
 
////折射 Single-side Refraction
//void main()
//{             
//    float ratio = 1.00 / 1.52;
//	vec3 inRay = normalize(FragPos - viewPos);
//
//	vec3 refractRay =  refract(inRay, normalize(Normal), ratio);
//
//	FragColor = vec4(texture(skyBox, refractRay).rgb, 1.0);
//}