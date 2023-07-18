#include "MathFunc.h"
#include <cassert>

//�X�P�[�����O�s���ݒ肷��֐�
void MathFunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matScale;

}

//��]�s���ݒ肷��֐�
void MathFunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	Quaternion rotaQ;

	//��]�s���錾
	//Z����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({0 , 0 , 1.0f} , rotation.z);
	Matrix4 matRotZ = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//X����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({1.0f , 0 , 0} , rotation.x);
	Matrix4 matRotX = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//Y����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({0 , 1.0f , 0} , rotation.y);
	Matrix4 matRotY = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//�s��̌v�Z
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//���s�ړ��s��̐ݒ������֐�
void MathFunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//���[���h�ϊ��s��𐶐�����֐�
//�X�P�[�����O�s��𐶐�����֐�
Matrix4 MathFunc::Affine::CreateMatScale(Vector3 scale) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	return matScale;

}

//��]�s��𐶐�����֐�
Matrix4 MathFunc::Affine::CreateMatRotation(Vector3 rotation) {

	//��]�s���錾
	Matrix4 matRot;
	matRot.SetIdentityMatrix();

	//Z����]
	Quaternion rotaQ;

	//��]�s���錾
	//Z����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({0 , 0 , 1.0f} , rotation.z);
	Matrix4 matRotZ = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//X����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({1.0f , 0 , 0} , rotation.x);
	Matrix4 matRotX = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//Y����]
	rotaQ = MathFunc::Utility::MakeAxisAngle({0 , 1.0f , 0} , rotation.y);
	Matrix4 matRotY = MathFunc::Utility::MakeRotateMatrix(rotaQ);

	//�s��̌v�Z
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}


//���s�ړ��s��̐���������֐�
Matrix4 MathFunc::Affine::CreateMatTranslation(Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	return matTranslation;
}

#pragma endregion

#pragma region//�x���@�̌ʓx�@�̊֐�

//�x���@���ʓx�@�ɕϊ�����֐�
float MathFunc::Utility::Deg2Rad(float Deg) {

	return Deg * MathFunc::PI / 180;

}

//�ʓx�@��x���@�ɕϊ�����֐�
float MathFunc::Utility::Rad2Deg(float rad) {

	return rad / MathFunc::PI * 180;

}

#pragma endregion

//�x�N�g���ƍs��̊|���Z������֐�
Vector3  MathFunc::Utility::MulVector3AndMatrix4(Vector3 vec , Matrix4 mat) {

	Vector3 ans;

	ans.x =
		mat.m[0][0] * vec.x +
		mat.m[1][0] * vec.y +
		mat.m[2][0] * vec.z +
		mat.m[3][0] * 0;

	ans.y =
		mat.m[0][1] * vec.x +
		mat.m[1][1] * vec.y +
		mat.m[2][1] * vec.z +
		mat.m[3][1] * 0;

	ans.z =
		mat.m[0][2] * vec.x +
		mat.m[1][2] * vec.y +
		mat.m[2][2] * vec.z +
		mat.m[3][2] * 0;

	return ans;

}

Matrix4 MathFunc::Utility::PerspectiveFovLH(float fovAngleY , float aspectRatio , float nearZ , float farZ) {

	float viewHeight = 1 / tan(fovAngleY * 0.5);
	float viewWidth = viewHeight / aspectRatio;
	float fRange = farZ / (farZ - nearZ);

	Matrix4 perspectiveFovLH = {
		viewWidth , 0 , 0 , 0 ,
		0 , viewHeight , 0 , 0 ,
		0 , 0 , fRange , 1 ,
		0 , 0 , -fRange * nearZ , 0
	};


	return perspectiveFovLH;
}

Quaternion MathFunc::Utility::MakeAxisAngle(const Vector3& axis , float angle) {
	Quaternion ans;
	Vector3 ansV = axis;

	ansV.normalize();

	ansV *= sinf(angle / 2);

	ans = {
		ansV.x ,
		ansV.y ,
		ansV.z ,
		cosf(angle / 2)
	};

	return ans;

}

Vector3 MathFunc::Utility::RotateVector(const Vector3& v , const Quaternion& q) {
	Quaternion ans = q;

	Quaternion pos = {v.x , v.y , v.z , 0};
	Quaternion rotaQ = q;

	ans = ans.Multiply(pos);
	ans = ans.Multiply(rotaQ.Conjugate());

	return Vector3(ans.x , ans.y , ans.z);

}

