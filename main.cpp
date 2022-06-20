#include <iostream>
#include <string>
#include <vector>

#include "Trb/TRBFile.h"
#include "Trb/Symbols/Collision.h"
#include "Trb/Symbols/Header.h"
#include "Trb/Symbols/Materials.h"

#include <NvTriStrip.h>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace Database;

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
	TRBFile file("samples/Barn_L0Mod0.trb");
	
	TSFL* tsfl = file.GetTSFL();

	// todo: hdrx
	SECT* sect = tsfl->GetSECT();
	RELC* relc = tsfl->GetRELC();
	SYMB* symb = tsfl->GetSYMB();

	std::cout << "Successfully readed the TRB file" << std::endl;

	SDatabase* db = symb->FindEntryByName<SDatabase>("Database");
	Header::SHeader* header = symb->FindEntryByName<Header::SHeader>("Header");

	Materials::SMaterials* materials = symb->FindEntryByName<Materials::SMaterials>("Materials");
	std::cout << "Found " << materials->m_count << " materials" << std::endl;

	const aiScene* scene = aiImportFile("./models/sphere_and_cube.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	
	Collision::SCollision* coll = symb->FindEntryByName<Collision::SCollision>("Collision");
	auto meshData = coll->m_infoArray.m_models[0]->m_meshes[0]->m_meshData;
	
	// todo: rebuild whole trb file
	if (scene->HasMeshes())
	{
		// todo: import more than 1 mesh
		assert(scene->mNumMeshes == 1 && "Not implemented");
		aiMesh** meshes = scene->mMeshes;

		std::cout << "Editing mesh of material " << meshData->m_matName << "\n";

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
	}

	sect->UnlinkRELC(relc);
	tsfl->DumpSECTToFile("./DUMP.SECT");
}