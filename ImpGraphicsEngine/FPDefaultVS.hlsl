cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gWorld;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float4 PosPos : POSITION;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Color = vin.Color;
    vout.PosPos = vout.PosH;
    
    return vout;
}