#include "GameScene.h"

//�R���X�g���N�^
GameScene::GameScene() {

}

//�f�X�g���N�^
GameScene::~GameScene() {
	delete input_;
	delete sprite_;
	delete spriteCommon_;
	delete particleManager_;
	delete camera_;
	delete object1_;
	delete object2_;
	delete object3_;
	delete object4_;
	delete billboard_;
	delete billboardY_;
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
	Object3D::StaticInitialize(dxCommon , camera_);

	//�r���{�[�h���ʏ���
	Billboard::StaticInitialize(dxCommon , camera_);
	BillboardY::StaticInitialize(dxCommon , camera_);

	//�L�[�{�[�h�f�o�C�X�̐���
	input_ = new Input();
	input_->Initialize(winApp);

	//���f���ǂݍ���
	modelManager_->LoadModel("cube");

	//�e�N�X�`���ǂݍ���
	textureManager_->LoadTexture("texture.jpg");
	textureManager_->LoadTexture("white1x1.png");
	textureManager_->LoadTexture("reimu.png");
	textureManager_->LoadTexture("cube.jpg");
	textureManager_->LoadTexture("effect1.png");
	textureManager_->LoadTexture("effect2.png");
	textureManager_->LoadTexture("effect3.png");

#pragma region//test
	//�X�v���C�g������
	sprite_ = new Sprite;
	sprite_->Initialize(spriteCommon_ , textureManager_->CallTexture("texture"));
	sprite_->SetPosition({100 , 100});
	sprite_->SetTextureSize(sprite_->GetTextureSize() / 2);

	//3D���f��������
	object1_ = new Object3D;
	object1_->Initialize();
	object1_->SetModel(modelManager_->CallModel("cube"));
	object1_->SetTexture(textureManager_->CallTexture("white1x1"));
	//3D���f��������
	object2_ = new Object3D;
	object2_->Initialize();
	object2_->SetModel(modelManager_->CallModel("cube"));
	object2_->SetTexture(textureManager_->CallTexture("white1x1"));
	object2_->SetPosition({10 , 0 , -10});
	//3D���f��������
	object3_ = new Object3D;
	object3_->Initialize();
	object3_->SetModel(modelManager_->CallModel("cube"));
	object3_->SetTexture(textureManager_->CallTexture("white1x1"));
	object3_->SetPosition({0 , 0 , -20});

	object4_ = new Object3D;
	object4_->Initialize();
	object4_->SetModel(modelManager_->CallModel("cube"));
	object4_->SetTexture(textureManager_->CallTexture("white1x1"));
	object4_->SetPosition({-10 , 0 , -10});
	//3D���f��������
	object5_ = new Object3D;
	object5_->Initialize();
	object5_->SetModel(modelManager_->CallModel("cube"));
	object5_->SetTexture(textureManager_->CallTexture("white1x1"));
	object5_->SetPosition({0 , 10 , -10});
	//3D���f��������
	object6_ = new Object3D;
	object6_->Initialize();
	object6_->SetModel(modelManager_->CallModel("cube"));
	object6_->SetTexture(textureManager_->CallTexture("white1x1"));
	object6_->SetPosition({0 , -10 , -10});

	//�r���{�[�h������
	billboard_ = new Billboard;
	billboard_->Initialize();
	billboard_->SetTexture(textureManager_->CallTexture("texture"));
	billboard_->SetPosition({3 , 0 , 0});

	//�r���{�[�h������
	billboardY_ = new BillboardY;
	billboardY_->Initialize();
	billboardY_->SetTexture(textureManager_->CallTexture("reimu"));
	billboardY_->SetPosition({-3 , 0 , 0});

	//�p�[�e�B�N���}�l�[�W��
	ParticleManager::StaticInitialize(dxCommon , camera_);
	particleManager_ = new ParticleManager;
	particleManager_->Initialize();
	particleManager_->SetTexture(textureManager_->CallTexture("effect1"));

	pos = {0 , 0 , -10};
	rotation = {0 , 0 , 0};
#pragma endregion//test
}

