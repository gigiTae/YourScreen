Texture2D baseColor : register(t0);
Texture2D MR : register(t1);
Texture2D emission : register(t2);
Texture2D normalMap : register(t3);

SamplerState gSampler : register(s0);

// 상수 버퍼 인덱스는 어떻게 정해지는 거지 진짜 이상하다
// 안 쓰면 4가 적용 안되고 쓰면 적용 된다
// 그렇다고 인덱스를 0으로 넣으면 다른 PS의 상수 버퍼에 간섭하는 것처럼 보인다...

cbuffer cbPerObject2
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
    float4 BaseColor : SV_Target0;
    float4 Normal : SV_Target1;
    float4 PosW : SV_Target2;
    float4 Color : SV_Target3;
    float4 MR : SV_Target4; // roughness, metallic
};

float3 WorldNormalToColor(float3 normal)
{
    return (normal + 1.0f) * 0.5f;
}

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

PixelOut PS(VertexOut pin)
{
    PixelOut pOut;
    
    pin.NormalW = normalize(pin.NormalW);
    
    //pOut.Normal = float4(pin.NormalW.xyz, pin.PosH.z / pin.PosH.w);
    pOut.BaseColor = baseColor.Sample(gSampler, pin.UV);
    pOut.BaseColor.a = 1.0f;

    float3 normalMapSample = normalMap.Sample(gSampler, pin.UV).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
    //pOut.Normal = float4(WorldNormalToColor(pin.NormalW), 1.0f);
    pOut.Normal = float4(WorldNormalToColor(bumpedNormalW), 1.0f);

    pOut.PosW = float4(pin.PosW, 1.0f);
    pOut.Color = emission.Sample(gSampler, pin.UV); // 일단 컬러를 더해주니까 더하는 연산인 emission을 넣어둠
    //pOut.Color.x *= objectEmissionCoefficient;
    //pOut.Color.y *= objectEmissionCoefficient;
    //pOut.Color.z *= objectEmissionCoefficient;
    pOut.MR = MR.Sample(gSampler, pin.UV);
    pOut.MR.x = pin.PosPos.z / pin.PosPos.w;
    return pOut;
}