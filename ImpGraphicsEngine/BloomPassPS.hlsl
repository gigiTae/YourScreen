Texture2D tex_original : register(t0);
Texture2D tex_bright : register(t1);
Texture2D tex_960x540 : register(t2);
Texture2D tex_480x270 : register(t3);
Texture2D tex_240x135 : register(t4);

SamplerState gSampler : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

// 블러 하는 함수
// 합성

float4 PS(PS_INPUT pin) : SV_Target
{
   //float3 result = tex_original.Sample(gSampler, pin.TexCoord).xyz ;

//       float2 st = pin.TexCoord + uvDir;// * fTime0_X;
//       float2 offset = noise.Sample(gSamplerWrap, st).xy * 2 - 1;
//       float2 uv2 = pin.TexCoord + offset.xy * fStrength;
//
//    float3 result = 
//tex_240x135.Sample(gSampler, uv2).xyz + 
//tex_480x270.Sample(gSampler, uv2).xyz +
//tex_960x540.Sample(gSampler, uv2).xyz +
//tex_original.Sample(gSampler, uv2).xyz;

    float3 result = 
tex_240x135.Sample(gSampler, pin.TexCoord).xyz + 
tex_480x270.Sample(gSampler, pin.TexCoord).xyz +
tex_960x540.Sample(gSampler, pin.TexCoord).xyz +
tex_original.Sample(gSampler, pin.TexCoord).xyz;

    return float4(result, 1.0f);
}