//�X�V����
void GameScene::Update() {
	//DirectX���t���[������
	//�X�V����

#pragma region//test

	input_->Update();

	pos = {0 , 0 , 0};

	if (input_->PushKey(DIK_D)) {
		pos.x += 0.1f;
	}
	if (input_->PushKey(DIK_A)) {
		pos.x -= 0.1f;
	}
	if (input_->PushKey(DIK_W)) {
		pos.z += 0.1f;
	}
	if (input_->PushKey(DIK_S)) {
		pos.z -= 0.1f;
	}
	if (input_->PushKey(DIK_Z)) {
		pos.y += 0.1f;
	}
	if (input_->PushKey(DIK_C)) {
		pos.y -= 0.1f;
	}

	Quaternion rotaQ = MathFunc::Utility::MakeAxisAngle({0.0f , 0.0f , 1.0f} , camera_->GetRotation().z);
	pos = MathFunc::Utility::RotateVector(pos , rotaQ);

	rotaQ = MathFunc::Utility::MakeAxisAngle({1.0f , 0.0f , 0.0f} , camera_->GetRotation().x);
	pos = MathFunc::Utility::RotateVector(pos , rotaQ);

	rotaQ = MathFunc::Utility::MakeAxisAngle({0.0f , 1.0f , 0.0f} , camera_->GetRotation().y);
	pos = MathFunc::Utility::RotateVector(pos , rotaQ);

	//if (input_->PushKey(DIK_UP)) {
	//	rotation.x = object1_->GetRotation().x + MathFunc::Utility::Deg2Rad(1.0f);
	//}
	//if (input_->PushKey(DIK_DOWN)) {
	//	rotation.x = object1_->GetRotation().x - MathFunc::Utility::Deg2Rad(1.0f);
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	rotation.y = object1_->GetRotation().y - MathFunc::Utility::Deg2Rad(1.0f);
	//}
	//if (input_->PushKey(DIK_RIGHT)) {
	//	rotation.y = object1_->GetRotation().y + MathFunc::Utility::Deg2Rad(1.0f);
	//}
	//if (input_->PushKey(DIK_X)) {
	//	rotation.z = object1_->GetRotation().z + MathFunc::Utility::Deg2Rad(1.0f);
	//}

	if (input_->PushKey(DIK_UP)) {
		rotation.x -= MathFunc::Utility::Deg2Rad(1.0f);
	}
	if (input_->PushKey(DIK_DOWN)) {
		rotation.x += MathFunc::Utility::Deg2Rad(1.0f);
	}
	if (input_->PushKey(DIK_LEFT)) {
		rotation.y -= MathFunc::Utility::Deg2Rad(1.0f);
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotation.y += MathFunc::Utility::Deg2Rad(1.0f);
	}
	if (input_->PushKey(DIK_X)) {
		rotation.z += MathFunc::Utility::Deg2Rad(1.0f);
	}

	camera_->SetPosition(camera_->GetPosition() + pos);
	camera_->SetRotation(rotation);

	//object1_->SetRotation(rotation);

	camera_->Update();

	if (input_->PushKey(DIK_SPACE)) {
		object1_->SetTexture(textureManager_->CallTexture("cube"));
		sprite_->SetTexture(textureManager_->CallTexture("reimu"));
	}
	else {
		object1_->SetTexture(textureManager_->CallTexture("white1x1"));
		sprite_->SetTexture(textureManager_->CallTexture("texture"));
	}

	if (input_->PushKey(DIK_P)) {
		for (int i = 0; i < 5; i++) {
			Vector3 pos = {0 , 0 , 0};
			const float rnd_pos = 10.0f;
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;


			Vector3 vel;
			const float rnd_vel = 0.1f;
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			Vector3 acc;
			const float rnd_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

			particleManager_->Add(150 , pos , vel , acc , 1.0f , 0.0f);
		}
	}


	object1_->Update();
	object2_->Update();
	object3_->Update();
	object4_->Update();
	object5_->Update();
	object6_->Update();
	billboard_->Update();
	billboardY_->Update();
	sprite_->Update();
	particleManager_->Update();

#pragma endregion//test

}

//�`�揈��
void GameScene::Draw() {
#pragma region//test
	//3D�`��
	object1_->Draw();
	object2_->Draw();
	object3_->Draw();
	object4_->Draw();
	object5_->Draw();
	object6_->Draw();

	billboard_->Draw();
	billboardY_->Draw();

	particleManager_->Draw();
#pragma endregion//test

	//�X�v���C�g�`��
	spriteCommon_->PreDraw();

	//sprite_->Draw();
}