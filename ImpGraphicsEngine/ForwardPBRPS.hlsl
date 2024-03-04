Texture2D baseColor : register(t0);
Texture2D MR : register(t1);
Texture2D emission : register(t2);
Texture2D normalMap : register(t3);

Texture2D depth : register(t4);

SamplerState gSampler : register(s0);

struct DirectionalLight
{
    float4 color;
    float3 Direction;
    float intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 attenuation;
    float range;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 attenuation;
    float range;
    float3 direction;
    float spot;
};

cbuffer cbPerObject : register(b0)
{
    float4 one;
    DirectionalLight directionalLight[3];
    PointLight pointLight[30];
    SpotLight spotLight[10];
    float3 eyePos;
    float globalEmissionCoefficient;
    unsigned int numOfDirectionalLight;
    unsigned int numOfSpotLight;
    unsigned int numOfPointLight;
    float pad2;
};

cbuffer cbPerObject2 : register(b1)
{
    float customAlpha;
    float3 test;
};

cbuffer cbPerObject3 : register(b2)
{
    float objectEmissionCoefficient;
    float3 pad;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 UV : TEXCOORD;
    float4 PosPos : POSITION2;
    float3 TangentW : TANGENT;
};

struct PixelOut
{
    float4 accum : SV_Target0;
    float4 reveal : SV_Target1;
};

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

// PBR
static const float PI = 3.14159265f;
static const float EPSILON = 1e-6f;

float3 Fresnel_Shlick(in float3 f0, in float3 f90, in float x)
{
    return f0 + (f90 - f0) * pow(1.f - x, 5.f);
}

float Diffuse_Burley(in float NdotL, in float NdotV, in float LdotH, in float roughness)
{
    float fd90 = 0.5f + 2.f * roughness * LdotH * LdotH;
    return Fresnel_Shlick(1, fd90, NdotL).x * Fresnel_Shlick(1, fd90, NdotV).x;
}

float Specular_D_GGX(in float alpha, in float NdotH)
{
    const float alpha2 = alpha * alpha;
    const float lower = (NdotH * NdotH * (alpha2 - 1)) + 1;
    return alpha2 / max(EPSILON, PI * lower * lower);
}

float G_Shlick_Smith_Hable(float alpha, float LdotH)
{
    return rcp(lerp(LdotH * LdotH, 1, alpha * alpha * 0.25f));
}

float3 Specular_BRDF(in float alpha, in float3 specularColor, in float NdotV, in float NdotL, in float LdotH, in float NdotH)
{
    // Specular D (microfacet normal distribution) component
    float specular_D = Specular_D_GGX(alpha, NdotH);

    // Specular Fresnel
    float3 specular_F = Fresnel_Shlick(specularColor, 1, LdotH);

    // Specular G (visibility) component
    float specular_G = G_Shlick_Smith_Hable(alpha, LdotH);

    return specular_D * specular_F * specular_G;
}

// L lightDirection
float3 LightSurface1(
    in float3 V, in float3 N,
    in float3 lightColor, in float3 L,
    in float3 albedo, in float intensity,
    in float2 UV,
    unsigned int lightType) // 0 directional, 1 spot, 2 point
{
    // Specular coefficiant - fixed reflectance value for non-metals
    static const float kSpecularCoefficient = 0.04;
    
    //static const float roughness = 0.7;
    const float roughness = MR.Sample(gSampler, UV).y;
    //static const float metallic = 0.0;
    const float metallic = MR.Sample(gSampler, UV).z;

    const float NdotV = saturate(dot(N, V));

    // Burley roughness bias
    const float alpha = roughness * roughness;

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic);
    //const float3 c_diff = 0;
    const float3 c_spec = lerp(kSpecularCoefficient, albedo, metallic);

    // Output color
    float3 acc_color = 0;

    // Accumulate light values
    {
        // light vector (to light)
        //const float3 L

        // Half vector
        const float3 H = normalize(L + V);

        // products
        const float NdotL = saturate(dot(N, L));
        const float LdotH = saturate(dot(L, H));
        const float NdotH = saturate(dot(N, H));

        // Diffuse & specular factors
        float diffuse_factor = Diffuse_Burley(NdotL, NdotV, LdotH, roughness);
        float3 specular = Specular_BRDF(alpha, c_spec, NdotV, NdotL, LdotH, NdotH);
        
        // Directional light
        acc_color += NdotL * lightColor * (((c_diff * diffuse_factor) + specular)) * intensity;
    }

    return acc_color;
}

