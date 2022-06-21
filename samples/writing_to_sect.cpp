#include <iostream>

#include "../Trb/TSFL.h"
#include "../Defines.h"

// Writing our own data to SECT and saving pointers to RELC 

struct CustomData
{
	Vector3 pos;
	Vector2 scale;
};

int main()
{
	TSFL tsfl; 

	// allocating a pointer to float array and saving it in FloatArray SYMB
	tsfl.AddSymbol(0, "FloatArray", 0x0001, tsfl.AllocateSECT<float*>());
	// allocating memory for CustomData and saving it in CustomData SYMB
	tsfl.AddSymbol(0, "CustomData", 0x0002, tsfl.AllocateSECT<CustomData>());

	// CustomData
	CustomData* pData = tsfl.FindSymbol<CustomData>("CustomData");
	pData->pos = { 1.2f, 6.69f, 89.1f };
	pData->scale = { 1.0f, 1.0f };

	// FloatArray
	// allocating 7 floats at FloatArray and saving the pointer to RELC
	float* pArray = tsfl.AllocateSECT<float>(7, tsfl.FindSymbol<float*>("FloatArray"));

	// filling the array
	for (int i = 0; i < 7; i++) pArray[i] = (float)(i + 1);

	// dumping SECT data to a file
	tsfl.DumpSECT("CUSTOM.SECT");
}