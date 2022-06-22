#pragma once
#include "../../Defines.h"

namespace Database
{
	struct Vertex
	{
		Vector3 pos;
		Vector3 normal;

		/* Use { 1, 1, 1 } to disable shadows */
		Vector3 tint;
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
		/* Origin of the mesh */
		Vector3 m_origin;
		/* Radius at which the mesh will be visible */
		float m_radius;
		MeshData* m_meshData;
	};

	/* the reason why I can't implement more than 1 mesh loading */
	struct MeshRenderGroup
	{
		Vector3 m_origin;
		float m_radius;
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