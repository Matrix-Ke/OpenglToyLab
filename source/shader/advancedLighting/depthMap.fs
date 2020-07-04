# version 330 core

out vec4 FragColor;
in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;

//用于显示场景用
void main()
{
//	FragColor = vec4(texture(texture_diffuse1, fs_in.TexCoords).rgb,1.0);
}

////单独渲染出z深度
//void main()
//{
//	gl_FragDepth = gl_FragCoord.z;
//}
