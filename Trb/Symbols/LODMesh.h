#pragma once
#include "../Defines.h"

namespace LODMesh
{
	struct MeshInfo
	{
		size_t m_unk1;
		size_t m_vertexCount;
		size_t m_faceCount;
		size_t m_weightCount;
		void* m_weights;
		Vertex* m_vertices;
		unsigned short* m_faces;
		size_t m_zero;
		size_t m_unk2;
		Sphere m_sphere;
	};

	struct SLODMesh
	{
		size_t m_meshCount;
		size_t m_totalFaceCount;
		size_t m_totalVertexCount;
		const char* m_matName;
		MeshInfo* m_meshes;
	};
}