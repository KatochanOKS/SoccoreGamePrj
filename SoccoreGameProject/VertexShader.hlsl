struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

// ���_�V�F�[�_
VS_OUTPUT main(float3 pos : POSITION, float2 uv : TEXCOORD)
{
    VS_OUTPUT output;
    output.Pos = float4(pos, 1.0f);
    output.Tex = uv;
    return output;
}