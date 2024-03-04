struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = input.TexCoord;

    //output.Position = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //output.TexCoord = float2(1.0f, 1.0f);

    return output;
}