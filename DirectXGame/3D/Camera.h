#pragma once
#include "WorldTransform.h"

class Camera{
public:
	//コンストラクタ
	Camera();

	//デストラクタ
	~Camera();

	//メンバ関数
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//アクセッサ
	Matrix4 GetMatView() {
		return matView_;
	}

	Matrix4 GetMatProjection() {
		return matProjection_;
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

	//メンバ変数
private:
	WorldTransform worldTransform_;

	Vector3* targetPos_ = nullptr;

	Matrix4 matView_;
	Vector3 eye_;	//視点座標
	Vector3 target_;	//注視点座標
	Vector3 up_;		//上方向ベクトル

	Matrix4 matProjection_;
	float fovY_;
	float nearZ_;
	float farZ_;

};

