cbuffer cbPerObject
{
    // x: width
    // y: height
    // z: ratio 1920 * 1080 에서 몇 배수로 나눈 텍스처인지
    // w: blur 방향 0: 아니면 y
    float4 screenSize;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Screensize : POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    input.Position.x = (input.Position.x - (screenSize.z - 1)) / screenSize.z;
    input.Position.y = (input.Position.y + (screenSize.z - 1)) / screenSize.z;

    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = input.TexCoord;

    //output.Position = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //output.TexCoord = float2(1.0f, 1.0f);
    
    output.Screensize = float3(screenSize.x, screenSize.y, screenSize.w);

    return output;
}