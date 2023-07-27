#pragma once
#include "Vector3.h"
#include <math.h>

class Quaternion{
public:
	//コンストラクタ
	Quaternion();
	Quaternion(float x , float y , float z , float w);

	//デストラクタ
	~Quaternion();

	//メンバ関数
	//Quaternionの積
	Quaternion Multiply(const Quaternion q);

	//単位Quaternionいれる
	void IdentityQuaternion();

	//共役Quaternionを返す
	Quaternion Conjugate();

	//Quaternionのnormを返す
	float Norm();

	//正規化したQuaternionを返す
	Quaternion Normalize();

	//逆Quaternionを返す
	Quaternion Inverse();

	//単項演算子オーバーロード
	Quaternion operator+() const;
	Quaternion operator-() const;

	//代入演算子オーバーロード
	Quaternion operator+=(const Quaternion& q);
	Quaternion operator-=(const Quaternion& q);
	Quaternion operator*=(float s);
	Quaternion operator/=(float s);

	//比較演算子
	bool operator == (const Quaternion& q);
	bool operator != (const Quaternion& q);


public:
	float x , y , z , w;

};

//3項演算子オーバーロード
//いろんな引数(引数の型と順序)のパターンに対応するために、以下のように準備している
const Quaternion& operator+(const Quaternion& q1 , const Quaternion& q2);
const Quaternion& operator-(const Quaternion& q1 , const Quaternion& q2);
const Quaternion& operator*(const Quaternion& q , float s);
const Quaternion& operator*(float s , const Quaternion& q);
const Quaternion& operator/(const Quaternion& v , float s);