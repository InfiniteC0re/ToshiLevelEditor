#include "Utils.h"
#include <vector>

std::string read_str_until_zero(FILE* pFile, int* length)
{
	std::vector<char> chars;

	char c;
	while (fread(&c, sizeof(char), 1, pFile))
	{
		if (c != '\x00')
		{
			chars.push_back(c);
		}
		else
		{
			break;
		}
	}

	std::string str(chars.begin(), chars.end());

	if (length)
	{
		*length = chars.size() + 1;
	}

	return str;
}

std::string read_str_offset(FILE* pFile, long offset, int* length)
{
	long old = ftell(pFile);

	fseek(pFile, offset, SEEK_SET);
	std::string str = read_str_until_zero(pFile, length);
	fseek(pFile, old, SEEK_SET);

	return str;
}

void append_bytes(std::vector<unsigned char>& dst_data, std::vector<unsigned char> scr_data, int begin, int count)
{
	for (size_t i = 0; i < count; i++)
	{
		dst_data.push_back(scr_data[begin + i]);
	}
}

void xor_data(std::vector<unsigned char>& data, int xorValue)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		data[i] ^= xorValue;
	}
}