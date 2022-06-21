#include "../Trb/TSFL.h"
#include <iostream>

// Writing our own data to SECT and saving pointers to RELC 

int main()
{
	TSFL tsfl; 

	// allocates 1 pointer to a float array
	float** ppArray = tsfl.AllocateSECT<float*>(1);

	// allocates 7 floats and saving the pointer to RELC and to ppArray
	float* pArray = tsfl.AllocateSECT<float>(7, ppArray);

	// filling the array
	for (int i = 0; i < 7; i++)
	{
		pArray[i] = (float)(i + 1);
	}

	tsfl.DumpSECT("CUSTOM.SECT");
}