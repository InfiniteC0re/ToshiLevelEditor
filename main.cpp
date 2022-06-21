#include <iostream>

#include "Trb/TRBFile.h"
#include "Trb/Symbols/Materials.h"
#include "Trb/Symbols/Collision.h"
#include "Trb/Symbols/Skeleton.h"
#include "Trb/Symbols/SkeletonHeader.h"

int main()
{
	TRBFile file("sample_files/Meadowsouth_L0Mod2.trb");
	
	TSFL* tsfl = file.GetTSFL();

	SECT* sect = tsfl->GetSECT();
	RELC* relc = tsfl->GetRELC();
	SYMB* symb = tsfl->GetSYMB();

	std::cout << "Successfully readed the file" << std::endl;

	Materials::SMaterials* materials = tsfl->FindSymbol<Materials::SMaterials>("Materials");
	Collision::SCollision* coll = tsfl->FindSymbol<Collision::SCollision>("Collision");
	SkeletonHeader::SSkeletonHeader* skeletonHeader = tsfl->FindSymbol<SkeletonHeader::SSkeletonHeader>("SkeletonHeader");
	Skeleton::SSkeleton* skeleton = tsfl->FindSymbol<Skeleton::SSkeleton>("Skeleton");
	
	if (materials)
	{
		std::cout << std::endl << "----Materials----" << std::endl;
		std::cout << "Found " << materials->m_count << " materials" << std::endl;
	}

	if (skeletonHeader)
	{
		std::cout << std::endl << "----SkeletonHeader----" << std::endl;
		std::cout << "TKL name: " << skeletonHeader->m_tklName << std::endl;
	}

	if (skeleton)
	{
		std::cout << std::endl << "----Skeleton----" << std::endl;
		std::cout << "Bone Count: " << skeleton->m_nodeCount << std::endl;
		std::cout << "Bones: ";
		for (int i = 0; i < skeleton->m_nodeCount; i++)
			std::cout << skeleton->m_nodes[i].m_name << "; ";

		std::cout << std::endl << "Animation Count: " << skeleton->m_animCount << std::endl;
		std::cout << "Animations: ";
		for (int i = 0; i < skeleton->m_animCount; i++)
			std::cout << skeleton->m_animations[i].m_name << "; ";
		std::cout << std::endl;
	}
}