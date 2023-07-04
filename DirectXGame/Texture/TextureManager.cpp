#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

TextureManager* TextureManager::Create(DirectXCommon* dxCommon) {

	TextureManager* instance;
	instance = new TextureManager;

	instance->Initialize(dxCommon);

	return instance;
}

void TextureManager::Finalize() {
	deleteTextures("ALL_TEXTURE");
	delete this;
}

void TextureManager::LoadTexture(const string& textureName) {
	Texture* tex;
	tex = new Texture;
	tex->LoadTexture(textureName);

	textures.push_back(tex);
}

Texture* TextureManager::CallTexture(const string& textureName) {

	for (int i = 0; i < textures.size(); i++) {
		if (textures[i]->GetTextureName() == textureName) {
			return textures[i];
		}
	}
	assert(0);
	return nullptr;
}

void TextureManager::deleteTextures(const string& textureName) {
	if (textureName == "ALL_TEXTURE") {
		for (int i = 0; i < textures.size(); i++) {
			delete textures[i];
		}
	}
	else{
		for (int i = 0; i < textures.size(); i++) {
			if (textures[i]->GetTextureName() == textureName) {
				textures.erase(textures.begin() + i);
			}
		}
	}
}

void TextureManager::Initialize(DirectXCommon* dxCommon) {
	//テクスチャ共通処理
	Texture::SetDevice(dxCommon->GetDevice());
	Texture::SetCmdList(dxCommon->GetCmdList());
}

TextureManager::TextureManager() {
}
