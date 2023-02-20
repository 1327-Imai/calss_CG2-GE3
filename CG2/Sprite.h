#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"

class Sprite {
public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�����o�֐�
	void Initialize(SpriteCommon* spriteCommon,uint32_t textureIndex = UINT32_MAX);

	void Update();

	void Draw();

	void SetPosition(const Vector2 position);
	Vector2 GetPosition() const;

	void SetRotation(float rotation);
	float GetRotation() const;

	void SetSize(const Vector2 size);
	Vector2 GetSize() const;

	void SetScale(const Vector2 scale);
	Vector2 GetScale() const;

	void SetAnchorPoint(const Vector2 anchorPoint);
	Vector2 GetAnchorPoint() const;

	void SetColor(Vector4 color);
	Vector4 GetColor() const;

	void SetIsFlipX(bool isFlipX);
	bool GetIsFlipX() const;

	void SetIsFlipY(bool isFlipY);
	bool GetIsFlipY() const;

	void SetIsVisible(bool isVisible);
	bool GetIsVisible() const;

	void SetTextureIndex(uint32_t textureIndex);
	uint32_t GetTextureIndex();

	void setTextureSize(Vector2 textureSize);
	Vector2 GetTextureSize();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();

	void AdujustTextureSize();

	//�\����
private:
	struct ConstBufferDataMaterial {
		Vector4 color;
	};

	struct ConstBufferDataTransform {
		Matrix4 mat;
	};

	struct Vertex {
		Vector3 pos;	//xyz���W
		Vector2 uv;		//uv���W
	};

	//�񋓌^
private:
	enum VertexNumber {
		LB,	//����:0
		LT,	//����:1
		RB,	//�E��:2
		RT,	//�E��:3
	};

	//�����o�ϐ�
private:
	//DirectX��b
	DX12base* dx12base_ = nullptr;

	//�X�v���C�g���ʏ���
	SpriteCommon* spriteCommon_ = nullptr;;

	Vertex* vertMap_ = nullptr;

	//�}�e���A���萔�o�b�t�@
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//�}�e���A���萔�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;

	//
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	//
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//�e�N�X�`���C���f�b�N�X
	uint32_t textureIndex_;

	//���[���h�ϊ�
	WorldTransform worldTransform_;

	//2D���W
	Vector2 position_;

	//��]
	float rotation_;

	//�\���T�C�Y
	Vector2 size_;

	//�X�P�[��
	Vector2 scale_;

	//�A���J�[
	Vector2 anchorPoint_;

	//�F
	Vector4 color_;

	//X���t���b�v
	bool isFlipX_ = false;

	//Y���t���b�v
	bool isFlipY_ = false;

	//�\���t���O
	bool isVisible_ = true;

	//�e�N�X�`��������W
	Vector2 textureleftTop_ = {0 , 0};

	//�e�N�X�`���؂�o���T�C�Y
	Vector2 textureSize_;

	//���_�f�[�^
	Vertex vertices_[4];
};

