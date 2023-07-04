#pragma once
#include "Model.h"
#include "DirectXCommon.h"

class ModelManager{
	//�G�C���A�X
private:
	using string = std::string;

	//�ÓI�����o�֐�
public:
	//�C���X�^���X�擾
	static ModelManager* GetInstance();

	//�C���X�^���X����
	static ModelManager* Create(DirectXCommon* dxCommon);

	//�����o�֐�
public:
	//�I��������
	void Finalize();

	//���f���ǂݍ���
	void LoadModel(const string& modelName);

	//���f���Ăяo��
	Model* CallModel(const string& modelName);

	//���f���폜
	void deleteModels(const string& modelName);

private:
	//������
	void Initialize(DirectXCommon* dxCommon);

	//�����o�ϐ�
private:
	//���f���z��
	std::vector<Model*> models;

protected:
	//�R���X�g���N�^
	ModelManager();

};

