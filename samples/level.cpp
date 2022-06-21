#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

#include "../Trb/TRBFile.h"
#include "../Trb/Symbols/Materials.h"
#include "../Trb/Symbols/Collision.h"

#include <NvTriStrip.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "../Trb/Symbols/Header.h"

void GenerateStrips(std::vector<unsigned short>& indices, TSFL* tsfl, SECTPtr<Database::MeshData> mesh)
{
	PrimitiveGroup* prims;

	unsigned short numprims;
	bool done = GenerateStrips(&indices[0], indices.size(), &prims, &numprims);

	if (done) {
		PrimitiveGroup pg = prims[0];

		mesh.data()->m_faceCount = pg.numIndices;
		tsfl->AllocateSECT<unsigned short>(&mesh.data()->m_faces, pg.numIndices);
		
		for (int i = 0; i < pg.numIndices; i++)
		{
			mesh.data()->m_faces[i] = pg.indices[i];
		}
	}
}

int main()
{
	TRBFile file;

	TSFL* tsfl = file.GetTSFL();
	SECT* sect = tsfl->GetSECT();
	
	auto header = tsfl->AllocateSECT<Header::SHeader>();
	
	header.data()->m_lodDistance = 100.f;
	header.data()->m_meshCount = 1;
	header.data()->m_modelCount = 1;
	header.data()->m_origin = { 0, 0, 0 };
	header.data()->m_radius = 256;
	header.data()->m_shader = Header::MODELSHADER::MODELSHADER_WORLD;
	header.data()->m_unk3 = 0;

	auto database = tsfl->AllocateSECT<Database::SDatabase>();

	database.data()->m_infoCount = 1;
	auto pInfoArray = tsfl->AllocateSECT<Database::InfoArray*>(&database.data()->m_infoArray);
	
	auto materials = tsfl->AllocateSECT<Materials::SMaterials>();
	materials.data()->m_zero1 = 0;
	materials.data()->m_zero2 = 0;
	materials.data()->m_count = 1;
	materials.data()->m_size = sizeof(Materials::Material) * materials.data()->m_count;
	
	auto material = tsfl->AllocateSECT<Materials::Material>();
	strcpy(material.data()->m_matName, "grassmain");
	strcpy(material.data()->m_matFile, "World\\grassmain.tga");

	tsfl->AddSymbol(0, "Database", 30107, database.data());
	tsfl->AddSymbol(0, "Header", 52909, header.data());
	tsfl->AddSymbol(0, "Materials", 24588, materials.data());
	tsfl->AddSymbol(0, "Collision", 26386, tsfl->AllocateSECT<int>().data());
	
	auto infoArray = tsfl->AllocateSECT<Database::InfoArray>(pInfoArray);
	infoArray.data()->m_modelCount = 1;

	auto pModels = tsfl->AllocateSECT<Database::Model*>(&infoArray.data()->m_models);
	auto model = tsfl->AllocateSECT<Database::Model>(pModels);
	model.data()->m_meshCount = 1;
	tsfl->AllocateSECT<Database::ModelRenderInfo>(&model.data()->m_subInfo);
	model.data()->m_subInfo->m_unk2 = 1; // count of meshes????

	auto pMeshes = tsfl->AllocateSECT<Database::Mesh*>(&model.data()->m_meshes);
	auto mesh = tsfl->AllocateSECT<Database::Mesh>(pMeshes);
	mesh.data()->m_origin = { 0, 0, 0 };
	mesh.data()->m_radius = 256;

	auto meshData = tsfl->AllocateSECT<Database::MeshData>(&mesh.data()->m_meshData);
	
	const aiScene* scene = aiImportFile("./models/barn_L0Mod0.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene->HasMeshes())
	{
		// todo: import more than 1 mesh
		assert(scene->mNumMeshes == 1 && "Not implemented yet");
		aiMesh** meshes = scene->mMeshes;

		aiMesh* aimesh = meshes[0];

		auto matNameArray = tsfl->AllocateSECT<char>(&meshData.data()->m_matName, 12);
		strcpy(matNameArray.data(), material.data()->m_matName);
		meshData.data()->m_vertexCount = aimesh->mNumVertices;
		meshData.data()->m_vertexCount2 = aimesh->mNumVertices;
		tsfl->AllocateSECT<Database::Vertex>(&meshData.data()->m_vertices, aimesh->mNumVertices);

		// vertices
		for (int k = 0; k < aimesh->mNumVertices; k++)
		{
			// pos
			meshData.data()->m_vertices[k].pos = aimesh->mVertices[k];

			// uv
			Vector3 texCoords = aimesh->mTextureCoords[0][k];
			meshData.data()->m_vertices[k].uv = { texCoords.x, texCoords.y };

			// normals
			meshData.data()->m_vertices[k].normal = *aimesh->mNormals;

			// shadows
			meshData.data()->m_vertices[k].tint = { 1, 1, 1 };
		}

		std::vector<unsigned short> indices;
		for (int k = 0; k < aimesh->mNumFaces; k++)
		{
			indices.push_back(aimesh->mFaces[k].mIndices[0]);
			indices.push_back(aimesh->mFaces[k].mIndices[1]);
			indices.push_back(aimesh->mFaces[k].mIndices[2]);
		}

		GenerateStrips(indices, tsfl, meshData);
		std::cout << std::endl << "Done!" << std::endl;
	}

	tsfl->AllocateSECT<char>(2);
	auto db = database.data();
	file.Save("./Barn_L0Mod0.trb");
}