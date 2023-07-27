#include "Quaternion.h"

Quaternion::Quaternion() {
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quaternion::Quaternion(float x , float y , float z , float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::~Quaternion() {
}

Quaternion Quaternion::Multiply(const Quaternion q) {
	Quaternion ans;
	ans.IdentityQuaternion();

	ans.w = w * q.w - x * q.x - y * q.y - z * q.z;
	ans.x = y * q.z - z * q.y + w * q.x + x * q.w;
	ans.y = z * q.x - x * q.z + w * q.y + y * q.w;
	ans.z = x * q.y - y * q.x + w * q.z + z * q.w;

	return ans;
}

void Quaternion::IdentityQuaternion() {
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quaternion Quaternion::Conjugate() {
	Quaternion ans;
	Vector3 thisV = {x , y , z};

	Vector3 ansV = -thisV;

	ans.x = ansV.x;
	ans.y = ansV.y;
	ans.z = ansV.z;
	ans.w = w;

	return ans;
}

float Quaternion::Norm() {
	return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::Normalize() {

	Quaternion ans = *this;

	ans /= this->Norm();

	return ans;
}

Quaternion Quaternion::Inverse() {

	Quaternion ans;

	ans = this->Conjugate();
	ans /= powf(this->Norm() , 2);

	return ans;

}

Quaternion Quaternion::operator+() const {
	return *this;
}

Quaternion Quaternion::operator-() const {
	return Quaternion(-x , -y , -z , -w);
}

Quaternion Quaternion::operator+=(const Quaternion& q) {
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

Quaternion Quaternion::operator-=(const Quaternion& q) {
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

Quaternion Quaternion::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Quaternion Quaternion::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

bool Quaternion::operator==(const Quaternion& q) {
	if (x == q.x &&
		y == q.y &&
		z == q.z &&
		w == q.w) {
		return true;
	}

	return false;
}

bool Quaternion::operator!=(const Quaternion& q) {
	if (*this == q) {
		return false;
	}

	return true;
}

const Quaternion& operator+(const Quaternion& q1 , const Quaternion& q2) {
	Quaternion temp(q1);
	return temp += q2;
}

const Quaternion& operator-(const Quaternion& q1 , const Quaternion& q2) {
	Quaternion temp(q1);
	return temp -= q2;
}

const Quaternion& operator*(const Quaternion& q , float s) {
	Quaternion temp(q);
	return temp *= s;
}

const Quaternion& operator*(float s , const Quaternion& q) {
	return q * s;
}

const Quaternion& operator/(const Quaternion& q , float s) {
	Quaternion temp(q);
	return temp /= s;
}