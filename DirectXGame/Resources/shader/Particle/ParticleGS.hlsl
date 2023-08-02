#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[4] = {
	float4(-0.5f , -0.5f , 0 , 0) ,
	float4(-0.5f , +0.5f , 0 , 0) ,
	float4(+0.5f , -0.5f , 0 , 0) ,
	float4(+0.5f , +0.5f , 0 , 0)
};

static const float2 uv_array[4] = {
	float2(0.0f , 1.0) ,
	float2(0.0f , 0.0) ,
	float2(1.0f , 1.0) ,
	float2(1.0f , 0.0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION ,
	inout TriangleStream< GSOutput > output
) {
	GSOutput element;
	for (uint i = 0; i < vnum; i++) {
		float4 offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard , offset);
		element.svpos = input[0].pos + offset;
		element.svpos = mul(mat , element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}