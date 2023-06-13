#include "GameScene.h"

//�R���X�g���N�^
GameScene::GameScene() {

}

//�f�X�g���N�^
GameScene::~GameScene() {
	delete input_;
	delete sprite_;
	delete spriteCommon_;
}

//����������
void GameScene::Initialize(WinApp* winApp , DirectXCommon* dxCommon) {
	//�X�v���C�g���ʏ���
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon);

	// �L�[�{�[�h�f�o�C�X�̐���
	input_ = new Input();
	input_->Initialize(winApp);

	//�Q�[�����[�v�O����������
	spriteCommon_->LoadTexture(0 , "texture.jpg");
	spriteCommon_->LoadTexture(1 , "reimu.png");

	//�X�v���C�g
	sprite_ = new Sprite;
	sprite_->Initialize(spriteCommon_ , 1);
}

//�X�V����
void GameScene::Update() {
	//DirectX���t���[������
	//�X�V����
	input_->Update();

}

//�`�揈��
void GameScene::Draw() {
	//�X�v���C�g�`��
	spriteCommon_->PreDraw();

	sprite_->Draw();
}