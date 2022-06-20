#include "Materials.h"

Materials::Material* Materials::SMaterials::GetMaterial(int index)
{
	if (m_count > 0 && index < m_count)
	{
		size_t* pLastField = &m_size + 1;
		Materials::Material* materialsArray = (Materials::Material*)pLastField;

		return &materialsArray[index];
	}

	return nullptr;
}