#pragma once

namespace Materials
{
	struct Material
	{
		char m_matName[0x68];
		char m_matFile[0xC0];
	};

	struct SMaterials
	{
		int m_zero1;
		int m_zero2;
		size_t m_count;
		size_t m_size;

		Material* GetMaterial(int index);
	};

	static_assert(sizeof(Materials::Material) == 0x128, "invalid size of Materials::Material");
}