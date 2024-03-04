TextureCube texCube : register(t0);
SamplerState gSampler : register(s0);

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

struct PixelOut
{
    float4 BaseColor : SV_Target0;
    float4 Normal : SV_Target1;
    float4 PosW : SV_Target2;
    float4 Color : SV_Target3;
    float4 MR : SV_Target4;
};

float3 WorldNormalToColor(float3 normal)
{
    return (normal + 1.0f) * 0.5f;
}

PixelOut PS(VertexOut pin)
{
    PixelOut pOut;
    
    //pOut.BaseColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.BaseColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.Normal = float4(WorldNormalToColor(float3(0.0f, 0.0f, 0.0f)), 1.0f);
    //pOut.Color = texCube.Sample(gSampler, pin.PosL);
    pOut.Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.PosW = float4(1.0f, 1.0f, 1.0f, 1.0f);
    pOut.MR = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return pOut;
}