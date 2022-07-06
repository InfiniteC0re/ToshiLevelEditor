#pragma once
#include "../../Defines.h"

namespace Database
{
	struct Sphere
	{
		Vector3 m_origin;
		float m_radius;
	};

	struct Vertex
	{
		Vector3 pos;
		Vector3 normal;
		Vector3 color;
		Vector2 uv;
	};

	struct MeshData
	{
		size_t m_unk;
		size_t m_faceCount;
		size_t m_vertexCount;
		size_t m_vertexCount2;
		const char* m_matName;
		Vertex* m_vertices;
		unsigned short* m_faces;
	};

	struct Mesh
	{
		Sphere m_sphere;
		MeshData* m_meshData;
	};

	struct MeshRenderGroup
	{
		Sphere m_sphere;
		/* if m_nextGroup is 0 then RIGHT after this structure there's a MeshRenderGroupData structure */
		MeshRenderGroup* m_nextGroup;
	};

	struct MeshRenderGroupData
	{
		size_t m_meshCount;
	public:
		unsigned short get_mesh_index(int index);
	};

	struct Model
	{
		char m_customData[0x84];
		size_t m_meshCount;
		Mesh** m_meshes;
		MeshRenderGroup* m_renderGroups;
	};
	
	struct InfoArray
	{
		int m_modelCount;
		Model** m_models;
	};

	struct SDatabase
	{
		size_t m_infoCount;
		InfoArray** m_infoArray;
	};
}