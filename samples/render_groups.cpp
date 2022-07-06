#include <iostream>

#include "../Trb/TRBFile.h"
#include "../Trb/Symbols/Database.h"

void GetMeshRenderGroupInfo(Database::MeshRenderGroup* info)
{
	auto next = info->m_nextGroup;
	auto node = info;

	while (next != 0)
	{
		GetMeshRenderGroupInfo(&node[1]);
		node = next;
		next = node->m_nextGroup;
	}
	
	// MeshRenderGroupData is created ONLY if there's no any last->m_nextGroup
	Database::MeshRenderGroupData* data = (Database::MeshRenderGroupData*)&node[1];
	std::cout << "Meshes in this group: " << data->m_meshCount << std::endl;

	for (int i = 0; i < data->m_meshCount; i++)
	{
		std::cout << "Mesh index: " << data->get_mesh_index(i) << std::endl;
	}

	std::cout << std::endl;
}

int main()
{
	TRBFile file("sample_files/Barn_L0Mod0.trb");

	TSFL* tsfl = file.GetTSFL();

	SECT* sect = tsfl->GetSECT();
	RELC* relc = tsfl->GetRELC();
	SYMB* symb = tsfl->GetSYMB();

	std::cout << "Successfully readed the file" << std::endl;

	auto database = tsfl->FindSymbol<Database::SDatabase>("Database");

	if (database.valid())
	{
		GetMeshRenderGroupInfo(database.data()->m_infoArray[0]->m_models[0]->m_renderGroups);
	}

	file.Save("D:\\Barnyard\\Game\\Data\\Terrain\\EnvMain\\Barn_L0Mod0.trb");
}