cbuffer cbPerObject
{
	float4x4 WVP;
};

struct COLOR_VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

COLOR_VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor : COLOR)
{
    COLOR_VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);
    output.Color = inColor;

    return output;
}

float4 PS(COLOR_VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
}