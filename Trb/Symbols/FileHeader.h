#pragma once

namespace FileHeader
{
	struct SFileHeader
	{
		char m_tmdl[4];
		int m_zero1;
		int m_unk;
		int m_zero2;
	};
}