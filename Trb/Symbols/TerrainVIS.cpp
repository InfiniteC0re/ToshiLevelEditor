#include "TerrainVIS.h"
#include <cstdint>

TerrainVIS::SOrderdVIS::OrderdVISGroupInfo* TerrainVIS::SOrderdVIS::group_by_index(size_t index)
{
	OrderdVISGroupInfo* start = (OrderdVISGroupInfo*)this;
	return &start[index];
}