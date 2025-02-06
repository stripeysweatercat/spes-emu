#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory()
{
}

uint8_t Memory::read(uint32_t a)
{
	if (a < 0x20000)
		return ram[a];
	return 0x00;
}

void Memory::write(uint32_t a, uint8_t d)
{
	if (a < 0x20000)
		ram[a] = d;
}