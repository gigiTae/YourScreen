cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gWorld;
    
    float4x4 gBoneTransforms[100];
    float4x4 gPreBoneTransforms[100];
    
    int changingCount;
    int maxChanging;
    int isOnInterpolation;
    float pad;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 UV : TEXCOORD;
    float3 TangentL : TANGENT;

    float3 Weight : WEIGHT;
    uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 UV : TEXCOORD;
    float4 PosPos : POSITION2;
    float3 TangentW : TANGENT;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
    // Skinning
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    weights[0] = vin.Weight.x;
    weights[1] = vin.Weight.y;
    weights[2] = vin.Weight.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        if(changingCount <= 0 || isOnInterpolation == 0)
        {
            if(vin.BoneIndices[i] == -1)
            {
                break;
            }
            posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
            normalL += weights[i] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndices[i]]);
        }
        else
        {
            if(vin.BoneIndices[i] == -1)
            {
                break;
            }
               
            float ratio = (float)changingCount / maxChanging;

            posL += (weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz) * ratio;
            normalL += weights[i] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndices[i]]) * ratio;

            posL += (weights[i] * mul(float4(vin.PosL, 1.0f), gPreBoneTransforms[vin.BoneIndices[i]]).xyz) * (1 - ratio);
            normalL += weights[i] * mul(vin.NormalL, (float3x3) gPreBoneTransforms[vin.BoneIndices[i]]) * (1 - ratio);
        }
    }
    
    vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
    //vout.PosW = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //vout.PosW = float4(0.0f, 0.0f, 0.0f, 0.0f);
    vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
    vout.NormalW = mul(normalL, (float3x3) gWorldInvTranspose);
    vout.UV = vin.UV;
    vout.PosPos = vout.PosH;
    vout.TangentW = mul(vin.TangentL, gWorld);
    
    return vout;
}