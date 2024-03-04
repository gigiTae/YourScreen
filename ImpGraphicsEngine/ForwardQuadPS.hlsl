Texture2D forward_color : register(t0);
Texture2D forward_alpha : register(t1);

SamplerState gSampler : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

//struct PS_OUTPUT
//{
//    float4 original : SV_Target0;
//    float4 bright   : SV_Target1;
//};

const static float EPSILON = 0.00001f;

bool isApproximatelyEqual(float a, float b)
{
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

float max3(float3 v)
{
    return max(max(v.x, v.y), v.z);
}

float4 PS(PS_INPUT pin) : SV_Target
{
    //float3 positionW = tex_posW.Sample(gSampler, pin.TexCoord).xyz;
    float reveal = forward_alpha.Sample(gSampler, pin.TexCoord).r;

    //if (isApproximatelyEqual(reveal, 1.0f)) discard;
    bool isis = isApproximatelyEqual(reveal, 1.0f);

    float4 accum = forward_color.Sample(gSampler, pin.TexCoord);

    //if (isinf(max3(abs(accum.rgb)))) 
        //accum.rgb = float3(accum.aaa);

    float3 average_color = accum.rgb / max(accum.a, EPSILON);

    // blend func: GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA
    float4 result = float4(average_color.rgb, 1 - reveal);
    result.xyz = pow(result.xyz, 1 / 2.2);
    
    return result;
}