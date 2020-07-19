#version 330 core
out vec4 FragColor;

//从GBuffer中获取数据
uniform sampler2D	gPosition;
uniform sampler2D	gNormalAo;
uniform sampler2D	gAlbedoSpec;
uniform sampler2D   gNormalmapRoughness;


in vec2 TexCoords;


struct Light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
//cameraPos
uniform vec3 viewPos;
//constant value
const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(gNormalmapRoughness, TexCoords).rgb * 2.0 - 1.0;
	//切线空间, 
    vec3 Q1  = dFdx(texture(gPosition, TexCoords).rgb);
    vec3 Q2  = dFdy(texture(gPosition, TexCoords).rgb);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(texture(gNormalAo, TexCoords).rgb);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}




// ----------------------------------------------------------------------------
void main()
{		
	//gamma校正
    vec3 albedo     = pow(texture(gAlbedoSpec, TexCoords).rgb, vec3(2.2));
	vec3 WorldPos	= texture(gPosition, TexCoords).rgb;
    float metallic  = texture(gAlbedoSpec, TexCoords).a;
    float roughness = texture(gNormalmapRoughness, TexCoords).a;
    float ao        = texture(gNormalAo, TexCoords).a;


    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - WorldPos);

	//非金属时候F0位0.04 经验值  
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    //初始化出射辐射率
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 32; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position- WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - WorldPos);
		//点灯光辐射强度（单位面积的光通量）随距离平方衰减
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 防止除零错误
        vec3 specular = nominator / denominator;
        
        vec3 kS = F;
		//渲染方程遵循能量守恒原则， 漫反射光和高光反射不能超过1. 金属不会折射光线，因此不会有漫反射，所以乘以(1.0 - metallic);
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
	//环境光默认为0.05
    vec3 ambient = vec3(0.05) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
//    color = pow(color, vec3(1.0/2.2)); 
//	FragColor = vec4(color, 1.0);

	FragColor = texture(gPosition, TexCoords);
}

