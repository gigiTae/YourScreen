Texture2D tex_original : register(t0);

SamplerState gSampler : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Screensize : POSITION;
};

#define POSTEFFECT_BLUR_SAMPLING_COUNT 9

// X방향 Gaussian Blur
float3 psBlur_X( float2 texCoord, float2 Screensize ) : COLOR0
{
    const float width = 1.0f/Screensize.x;
    const float height = 1.0f/Screensize.y;

    const static float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    const static float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = {-4, -3, -2, -1, 0, +1, +2, +3, +4};
    const static float2 dir = float2( 1.0, 0.0 );
    
    float2 step = float2( dir.x * width , dir.y * height );
    float3 Result = 0.0f;
    
    for( int i = 0 ; i < 9 ; i++ )
    {
        Result += tex_original.Sample(gSampler, texCoord + indices[i]*step) * weights[i];
    }
    
    return float3( Result.rgb );
}

// Y방향 Gaussian Blur
float3 psBlur_Y( float2 texCoord, float2 Screensize) : COLOR0
{
    const float width = 1.0f/Screensize.x;
    const float height = 1.0f/Screensize.y;

    const static float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    const static float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = {-4, -3, -2, -1, 0, +1, +2, +3, +4};
    const static float2 dir = float2( 0.0, 1.0 );
    
    float2 step = float2( dir.x * width , dir.y * height );
    float3 Result = 0.0f;
    
    for( int i = 0 ; i < 9 ; i++ )
    {
        Result += tex_original.Sample(gSampler, texCoord + indices[i]*step) * weights[i];
    }
    
    return float3( Result.rgb );
}


float4 PS(PS_INPUT pin) : SV_Target
{
    //float3 result = tex_original.Sample(gSampler, pin.TexCoord).xyz;
    float3 result;// = float3(0.0f, 0.0f, 0.0f);
    
    if(pin.Screensize.z == 0)
    {
        result = psBlur_X(pin.TexCoord, pin.Screensize.xy);
    }
    else
    {
        result = psBlur_Y(pin.TexCoord, pin.Screensize.xy);
    }
    return float4(result, 1.0f);
}