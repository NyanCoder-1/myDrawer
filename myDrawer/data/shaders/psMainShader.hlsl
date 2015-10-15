struct PixelInputType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 PS(PixelInputType input) : SV_TARGET
{
    return input.color;
}