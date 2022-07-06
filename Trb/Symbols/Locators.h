#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace Locators
{
	struct Locator
	{
		glm::quat m_rot;
		glm::vec3 m_pos;
		unsigned short m_nameId;
		unsigned short m_flag;
	};

	struct SLocators
	{
		size_t m_locatorCount;
		Locator* m_locators;
		size_t m_stringCount;
		const char** m_names;
	};
}