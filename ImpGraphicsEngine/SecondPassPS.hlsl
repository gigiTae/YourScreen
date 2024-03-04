Texture2D tex_baseColor : register(t0);
Texture2D tex_normal : register(t1);
Texture2D tex_posW : register(t2);
Texture2D tex_color : register(t3);
Texture2D tex_MR : register(t4); // x가 깊이임

SamplerState gSampler : register(s0);

TextureCube texCube : register(t5);

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
    float objectEmissionCoefficient;
    
    // SSAO
    //float4x4 gViewToTexSpace;
    //float4 gOffsetVectors[14];
    //
    //float gOcclusionRadius = 0.5f;
    //float gOcclusionFadeStart = 0.2f;
    //float gOcclusionFadeEnd = 2.0f;
    //float gSurfaceEpsilon = 0.05f;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 ToFarPlane : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 original : SV_Target0;
    float4 bright   : SV_Target1;
};

SamplerState samNormalDepth
{
    Filter = MIN_MAG_SINEAR_MIP_POINT;

    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, le5f);
};

SamplerState samRandomVec
{
    Filter = MIN_MAG_SINEAR_MIP_POINT;

    AddressU = WRAP;
    AddressV = WRAP;
};

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
    const float roughness = tex_MR.Sample(gSampler, UV).y;
    //static const float metallic = 0.0;
    const float metallic = tex_MR.Sample(gSampler, UV).z;

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

float3 ColorToWorldNormal(float3 color)
{
    return color * 2 - 1.0f;
}

// Bloom
float GetBloomCurve( float x, float threshold )
{
    float result = x;
    x *= 2.0f;
    
    //#ifdef BLOOMCURVE_METHOD_1
    result = x*0.05+max(0,x-threshold)*0.5; // default threshold = 1.26
    //#endif
    
    //#ifdef BLOOMCURVE_METHOD_2
    //result = x*x/3.2;
    //#endif
    
    //#ifdef BLOOMCURVE_METHOD_3
    //result = max(0,x-threshold); // default threshold = 1.0
    //result *= result;
    //#endif 
    
    return result * 0.5f;
}

PS_OUTPUT PS(PS_INPUT pin)
{
    float3 positionW = tex_posW.Sample(gSampler, pin.TexCoord).xyz;
    float3 toEyeW = normalize(eyePos - positionW);
    float4 normalSample = tex_normal.Sample(gSampler, pin.TexCoord);
    float3 normal = normalSample.xyz;
    normal = ColorToWorldNormal(normal);
    float4 color = tex_color.Sample(gSampler, pin.TexCoord) * globalEmissionCoefficient;
    float4 baseColor = tex_baseColor.Sample(gSampler, pin.TexCoord);
    baseColor.xyz = pow(baseColor.xyz, 2.2);
    
    static const float elipse = 0.001f;
    float3 result = { 0.0f, 0.0f, 0.0f };
    if(normalSample.x < 100.0f)
    {
        // Directional Light 
        for(int i = 0; i < numOfDirectionalLight; i++)
        {
            result += LightSurface1(toEyeW, normal, directionalLight[i].color.rgb, -directionalLight[i].Direction, baseColor.xyz, directionalLight[i].intensity, pin.TexCoord, 0);
        }
    
        // PointLight 
        for(int i = 0; i < numOfPointLight; i++)
        {
            float3 lightVec = pointLight[i].position - positionW;
            float d = length(lightVec);
            lightVec /= d;
    
            if(d < pointLight[i].range)
            {
                float3 point1 = LightSurface1(toEyeW, normal, pointLight[i].color.rgb, lightVec, baseColor.xyz, pointLight[i].intensity, pin.TexCoord, 2);
                float att = 1.0f / dot(pointLight[i].attenuation, float3(1.0f, d, d * d));
                point1 *= att;
                result += point1;
            }
        }
    
        // SpotLight 
        for(int i = 0; i < numOfSpotLight; i++)
        {
            float3 lightVec = spotLight[i].position - positionW;
            float d = length(lightVec);
            lightVec /= d;
    
            if(d < spotLight[i].range)
            {
                float3 spot1 = LightSurface1(toEyeW, normal, spotLight[i].color.rgb, lightVec, baseColor.xyz, spotLight[i].intensity, pin.TexCoord, 1);
                float spot = pow(max(dot(-lightVec, spotLight[i].direction), 0.0f), spotLight[i].spot);
                float att = spot / dot(spotLight[i].attenuation, float3(1.0f, d, d * d));
                spot1 *= att;
                result += spot1;
            }
        }   
    }
    else
    {
        result = baseColor.xyz;
    }   
    
    // SSAO
    //{
    //    float3 n = tex_normal.Sample(samNormalDepth, pin.TexCoord).xyz;
    //    float pz = tex_MR.Sample(samNormalDepth, pin.TexCoord).x;
    //
    //    float3 p = (pz / pin.ToFarPlane.z) * pin.ToFarPlane;
    //
    //    float3 randVec = 2.0f * gRandomVecMap.Sample(samRandomVec, 4.0f * pin.TexCoord).rgb - 1.0f;
    //
    //    float occlusionSum = 0.0f;
    //
    //    [unroll]
    //    for(int i = 0; i < gSampleCount; i++)
    //    {
    //        float3 offset = reflect(gOffsetVectors[i].xyz, randVec);
    //
    //        float flip = sign(dot(offset, n));
    //
    //        float3 q = p + flip * gOcclusionRadius * offset;
    //
    //        float4 projQ = mul(float4(q, 1.0f), gViewToTexSpace); 
    //        projQ /= projQ.w;
    //
    //        float rz = tex_MR.Sample(samNormalDepth, projQ.xy).x;
    //
    //        float3 r = (rz / q.z) * q;
    //
    //        float distZ = p.z - r.z;
    //        float dp = max(dot(n, normalize(r - p)), 0.0f);
    //        float occlusion = dp * OcclusionFunction(distZ);
    //
    //        occlusionSum += occlusion;
    //    }
    //    occlusionSum /= gSampleCount;
    //
    //    float access = 1.0f - occlusionSum;
    //    // SSAO가 좀 더 극명한 효과를 내도록, 거듭제곱을 이용해서
    //    // SSAO 맵의 대비를 강화한다.
    //    return saturate(pow(access, 4.0f));
    //}

    // Bloom
    PS_OUTPUT pOut;
    result = pow(result, 1/2.2);
    pOut.original = float4(result, 1.0f) + color;
    //pOut.original = float4(baseColor.xyz, 1.0f) + color;
    
    float bloom_intensity = GetBloomCurve(0.7, 1.26);
    float3 bloom_color = (result + color.xyz) * bloom_intensity;

    pOut.bright = float4(bloom_color, 1.0f);

    return pOut;

    //return float4(result, 1.0f) + color;
}