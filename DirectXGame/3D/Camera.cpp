#include "Camera.h"
#include "WinApp.h"
#include <cassert>

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Initialize() {

	matView_.SetIdentityMatrix();
	matProjection_.SetIdentityMatrix();
	matBillboard_.SetIdentityMatrix();
	matBillboardY_.SetIdentityMatrix();

	worldTransform_.initialize();
	worldTransform_.translation_ = {0 , 0 , -10};

	fovY_ = 45.0f;
	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	Update();
}

void Camera::Update() {
	worldTransform_.UpdateMatWorld();

	eye_ = {
		worldTransform_.matWorld_.m[3][0] ,
		worldTransform_.matWorld_.m[3][1] ,
		worldTransform_.matWorld_.m[3][2]
	};


	up_ = {0 , 1.0f , 0};
	target_ = {0 , 0 , 1.0f};

	Quaternion rotation = MathFunc::Utility::MakeAxisAngle({0.0f , 0.0f , 1.0f} , worldTransform_.rotation_.z);
	up_ = MathFunc::Utility::RotateVector(up_ , rotation);
	target_ = MathFunc::Utility::RotateVector(target_ , rotation);

	rotation = MathFunc::Utility::MakeAxisAngle({1.0f , 0.0f , 0.0f} , worldTransform_.rotation_.x);
	up_ = MathFunc::Utility::RotateVector(up_ , rotation);
	target_ = MathFunc::Utility::RotateVector(target_ , rotation);

	rotation = MathFunc::Utility::MakeAxisAngle({0.0f , 1.0f , 0.0f} , worldTransform_.rotation_.y);
	up_ = MathFunc::Utility::RotateVector(up_ , rotation);
	target_ = MathFunc::Utility::RotateVector(target_ , rotation);

	target_ += eye_;

	CreateMatView();
	CreateMatProjection();
	CreateMatBillboard();

}

void Camera::CreateMatView(){

	Vector3 cameraAxisZ = target_ - eye_;

	assert(!cameraAxisZ.length() == 0);
	assert(!up_.length() == 0);

	cameraAxisZ.normalize();

	Vector3 cameraAxisX = up_.cross(cameraAxisZ);
	cameraAxisX.normalize();

	Vector3 cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	cameraAxisY.normalize();

	matView_ = {
		cameraAxisX.x , cameraAxisX.y , cameraAxisX.z , 0 ,
		cameraAxisY.x , cameraAxisY.y , cameraAxisY.z , 0 ,
		cameraAxisZ.x , cameraAxisZ.y , cameraAxisZ.z , 0 ,
		0 , 0 , 0 , 1
	};

	matView_ = matView_.CulInvers();

	Vector3 reverseEye = eye_ * -1;

	float tX = cameraAxisX.dot(reverseEye);
	float tY = cameraAxisY.dot(reverseEye);
	float tZ = cameraAxisZ.dot(reverseEye);


	matView_.m[3][0] = tX;
	matView_.m[3][1] = tY;
	matView_.m[3][2] = tZ;

}

void Camera::CreateMatProjection(){
	matProjection_ = MathFunc::Utility::PerspectiveFovLH(
		MathFunc::Utility::Deg2Rad(fovY_) ,
		(float)WinApp::WINDOW_WIDTH / WinApp::WINDOW_HEIGHT ,
		nearZ_ , farZ_
	);
}

void Camera::CreateMatBillboard(){

	Vector3 cameraAxisZ = target_ - eye_;

	assert(!cameraAxisZ.length() == 0);
	assert(!up_.length() == 0);

	cameraAxisZ.normalize();

	Vector3 cameraAxisX = up_.cross(cameraAxisZ);
	cameraAxisX.normalize();

	Vector3 cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	cameraAxisY.normalize();


	matBillboard_ = {
		cameraAxisX.x , cameraAxisX.y , cameraAxisX.z , 0 ,
		cameraAxisY.x , cameraAxisY.y , cameraAxisY.z , 0 ,
		cameraAxisZ.x , cameraAxisZ.y , cameraAxisZ.z , 0 ,
		0 , 0 , 0 , 1
	};

	Vector3 ybillCameraAxisX = cameraAxisX;

	Vector3 ybillCameraAxisY = up_;
	ybillCameraAxisY.normalize();

	Vector3 ybillCameraAxisZ = ybillCameraAxisX.cross(ybillCameraAxisY);

	matBillboardY_ = {
		ybillCameraAxisX.x , ybillCameraAxisX.y , ybillCameraAxisX.z , 0 ,
		ybillCameraAxisY.x , ybillCameraAxisY.y , ybillCameraAxisY.z , 0 ,
		ybillCameraAxisZ.x , ybillCameraAxisZ.y , ybillCameraAxisZ.z , 0 ,
		0 , 0 , 0 , 1
	};
}
