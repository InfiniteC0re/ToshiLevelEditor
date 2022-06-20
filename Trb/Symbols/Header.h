#pragma once
#include "../../Defines.h"

namespace Header
{
	enum class MODELSHADER
	{
		MODELSHADER_SKIN = 0,
		MODELSHADER_WORLD = 2,
		MODELSHADER_GRASS = 4,
	};

	struct SHeader
	{
		int m_modelCount;

		/* not sure, it's not used in Barnyard (at least by terrains) */
		float m_lodDistance;
		int m_meshCount;
		int m_unk3;

		/* can be used to make a model with the grass shader */
		MODELSHADER m_shader;
	
		/* probably a rotation origin, not used by terrains */
		Vector3 m_origin;

		/* is it a radius? doesn't change anything */
		float m_radius;
	};
}