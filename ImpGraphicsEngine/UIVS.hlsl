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

cbuffer cbPerObject
{
    // x: width
    // y: height
    // z: ratio 1920 * 1080 에서 몇 배수로 나눈 텍스처인지
    // w: blur 방향 0: 아니면 y
    float x;
    float y;
    float ratio;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    //input.Position.xy *= rotationMatrix;
    //input.Position.xy *= 2;
    ///회전은 잠깐 보류
    //float  rotationAngle = 1.5708 / 2; // 회전 각도
    ////
    //float2x2 rotationMatrix = float2x2(
    //cos(rotationAngle), -sin(rotationAngle),
    //sin(rotationAngle), cos(rotationAngle));
    ////
    //
    //input.Position.xy = mul(input.Position.xy, rotationMatrix);
    //if(rotationAngle <= 1.5708)
    //{
    //    input.Position.x /= ((ratio) * (rotationAngle / 1.5708));
    //    input.Position.y *= ((ratio) * (rotationAngle / 1.5708));
    //}
    //input.Position.xy *= 1.1;    
//* (rotationAngle / 1.5708)
    // 스크린 공간을 정규화
     //input.Position.x /= ratio;
     //input.Position.y *= ratio;

    input.Position.x += x;
    input.Position.y -= y;

    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = input.TexCoord;

    return output;
}