Texture2D baseColor : register(t0);

SamplerState gSampler : register(s0);

cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gWorld;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 UV : TEXCOORD;
};

float4 PS(VertexOut pin) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);
    
    //pOut.Normal = float4(pin.NormalW.xyz, pin.PosH.z / pin.PosH.w);
    float4 BaseColor = float4(1.0f, 0.5f, 1.0f, 0.5f);
    return BaseColor;
}