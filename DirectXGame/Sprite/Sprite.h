#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"
#include "Texture.h"

class Sprite {
public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�����o�֐�
	void Initialize(SpriteCommon* spriteCommon, Texture* texture);

	void Update();

	void Draw();

	void SetTexture(Texture* texture);

	//�A�N�Z�b�T
	void SetPosition(const Vector2 position) {
		position_ = position;
	}
	Vector2 GetPosition() const {
		return position_;
	}

	void SetRotation(float rotation) {
		rotation_ = rotation;
	}
	float GetRotation() const {
		return rotation_;
	}

	void SetScale(const Vector2 scale) {
		scale_ = scale;
	}
	Vector2 GetScale() const {
		return scale_;
	}

	void SetSize(const Vector2 size) {
		size_ = size;
	}
	Vector2 GetSize() const {
		return size_;
	}

	void SetAnchorPoint(const Vector2 anchorPoint) {
		anchorPoint_ = anchorPoint;
	}
	Vector2 GetAnchorPoint() const {
		return anchorPoint_;
	}

	void SetColor(Vector4 color) {
		color_ = color;
	}
	Vector4 GetColor() const {
		return color_;
	}

	void SetIsFlipX(bool isFlipX) {
		isFlipX_ = isFlipX;
	}
	bool GetIsFlipX() const {
		return isFlipX_;
	}

	void SetIsFlipY(bool isFlipY) {
		isFlipY_ = isFlipY;
	}
	bool GetIsFlipY() const {
		return isFlipY_;
	}

	void SetIsVisible(bool isVisible) {
		isVisible_ = isVisible;
	}
	bool GetIsVisible() const {
		return isVisible_;
	}

	void SetTextureSize(Vector2 textureSize) {
		textureSize_ = textureSize;
	}
	Vector2 GetTextureSize() {
		return textureSize_;
	}

	void AdujustTextureSize();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();


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
	DirectXCommon* dxCommon_ = nullptr;

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

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRV�q�[�v�̐擪�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

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

	//�e�N�X�`��
	Texture* texture_ = nullptr;

};

