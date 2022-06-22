#include "Database.h"

unsigned short Database::MeshRenderGroupData::get_mesh_index(int index)
{
	short* start = (short*)((&m_meshCount) + 1);
	return start[index];
}
