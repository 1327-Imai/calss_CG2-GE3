#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION ,float scale : SCALE) {

	VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
	output.pos = pos;
	output.scale = scale;
	
	return output;

}