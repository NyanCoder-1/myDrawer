cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VertexInputType
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

    output.pos = mul(input.pos, WVP);
    output.color = input.color;

    return output;
}