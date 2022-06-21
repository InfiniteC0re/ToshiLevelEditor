#include <iostream>
#include <string>
#include <vector>

#include "Trb/TRBFile.h"
#include "Trb/Symbols/Materials.h"
#include "Trb/Symbols/Collision.h"
#include "Trb/Symbols/Skeleton.h"
#include "Trb/Symbols/SkeletonHeader.h"

#include <NvTriStrip.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void GenerateStrips(std::vector<unsigned short> &indices, Database::MeshData* mesh)
{
	PrimitiveGroup* prims;

	unsigned short numprims;
	bool done = GenerateStrips(&indices[0], indices.size(), &prims, &numprims);
	
	if (done) {
		PrimitiveGroup pg = prims[0];
		mesh->m_faceCount = pg.numIndices;

		for (int i = 0; i < pg.numIndices; i++)
		{
			mesh->m_faces[i] = pg.indices[i];
		}
	}
}

int main()
{
	TRBFile file("sample_files/Barn_L0Mod0.trb");
	
	TSFL* tsfl = file.GetTSFL();

	// todo: hdrx
	SECT* sect = tsfl->GetSECT();
	RELC* relc = tsfl->GetRELC();
	SYMB* symb = tsfl->GetSYMB();

	std::cout << "Successfully readed the file" << std::endl;

	Materials::SMaterials* materials = tsfl->FindSymbol<Materials::SMaterials>("Materials");
	Collision::SCollision* coll = tsfl->FindSymbol<Collision::SCollision>("Collision");
	SkeletonHeader::SSkeletonHeader* skeletonHeader = tsfl->FindSymbol<SkeletonHeader::SSkeletonHeader>("SkeletonHeader");
	Skeleton::SSkeleton* skeleton = tsfl->FindSymbol<Skeleton::SSkeleton>("Skeleton");
	
	if (materials)
	{
		std::cout << std::endl << "----Materials----" << std::endl;
		std::cout << "Found " << materials->m_count << " materials" << std::endl;
	}

	if (skeletonHeader)
	{
		std::cout << std::endl << "----SkeletonHeader----" << std::endl;
		std::cout << "TKL name: " << skeletonHeader->m_tklName << std::endl;
	}

	if (skeleton)
	{
		std::cout << std::endl << "----Skeleton----" << std::endl;
		std::cout << "Bone Count: " << skeleton->m_nodeCount << std::endl;
		std::cout << "Bones: ";
		for (int i = 0; i < skeleton->m_nodeCount; i++)
			std::cout << skeleton->m_nodes[i].m_name << "; ";

		std::cout << std::endl << "Animation Count: " << skeleton->m_animCount << std::endl;
		std::cout << "Animations: ";
		for (int i = 0; i < skeleton->m_animCount; i++)
			std::cout << skeleton->m_animations[i].m_name << "; ";
		std::cout << std::endl;
	}

	if (coll && coll->m_infoArray.m_modelCount > 0)
	{
		const aiScene* scene = aiImportFile("./models/sphere_and_cube.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		auto meshData = coll->m_infoArray.m_models[0]->m_meshes[0]->m_meshData;

		// todo: rebuild whole trb file
		if (scene->HasMeshes())
		{
			// todo: import more than 1 mesh
			assert(scene->mNumMeshes == 1 && "Not implemented yet");
			aiMesh** meshes = scene->mMeshes;

			aiMesh* mesh = meshes[0];
			assert(meshData->m_vertexCount >= mesh->mNumVertices);
			assert(meshData->m_faceCount >= mesh->mNumFaces);

			meshData->m_vertexCount = mesh->mNumVertices;
			meshData->m_vertexCount2 = mesh->mNumVertices;

			// vertices
			for (int k = 0; k < mesh->mNumVertices; k++)
			{
				// pos
				meshData->m_vertices[k].pos = mesh->mVertices[k];

				// uv
				Vector3 texCoords = mesh->mTextureCoords[0][k];
				meshData->m_vertices[k].uv = { texCoords.x, texCoords.y };

				// normals
				meshData->m_vertices[k].normal = *mesh->mNormals;

				// shadows
				meshData->m_vertices[k].tint = { 1, 1, 1 };
			}

			std::vector<unsigned short> indices;
			for (int k = 0; k < mesh->mNumFaces; k++)
			{
				indices.push_back(mesh->mFaces[k].mIndices[0]);
				indices.push_back(mesh->mFaces[k].mIndices[1]);
				indices.push_back(mesh->mFaces[k].mIndices[2]);
			}

			GenerateStrips(indices, meshData);
			std::cout << std::endl << "Successfully replaced the first mesh with the custom one" << std::endl;
		}
		
		tsfl->DumpSECT("./DUMP.SECT");
	}
}