PixelOut PS(VertexOut pin)
{
    PixelOut ps;
    
    float2 textureUV;
    textureUV.x = pin.PosH.x / 1920;
    textureUV.y = pin.PosH.y / 1080;
    float3 preDepth = depth.Sample(gSampler, textureUV);
    // x값만 뽑아온게 아니라 그런 줄 알았는데 확인 더해야됨
    float curDepth = pin.PosPos.z / pin.PosPos.w;
    
    bool is_front = (preDepth.x == 0.0f) || (preDepth.x >= curDepth);

    float4 ps_baseColor = baseColor.Sample(gSampler, pin.UV); 
    ps_baseColor.xyz = pow(ps_baseColor.xyz, 2.2);
    if(is_front)
    {
        float3 ps_positionW = pin.PosW;
        float3 ps_toEyeW = normalize(eyePos - ps_positionW);
        float3 ps_color = emission.Sample(gSampler, pin.UV) * globalEmissionCoefficient * objectEmissionCoefficient;

        float3 normalMapSample = normalMap.Sample(gSampler, pin.UV).rgb;
        float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
        //pOut.Normal = float4(WorldNormalToColor(pin.NormalW), 1.0f);
        float3 ps_normal = float4(bumpedNormalW, 1.0f);
        ps_normal = normalize(ps_normal);        

        /// Lighting
        float3 result = { 0.0f, 0.0f, 0.0f };
        // Directional Light 1개 적용
        for(int i = 0; i < numOfDirectionalLight; i++)
        {
            result += LightSurface1(ps_toEyeW, ps_normal, directionalLight[i].color.rgb, -directionalLight[i].Direction, ps_baseColor.xyz, directionalLight[i].intensity, pin.UV, 0);
        }
    
        // PointLight 1개 적용
        for(int i = 0; i < numOfPointLight; i++)
        {
            float3 lightVec = pointLight[i].position - ps_positionW;
            float d = length(lightVec);
            lightVec /= d;
        
            if(d < pointLight[i].range)
            {
                float3 point1 = LightSurface1(ps_toEyeW, ps_normal, pointLight[i].color.rgb, lightVec, ps_baseColor.xyz, pointLight[i].intensity, pin.UV, 2);
                float att = 1.0f / dot(pointLight[i].attenuation, float3(1.0f, d, d * d));
                point1 *= att;
                result += point1;
            }
        }
        
        // SpotLight 1개 적용
        for(int i = 0; i < numOfSpotLight; i++)
        {
            float3 lightVec = spotLight[i].position - ps_positionW;
            float d = length(lightVec);
            lightVec /= d;
        
            if(d < spotLight[i].range)
            {
                float3 spot1 = LightSurface1(ps_toEyeW, ps_normal, spotLight[i].color.rgb, lightVec, ps_baseColor.xyz, spotLight[i].intensity, pin.UV, 1);
                float spot = pow(max(dot(-lightVec, spotLight[i].direction), 0.0f), spotLight[i].spot);
                float att = spot / dot(spotLight[i].attenuation, float3(1.0f, d, d * d));
                spot1 *= att;
                result += spot1;
            }
        }    
        //return float4(result * ps_baseColor.a + ps_color, ps_baseColor.a);
        //float4 outColor = float4(result * ps_baseColor.a + ps_color, ps_baseColor.a);
        float4 outColor = float4(result * ps_baseColor.a * customAlpha + ps_color, ps_baseColor.a * customAlpha);
        //float4 outColor = float4(result * 0.5f, 0.5f);
        
        /// Test Alpha
        //float4 outColor = float4(result * 1.0f + ps_color, 1.0f);
        
        //float weight = max(min(1.0, max(max(outColor.r, outColor.g), outColor.b) * outColor.a), outColor.a) * clamp(0.03 / (1e-5 + pow(curDepth / 200, 4.0)), 1e-2, 3e3);
        float weight = clamp(pow(min(1.0, outColor.a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - curDepth * 0.9, 3.0), 1e-2, 3e3);
        ps.accum = float4(outColor.rgb * outColor.a, outColor.a) * weight;
        ps.reveal.x = outColor.a;
        
        return ps;
        //return float4(result * ps_baseColor.a + ps_color, 1.0f);
    }    
    
    ps.accum = float4(0.0f, 0.0f, 0.0f, 0.0f);
    ps.reveal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return ps;
    //return float4(0.0f, 0.0f, 0.0f, 0.0f);
    //pOut.MR.x = pin.PosPos.z / pin.PosPos.w;
    //return pOut;
}