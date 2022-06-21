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

	struct ModelRenderInfo
	{
		Vector3 m_origin;
		float m_radius;
		size_t m_unk1;
		size_t m_unk2;
		size_t m_unk3;
	};

	struct Model
	{
		char m_customData[0x84];
		size_t m_meshCount;
		Mesh** m_meshes;
		ModelRenderInfo* m_subInfo;
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