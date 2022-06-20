#pragma once
#include <cstdio>
#include <cassert>
#include <string>

#define ReadFileData(buffer, size, count, pFile) \
	assert(fread(buffer, size, count, pFile) != size && "EOF")

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |       \
                ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))

std::string read_str_until_zero(FILE* pFile, int* length = nullptr);

std::string read_str_offset(FILE* pFile, long offset, int* length = nullptr);