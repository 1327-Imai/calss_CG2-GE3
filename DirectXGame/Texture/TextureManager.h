#pragma once
#include "Texture.h"
#include "DirectXCommon.h"

class TextureManager{
	//�G�C���A�X
private:
	using string = std::string;

	//�ÓI�����o�֐�
public:
	//�C���X�^���X�擾
	static TextureManager* GetInstance();

	//�C���X�^���X����
	static TextureManager* Create(DirectXCommon* dxCommon);

	//�����o�֐�
public:
	//�I��������
	void Finalize();

	//�e�N�X�`���ǂݍ���
	void LoadTexture(const string& textureName);

	//�e�N�X�`���Ăяo��
	Texture* CallTexture(const string& textureName);

	//�e�N�X�`���폜
	void deleteTextures(const string& textureName);

private:
	//������
	void Initialize(DirectXCommon* dxCommon);

	//�����o�ϐ�
private:
	//�e�N�X�`���z��
	std::vector<Texture*> textures;

protected:
	//�R���X�g���N�^
	TextureManager();

};

