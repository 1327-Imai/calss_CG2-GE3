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

	//モデルと同じ生のフォルダから読み込む
	const string directoryPath = baseDirectory + modelName + "/";

	//拡張子.fbxを追加
	const string fileName = modelName + ".obj";

	//連結してフルパスを得る
	const string fullpath = directoryPath + fileName;

	//ファイルストリーム
	std::fstream file;
	//.objファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	std::vector<Vector3>positions;	//頂点座標
	std::vector<Vector3>normals;	//法線ベクトル
	std::vector<Vector2>texcoords;	//テクスチャUV
	//1行ずつ読み込む
	std::string line;
	while (getline(file , line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字を取得
		std::string key;
		getline(line_stream , key , ' ');

		//先頭文字列がvなら頂点座標
		if (key == "v") {

			//X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//座標データに追加
			positions.emplace_back(position);

		}

		//先頭の文字が	vtならテクスチャ
		if (key == "vt") {
			
			//UV成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//V方向反転
			texcoord.y = 1.0f - texcoord.y;

			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);

		}

		//先頭の文字がvnなら法線ベクトル
		if (key == "vn") {

			//xyz成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			//法線ベクトルデータに追加
			normals.emplace_back(normal);

		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f") {

			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream , index_string , ' ')) {

				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition,indexNormal,indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1 , std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1 , std::ios_base::cur);
				index_stream >> indexNormal;

				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//インデックスデータの追加
				indices.emplace_back((unsigned short)indices.size());
			}

		}

	}
	file.close();

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0 , nullptr);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0 , nullptr , (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//座標をコピー
	}
	//繋がりを解除
	indexBuff->Unmap(0 , nullptr);

	//頂点バッファビューの作成
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	this->modelName = modelName;
}

void Model::Draw(){
	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0 , 1 , &vbView);
	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);
	//描画コマンド
	cmdList->DrawIndexedInstanced(indices.size() , 1 , 0 , 0 , 0);
}
