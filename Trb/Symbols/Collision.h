#pragma once
#include "../../Defines.h"

namespace Collision
{
	struct CollisionModel
	{
		size_t m_unk1;
		Vector3* m_vertices;
		size_t m_vertexCount;
		unsigned short* m_faces;
		size_t m_faceCount;
		size_t m_unk2;
		const char** m_collName;
		size_t m_unk3;
	};

	struct SCollision
	{
		int m_count;
		CollisionModel* m_models;
	};
}