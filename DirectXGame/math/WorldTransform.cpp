#include "WorldTransform.h"

void WorldTransform::initialize() {

	scale_ = {1 , 1 , 1};
	rotation_ = {0 , 0 , 0};
	translation_ = {0 , 0 , 0};
	matWorld_.SetIdentityMatrix();

	UpdateMatWorld();

}

//ワールド変換行列の更新
void WorldTransform::UpdateMatWorld() {

	//ワールド変換行列を用意
	Matrix4 affineMat;
	affineMat.SetIdentityMatrix();

	//ワールド変換行列にスケーリング,回転,平行移動の行列を合成
	MathFunc::Affine::SetMatScale(affineMat , scale_);
	MathFunc::Affine::SetMatRotation(affineMat , rotation_);
	MathFunc::Affine::SetMatTranslation(affineMat , translation_);

	//ワールド行列に単位行列を代入
	matWorld_.SetIdentityMatrix();

	//行列の計算
	matWorld_ *= affineMat;

	//もし親がある場合
	if (parent_) {
		//親のワールド行列との計算を行う
		matWorld_ *= parent_->matWorld_;
	}

}