#include <iostream>

#include "Trb/TRBFile.h"
#include "Trb/Symbols/Materials.h"
#include "Trb/Symbols/Collision.h"
#include "Trb/Symbols/Skeleton.h"
#include "Trb/Symbols/SkeletonHeader.h"

int main()
{
	TRBFile file("sample_files/Barn_L0Mod0.TRB");
	
	TSFL* tsfl = file.GetTSFL();

	SECT* sect = tsfl->GetSECT();
	RELC* relc = tsfl->GetRELC();
	SYMB* symb = tsfl->GetSYMB();

	std::cout << "Successfully readed the file" << std::endl;

	auto materials = tsfl->FindSymbol<Materials::SMaterials>("Materials");
	auto coll = tsfl->FindSymbol<Collision::SCollision>("Collision");
	auto skeletonHeader = tsfl->FindSymbol<SkeletonHeader::SSkeletonHeader>("SkeletonHeader");
	auto skeleton = tsfl->FindSymbol<Skeleton::SSkeleton>("Skeleton");
	
	if (materials.valid())
	{
		std::cout << std::endl << "----Materials----" << std::endl;
		std::cout << "Found " << materials.data()->m_count << " materials" << std::endl;
	}

	if (skeletonHeader.valid())
	{
		std::cout << std::endl << "----SkeletonHeader----" << std::endl;
		std::cout << "TKL name: " << skeletonHeader.data()->m_tklName << std::endl;
	}

	if (skeleton.valid())
	{
		std::cout << std::endl << "----Skeleton----" << std::endl;
		std::cout << "Bone Count: " << skeleton.data()->m_nodeCount << std::endl;
		std::cout << "Bones: ";
		for (int i = 0; i < skeleton.data()->m_nodeCount; i++)
			std::cout << skeleton.data()->m_nodes[i].m_name << "; ";

		std::cout << std::endl << "Animation Count: " << skeleton.data()->m_animCount << std::endl;
		std::cout << "Animations: ";
		for (int i = 0; i < skeleton.data()->m_animCount; i++)
			std::cout << skeleton.data()->m_animations[i].m_name << "; ";
		std::cout << std::endl;
	}

	file.Save("./test.trb");
}