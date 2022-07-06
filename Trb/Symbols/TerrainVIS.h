#pragma once

namespace TerrainVIS
{
	struct MatLibInfo
	{
		const char* m_name;
		size_t m_unk1;
		size_t m_unk2;
	};

	struct TerrainChunk
	{
		const char* m_name;
		const char** m_lods0;
		const char** m_lods1;
		size_t m_lod0Count;
		size_t m_lod1Count;
		void* m_unk1;
		void* m_unk2;
		const char* m_coll;
		size_t m_unk3;
		size_t m_unk4;
		size_t m_unk5;
		MatLibInfo m_lod0Ttl;
		MatLibInfo m_lod1Ttl;
		size_t m_unk6;
		size_t m_unk7;
		unsigned short m_unk8;
		unsigned short m_unk9;
		size_t m_unk10;
		size_t m_unk11;
		size_t m_unk12;
		size_t m_unk13;
		size_t m_unk14;
	};

	struct STerrainVIS
	{
		const char* m_tklName;
		const char* m_ttlName;
		const char* m_skelName;
		size_t m_zero1;
		size_t m_zero2;
		size_t m_zero3;
		size_t m_zero4;
		size_t m_chunkCount;
		TerrainChunk* m_chunks;
		size_t m_memoryBlockSize;
	};

	struct SOrderdVIS
	{
	public:
		struct OrderdVISGroup
		{
			unsigned short m_chunkIndex;
			unsigned short m_modelIndex;
		};

		struct OrderdVISGroupInfo
		{
			size_t m_count;
			OrderdVISGroup* m_groups;
		};

		OrderdVISGroupInfo* group_by_index(size_t index);
	};
}