#include "Camera.h"
#include "WinApp.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Initialize() {

	worldTransform_.initialize();
	worldTransform_.translation_ = {5 , 5 , -10};

	fovY_ = 45.0f;
	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

}

void Camera::Update() {
	worldTransform_.UpdateMatWorld();

	eye_ = {
		worldTransform_.matWorld_.m[3][0] ,
		worldTransform_.matWorld_.m[3][1] ,
		worldTransform_.matWorld_.m[3][2]
	};

	up_ = {
		sinf(MathFunc::Utility::Deg2Rad(worldTransform_.rotation_.z)) ,
		cosf(MathFunc::Utility::Deg2Rad(worldTransform_.rotation_.z)) ,
		0
	};
	up_.nomalize();

	if (targetPos_) {
		target_ = *targetPos_;
	}
	else {
		target_ = {
			sinf(MathFunc::Utility::Deg2Rad(worldTransform_.rotation_.y)) ,
			sinf(MathFunc::Utility::Deg2Rad(worldTransform_.rotation_.x)) ,
			cosf(MathFunc::Utility::Deg2Rad(worldTransform_.rotation_.y))
		};
		target_.nomalize();
		target_ += eye_;
	}

	matView_ = MathFunc::Utility::CreatMatView(eye_ , target_ , up_);

	matProjection_ = MathFunc::Utility::PerspectiveFovLH(
		MathFunc::Utility::Deg2Rad(fovY_) ,
		(float)WinApp::WINDOW_WIDTH / WinApp::WINDOW_HEIGHT ,
		nearZ_ , farZ_
	);

}
