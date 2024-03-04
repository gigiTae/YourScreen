cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gWorld;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 UV : TEXCOORD;
    float3 TangentL : TANGENT;
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

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    //vout.PosW = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //vout.PosW = float4(0.0f, 0.0f, 0.0f, 0.0f);
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.NormalW = float3(0.0f, 0.0f, 0.0f);
    vout.UV = vin.UV;
    vout.PosPos = vout.PosH;
    vout.TangentW = mul(vin.TangentL, gWorld);
    
    return vout;
}