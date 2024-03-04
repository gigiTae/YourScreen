struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float4 PosPos : POSITION;
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
    
    pOut.BaseColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.Color = float4(pin.Color.xyz, pin.PosH.z / pin.PosH.w);
    pOut.Normal = float4(WorldNormalToColor(float3(0.0f, 0.0f, 0.0f)), 1.0f);
    pOut.PosW = float4(1.0f, 1.0f, 1.0f, 1.0f);
    pOut.MR = float4(0.0f, 0.0f, 0.0f, 1.0f);
    pOut.MR.x = pOut.MR.x = pin.PosPos.z / pin.PosPos.w;
    
    return pOut;
}