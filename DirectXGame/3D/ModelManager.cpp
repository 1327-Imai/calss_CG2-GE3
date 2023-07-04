#include "ModelManager.h"

ModelManager* ModelManager::GetInstance(){
	static ModelManager instance;
	return &instance;
}

ModelManager* ModelManager::Create(DirectXCommon* dxCommon){

	ModelManager* instance;
	instance = new ModelManager;

	instance->Initialize(dxCommon);

	return instance;
}

void ModelManager::Finalize(){
	deleteModels("ALL_MODEL");
	delete this;
}

void ModelManager::LoadModel(const string& modelName){
	Model* model;
	model = new Model;
	model->LoadModel(modelName);

	models.push_back(model);
}

Model* ModelManager::CallModel(const string& modelName){

	for (int i = 0; i < models.size(); i++) {
		if (models[i]->GetModelName() == modelName) {
			return models[i];
		}
	}
	assert(0);
	return nullptr;
}

void ModelManager::deleteModels(const string& modelName){
	if (modelName == "ALL_MODEL") {
		for (int i = 0; i < models.size(); i++) {
			delete models[i];
		}
	}
	else{
		for (int i = 0; i < models.size(); i++) {
			if (models[i]->GetModelName() == modelName) {
				models.erase(models.begin() + i);
			}
		}
	}
}

void ModelManager::Initialize(DirectXCommon* dxCommon){

	//3Dƒ‚ƒfƒ‹‹¤’Êˆ—
	Model::SetDevice(dxCommon->GetDevice());
	Model::SetCmdList(dxCommon->GetCmdList());
}

ModelManager::ModelManager(){
}
