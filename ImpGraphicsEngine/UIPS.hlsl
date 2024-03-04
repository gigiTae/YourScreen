Texture2D tex_UI : register(t0);
Texture2D noise : register(t1);

SamplerState gSampler : register(s0);

cbuffer cbPerObject
{
    float engine_alpha;
    float fStrength;// = 0.1;
    float2 uvDir; //= float2( 1, 1 );

    int isNoise;
    int isPlus;
    float xRatio;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

// 블러 하는 함수
// 합성

float4 PS(PS_INPUT pin) : SV_Target
{
    float3 result;
    float  alpha ;
    if(isNoise)
    {
        float2 st = pin.TexCoord + uvDir;// * fTime0_X;
        float2 offset = noise.Sample(gSampler, st).xy * 2 - 1;
        float2 uv2;
        if(isPlus)
        {
            uv2.x = pin.TexCoord.x + offset.x * fStrength;
            uv2.y = pin.TexCoord.y;
        }
        else
        {
            uv2.x = pin.TexCoord.x - offset.x * fStrength;
            uv2.y = pin.TexCoord.y;
        }

        result = tex_UI.Sample(gSampler, uv2).xyz;
        alpha = tex_UI.Sample(gSampler, uv2).w;
    }
    else
    {
        pin.TexCoord.x = pin.TexCoord.x * xRatio;
        result = tex_UI.Sample(gSampler, pin.TexCoord).xyz;
        alpha = tex_UI.Sample(gSampler, pin.TexCoord).w;
    }
  
    return float4(result, alpha * engine_alpha);
}