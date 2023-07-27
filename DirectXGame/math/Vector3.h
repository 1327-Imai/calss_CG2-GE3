#pragma once
class Vector3 {
public:
	//�R���X�g���N�^
	Vector3();	//��x�N�g���Ƃ���
	Vector3(float x,float y,float z);	//xyz�������w�肵�Ă̐���

	//�����o�֐�
	float length() const;
	Vector3& normalize();
	float dot(const Vector3& v) const;
	Vector3 cross(const Vector3& v) const;
	const Vector3 lerp(const Vector3& start , const Vector3& end , const float t);

	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector3 operator+=(const Vector3& v);
	Vector3 operator-=(const Vector3& v);
	Vector3 operator*=(float s);
	Vector3 operator/=(float s);

	//��r���Z�q
	bool operator == (const Vector3& v);
	bool operator != (const Vector3& v);

	//�����o�ϐ�
public:
	float x;	//x����
	float y;	//y����
	float z;	//z����

};
//3�����Z�q�I�[�o�[���[�h
//�����Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߂ɁA�ȉ��̂悤�ɏ������Ă���
const Vector3& operator+(const Vector3& v1 , const Vector3& v2);
const Vector3& operator-(const Vector3& v1 , const Vector3& v2);
const Vector3& operator*(const Vector3& v , float s);
const Vector3& operator*(float s , const Vector3& v);
const Vector3& operator/(const Vector3& v , float s);

