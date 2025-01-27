#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp , input.uv));
	return float4(texcolor.rgb , texcolor.a) * color;
}