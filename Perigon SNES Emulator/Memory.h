#pragma once
#include <cstdint>

class Memory
{
public:
	Memory();
	~Memory();

	uint8_t ram[0x1000000];

	uint8_t read(uint32_t a);
	void write(uint32_t a, uint8_t d);
};