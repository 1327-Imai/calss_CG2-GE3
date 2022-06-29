#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION , float2 uv : TEXCODE) {

	VSOutput output; //ピクセルシェーダーに渡す値
	output.svpos = mul(mat , pos);
	output.uv = uv;
	return output;

}