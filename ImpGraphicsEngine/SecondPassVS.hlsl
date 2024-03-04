struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 ToFarPlaneIndex : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 ToFarPlane : TEXCOORD1;
};

cbuffer cbPerFrame
{
    float4 gFrustumCorners[4];
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    //input.Position.x = (input.Position.x - 1.0 ) / 2;
    //input.Position.y = (input.Position.y + 1 ) / 2;

    output.Position = float4(input.Position, 1.0f);
    output.Position.z = 0.0f;
    output.TexCoord = input.TexCoord;
    output.ToFarPlane = gFrustumCorners[input.ToFarPlaneIndex.x].xyz;

    //output.Position = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //output.TexCoord = float2(1.0f, 1.0f);

    return output;
}