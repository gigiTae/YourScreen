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
    float4 PosPos : POSITION2;
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
    
    pin.NormalW = normalize(pin.NormalW);
    
    //pOut.Normal = float4(pin.NormalW.xyz, pin.PosH.z / pin.PosH.w);
    pOut.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    pOut.Normal = float4(WorldNormalToColor(pin.NormalW), 1.0f);
    pOut.PosW = float4(pin.PosW, 1.0f);
    pOut.Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.MR = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.MR.x = pin.PosPos.z / pin.PosPos.w;;
    return pOut;
}