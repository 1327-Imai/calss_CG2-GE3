#pragma once
#include "WorldTransform.h"

class Camera{
public:
	//�R���X�g���N�^
	Camera();

	//�f�X�g���N�^
	~Camera();

	//�����o�֐�
public:
	//������
	void Initialize();

	//�X�V
	void Update();

private:
	//�r���[�s��̍쐬
	void CreateMatView();

	//�v���W�F�N�V�����s��̍쐬
	void CreateMatProjection();

	//�r���{�[�h�s��̍쐬
	void CreateMatBillboard();

	//�A�N�Z�b�T
public:
	Matrix4 GetMatView() {
		return matView_;
	}

	Matrix4 GetMatProjection() {
		return matProjection_;
	}

	Matrix4 GetMatBillBoard() {
		return matBillboard_;
	}

	Matrix4 GetMatBillBoardY() {
		return matBillboardY_;
	}

	void SetPosition(Vector3 position) {
		worldTransform_.translation_ = position;
	}
	Vector3 GetPosition() const {
		Vector3 pos = {
			worldTransform_.matWorld_.m[3][0] ,
			worldTransform_.matWorld_.m[3][1] ,
			worldTransform_.matWorld_.m[3][2]
		};
		return pos;
	}

	void SetRotation(Vector3 rotation) {
		worldTransform_.rotation_ = rotation;
	}
	Vector3 GetRotation() {
		return worldTransform_.rotation_;
	}

	void SetScale(Vector3 scale) {
		worldTransform_.scale_ = scale;
	}
	Vector3 GetScale() {
		return worldTransform_.scale_;
	}

	void SetTargetPos(Vector3 targetPos) {
		targetPos_ = &targetPos;
	}

	//�����o�ϐ�
private:
	WorldTransform worldTransform_;

	Vector3* targetPos_ = nullptr;

	Matrix4 matView_;
	Vector3 eye_;	//���_���W
	Vector3 target_;	//�����_���W
	Vector3 up_;		//������x�N�g��

	Matrix4 matProjection_;
	float fovY_;
	float nearZ_;
	float farZ_;

	Matrix4 matBillboard_;
	Matrix4 matBillboardY_;

};

