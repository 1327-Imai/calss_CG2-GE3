#pragma once
#include "Vector3.h"
#include <math.h>

class Quaternion{
public:
	//�R���X�g���N�^
	Quaternion();
	Quaternion(float x , float y , float z , float w);

	//�f�X�g���N�^
	~Quaternion();

	//�����o�֐�
	//Quaternion�̐�
	Quaternion Multiply(const Quaternion q);

	//�P��Quaternion�����
	void IdentityQuaternion();

	//����Quaternion��Ԃ�
	Quaternion Conjugate();

	//Quaternion��norm��Ԃ�
	float Norm();

	//���K������Quaternion��Ԃ�
	Quaternion Normalize();

	//�tQuaternion��Ԃ�
	Quaternion Inverse();

	//�P�����Z�q�I�[�o�[���[�h
	Quaternion operator+() const;
	Quaternion operator-() const;

	//������Z�q�I�[�o�[���[�h
	Quaternion operator+=(const Quaternion& q);
	Quaternion operator-=(const Quaternion& q);
	Quaternion operator*=(float s);
	Quaternion operator/=(float s);

	//��r���Z�q
	bool operator == (const Quaternion& q);
	bool operator != (const Quaternion& q);


public:
	float x , y , z , w;

};

//3�����Z�q�I�[�o�[���[�h
//�����Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߂ɁA�ȉ��̂悤�ɏ������Ă���
const Quaternion& operator+(const Quaternion& q1 , const Quaternion& q2);
const Quaternion& operator-(const Quaternion& q1 , const Quaternion& q2);
const Quaternion& operator*(const Quaternion& q , float s);
const Quaternion& operator*(float s , const Quaternion& q);
const Quaternion& operator/(const Quaternion& v , float s);