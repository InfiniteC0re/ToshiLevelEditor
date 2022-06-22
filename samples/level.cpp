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

	const aiScene* scene = aiImportFile("./models/Barn_L0Mod0.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	
	header.data()->m_lodDistance = 100.f;
	header.data()->m_meshCount = scene->mNumMeshes;
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
	materials.data()->m_count = scene->mNumMaterials;
	materials.data()->m_size = sizeof(Materials::Material) * materials.data()->m_count;

	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		auto aimaterial = scene->mMaterials[i];
		auto material = tsfl->AllocateSECT<Materials::Material>();

		aiString texPath;
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);

		strcpy(material.data()->m_matName, aimaterial->GetName().C_Str());
		strcpy(material.data()->m_matFile, texPath.C_Str());
	}

	tsfl->AddSymbol(0, "Database", 30107, database.data());
	tsfl->AddSymbol(0, "Header", 52909, header.data());
	tsfl->AddSymbol(0, "Materials", 24588, materials.data());
	tsfl->AddSymbol(0, "Collision", 26386, tsfl->AllocateSECT<int>().data());
	
	auto infoArray = tsfl->AllocateSECT<Database::InfoArray>(pInfoArray);
	infoArray.data()->m_modelCount = 1;

	auto pModels = tsfl->AllocateSECT<Database::Model*>(&infoArray.data()->m_models);
	auto model = tsfl->AllocateSECT<Database::Model>(pModels);
	model.data()->m_meshCount = scene->mNumMeshes;
	
	// IMPORTANT: Render groups
	tsfl->AllocateSECT<Database::MeshRenderGroup>(&model.data()->m_renderGroups);
	auto renderGroupData = tsfl->AllocateSECT<Database::MeshRenderGroupData>();
	
	// Adding all the meshes in one group
	renderGroupData.data()->m_meshCount = scene->mNumMeshes;
	auto meshIndexes = tsfl->AllocateSECT<unsigned short>(scene->mNumMeshes);
	for (int i = 0; i < scene->mNumMeshes; i++) meshIndexes.data()[i] = i;

	auto pMeshes = tsfl->AllocateSECT<Database::Mesh*>(&model.data()->m_meshes, scene->mNumMeshes);

	if (scene->HasMeshes())
	{
		aiMesh** meshes = scene->mMeshes;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* aimesh = meshes[i];

			auto mesh = tsfl->AllocateSECT<Database::Mesh>(&pMeshes.data()[i]);
			mesh.data()->m_origin = { 0, 0, 0 };
			mesh.data()->m_radius = 256;

			auto meshData = tsfl->AllocateSECT<Database::MeshData>(&mesh.data()->m_meshData);

			const char* matName = scene->mMaterials[aimesh->mMaterialIndex]->GetName().C_Str();
			auto matNameArray = tsfl->AllocateSECT<char>(&meshData.data()->m_matName, strlen(matName) + 1);
			strcpy(matNameArray.data(), matName);
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
				meshData.data()->m_vertices[k].uv = { texCoords.x, -texCoords.y };

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
		}

		std::cout << std::endl << "Done!" << std::endl;
	}

	auto db = database.data();

	file.Save("D:\\Barnyard\\Game\\Data\\Terrain\\EnvMain\\Barn_L0Mod0.trb");
}