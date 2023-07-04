#include "Model.h"

ID3D12Device* Model::device = nullptr;
ID3D12GraphicsCommandList* Model::cmdList = nullptr;

const std::string Model::baseDirectory = "Resources/3DObject/";

Model::Model(){
}

Model::~Model(){
}

void Model::LoadModel(const string& modelName){
	HRESULT result;

	//���f���Ɠ������̃t�H���_����ǂݍ���
	const string directoryPath = baseDirectory + modelName + "/";

	//�g���q.fbx��ǉ�
	const string fileName = modelName + ".obj";

	//�A�����ăt���p�X�𓾂�
	const string fullpath = directoryPath + fileName;

	//�t�@�C���X�g���[��
	std::fstream file;
	//.obj�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	std::vector<Vector3>positions;	//���_���W
	std::vector<Vector3>normals;	//�@���x�N�g��
	std::vector<Vector2>texcoords;	//�e�N�X�`��UV
	//1�s���ǂݍ���
	std::string line;
	while (getline(file , line)) {

		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		getline(line_stream , key , ' ');

		//�擪������v�Ȃ璸�_���W
		if (key == "v") {

			//X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

		}

		//�擪�̕�����	vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			
			//UV�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//V�������]
			texcoord.y = 1.0f - texcoord.y;

			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);

		}

		//�擪�̕�����vn�Ȃ�@���x�N�g��
		if (key == "vn") {

			//xyz�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);

		}

		//�擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f") {

			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream , index_string , ' ')) {

				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition,indexNormal,indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1 , std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1 , std::ios_base::cur);
				index_stream >> indexNormal;

				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//�C���f�b�N�X�f�[�^�̒ǉ�
				indices.emplace_back((unsigned short)indices.size());
			}

		}

	}
	file.close();

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff)
	);
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//���W���R�s�[
	}
	//�q���������
	vertBuff->Unmap(0 , nullptr);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0 , nullptr , (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//���W���R�s�[
	}
	//�q���������
	indexBuff->Unmap(0 , nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_�P���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	this->modelName = modelName;
}

void Model::Draw(){
	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0 , 1 , &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);
	//�`��R�}���h
	cmdList->DrawIndexedInstanced(indices.size() , 1 , 0 , 0 , 0);
}
