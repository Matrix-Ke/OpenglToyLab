#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;



uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

void main()
{
	vs_out.FragPos = aPos;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal; 
	vs_out.TexCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}