Matrix4 MathFunc::Utility::MakeRotateMatrix(const Quaternion& q) {

	Matrix4 ans;
	ans.SetIdentityMatrix();

	ans.m[0][0] = powf(q.w , 2) + powf(q.x , 2) - powf(q.y , 2) - powf(q.z , 2);
	ans.m[0][1] = 2 * (q.x * q.y + q.w * q.z);
	ans.m[0][2] = 2 * (q.x * q.z - q.w * q.y);
	ans.m[0][3] = 0;

	ans.m[1][0] = 2 * (q.x * q.y - q.w * q.z);
	ans.m[1][1] = powf(q.w , 2) - powf(q.x , 2) + powf(q.y , 2) - powf(q.z , 2);
	ans.m[1][2] = 2 * (q.y * q.z + q.w * q.x);
	ans.m[1][3] = 0;

	ans.m[2][0] = 2 * (q.x * q.z + q.w * q.y);
	ans.m[2][1] = 2 * (q.y * q.z - q.w * q.x);
	ans.m[2][2] = powf(q.w , 2) - powf(q.x , 2) - powf(q.y , 2) + powf(q.z , 2);
	ans.m[2][3] = 0;

	return ans;

}

double MathFunc::Ease::In(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * time * time);
}

double MathFunc::Ease::Out(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * (1 - (1 - time) * (1 - time)));
}

double MathFunc::Ease::InOut(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	if (time < 0.5)
	{
		return start + (move * (2 * time * time));
	}
	else
	{
		return start + move * (1 - (((-2 * time + 2) * (-2 * time + 2)) / 2));
	}
}

double MathFunc::Ease::In_Back(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (c3 * time * time * time - c1 * time * time));
}

double MathFunc::Ease::Out_Back(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (1 + c3 * ((time - 1) * (time - 1) * (time - 1)) + c1 * ((time - 1) * (time - 1))));
}

double MathFunc::Ease::InOut_Back(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c2 = c1 * 1.525;

	if (time < 0.5)
	{
		return start + move * ((((2 * time) * (2 * time)) * ((c2 + 1) * 2 * time - c2)) / 2);
	}
	else
	{
		return start + move * ((((2 * time - 2) * (2 * time - 2)) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2);
	}
}

double MathFunc::Ease::Out_Bounce(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 1 / d1) {
		return start + move * (n1 * time * time);
	}
	else if (time < 2 / d1) {
		return start + move * (n1 * (time -= 1.5 / d1) * time + 0.75);
	}
	else if (time < 2.5 / d1) {
		return start + move * (n1 * (time -= 2.25 / d1) * time + 0.9375);
	}
	else {
		return start + move * (n1 * (time -= 2.625 / d1) * time + 0.984375);
	}
}

double MathFunc::Ease::In_Bounce(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	time = 1 - time;
	if (time < 1 / d1) {
		return start + move * (1 - (n1 * time * time));
	}
	else if (time < 2 / d1) {
		return start + move * (1 - (n1 * (time -= 1.5 / d1) * time + 0.75));
	}
	else if (time < 2.5 / d1) {
		return start + move * (1 - (n1 * (time -= 2.25 / d1) * time + 0.9375));
	}
	else {
		return start + move * (1 - (n1 * (time -= 2.625 / d1) * time + 0.984375));
	}

}

double MathFunc::Ease::InOut_Bounce(double start , double end , double time , double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 0.5)
	{
		time = 1 - time * 2;
		if (time < 1 / d1) {
			return start + move * ((1 - (n1 * time * time)) / 2);
		}
		else if (time < 2 / d1) {
			return start + move * ((1 - (n1 * (time -= 1.5 / d1) * time + 0.75)) / 2);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((1 - (n1 * (time -= 2.25 / d1) * time + 0.9375)) / 2);
		}
		else {
			return start + move * ((1 - (n1 * (time -= 2.625 / d1) * time + 0.984375)) / 2);
		}
	}
	else
	{
		time = time * 2 - 1;
		if (time < 1 / d1) {
			return start + move * ((n1 * time * time) / 2 + 0.5);
		}
		else if (time < 2 / d1) {
			return start + move * ((n1 * (time -= 1.5 / d1) * time + 0.75) / 2 + 0.5);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((n1 * (time -= 2.25 / d1) * time + 0.9375) / 2 + 0.5);
		}
		else {
			return start + move * ((n1 * (time -= 2.625 / d1) * time + 0.984375) / 2 + 0.5);
		}
	}
}