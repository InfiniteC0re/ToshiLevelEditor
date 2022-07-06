#pragma once

namespace S_TTL
{
	struct TextureInfo
	{
		size_t m_hdrx;
		const char* m_name;
		size_t m_size;
		const unsigned char* m_data;
	};

	struct TTL
	{
		size_t m_textureCount;
		TextureInfo* m_textures;
		const char* m_libName;
	};
}