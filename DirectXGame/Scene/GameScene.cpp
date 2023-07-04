#include "GameScene.h"

//�R���X�g���N�^
GameScene::GameScene() {

}

//�f�X�g���N�^
GameScene::~GameScene() {
	delete input_;
	delete sprite_;
	delete spriteCommon_;
	delete camera_;
	delete object_;
	modelManager_->Finalize();
	textureManager_->Finalize();
}

//����������
void GameScene::Initialize(WinApp* winApp , DirectXCommon* dxCommon) {
	//�X�v���C�g���ʏ���
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon);

	//���f���}�l�[�W���[
	modelManager_ = ModelManager::Create(dxCommon);
	
	//�e�N�X�`���}�l�[�W���[
	textureManager_ = TextureManager::Create(dxCommon);

	//�J����������
	camera_ = new Camera;
	camera_->Initialize();

	//3D�I�u�W�F�N�g���ʏ���
	Object3D::StaticInitialize(dxCommon,camera_);

	// �L�[�{�[�h�f�o�C�X�̐���
	input_ = new Input();
	input_->Initialize(winApp);

	//���f���ǂݍ���
	modelManager_->LoadModel("cube");

	//�e�N�X�`���ǂݍ���
	textureManager_->LoadTexture("texture.jpg");
	textureManager_->LoadTexture("white1x1.png");
	textureManager_->LoadTexture("reimu.png");
	textureManager_->LoadTexture("cube.jpg");


	//�X�v���C�g������
	sprite_ = new Sprite;
	sprite_->Initialize(spriteCommon_ , textureManager_->CallTexture("texture"));

	//3D���f��������
	object_ = new Object3D;
	object_->Initialize();
	object_->SetModel(modelManager_->CallModel("cube"));
	object_->SetTexture(textureManager_->CallTexture("white1x1"));

	pos = {0 , 0 , -10};
	rotation = {0 , 0 , 0};

}

//�X�V����
void GameScene::Update() {
	//DirectX���t���[������
	//�X�V����
	input_->Update();


	if (input_->PushKey(DIK_D)) {
		pos.x += MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_A)) {
		pos.x -= MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_W)) {
		pos.y += MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_S)) {
		pos.y -= MathFunc::Utility::Deg2Rad(5);
	}

	if (input_->PushKey(DIK_UP)) {
		rotation.x += MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_DOWN)) {
		rotation.x -= MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_LEFT)) {
		rotation.y -= MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotation.y += MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_X)) {
		rotation.z += MathFunc::Utility::Deg2Rad(30);
	}

	camera_->SetPosition(pos);
	camera_->SetRotation(rotation);

	camera_->Update();

	if (input_->PushKey(DIK_SPACE)) {
		object_->SetTexture(textureManager_->CallTexture("cube"));
		sprite_->SetTexture(textureManager_->CallTexture("reimu"));
	}
	else {
		object_->SetTexture(textureManager_->CallTexture("white1x1"));
		sprite_->SetTexture(textureManager_->CallTexture("texture"));
	}



	object_->Update();
	sprite_->Update();

}

//�`�揈��
void GameScene::Draw() {
	//3D�`��
	object_->Draw();
	
	//�X�v���C�g�`��
	spriteCommon_->PreDraw();

	sprite_->Draw();
}