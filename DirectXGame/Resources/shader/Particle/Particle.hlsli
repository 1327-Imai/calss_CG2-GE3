//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {

	//�F(RGBA)
	float4 color;

};

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1) {

	matrix mat;
	matrix matBillboard;

};

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput {

	//�V�X�e���p���_���W
	float4 pos : POSITION;
	//�X�P�[�����O
	float scale : SCALE;

};

//�W�I���g���V�F�[�_�[�̏o�͍\����
//(�W�I���g���V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct GSOutput {

	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv�l
	float2 uv : TEXCOORD;

};