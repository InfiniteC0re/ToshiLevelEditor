#pragma once

namespace LocatorVIS
{
	struct Group
	{
		const char* m_name;
		size_t m_start;
		size_t m_size;
	};

	struct SLocatorVIS
	{
		size_t m_groupCount;
		Group* m_groups;
	};
}