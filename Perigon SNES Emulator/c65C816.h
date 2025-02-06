#pragma once
#include <cstdint>
#include "Memory.h"

class c65C816
{
public:
	c65C816();
	Memory* mem;
	void ConnectMem(Memory *m);
	int clock();

	// Interrupt Vectors between 0xFFE0 and 0xFFFF

	uint8_t opcode = 0x00;
	uint8_t TCU = 0x00;			// advances per cycle of instruction completed; reset to 0 before each instruction
	uint8_t DBR = 0x00;			// Data Bank Register, holds address to data bank being taken from. combined with instruction 16 bit addr to create full 24 bit addr
	uint8_t PBR = 0x00;			// Process Bank Register, bank address for all instruction fetches
	uint16_t pc = 0x0000;		// Program Counter, self explanitory
	uint16_t sp = 0x0000;		// Stack Pointer, in emulation mode high byte is set to 1. 0x0000 to 0xFFFF range

	uint16_t a = 0x0000;		// 16 bit in native mode when accumulator select bit is 1, 8 bit else
	uint16_t direct = 0x0000;	// Provides address offset for direct addressing instructions

	uint16_t X = 0x0000;		// General purpose register, often used to compute effective address.
	uint16_t Y = 0x0000;		// in native mode, both are 16 bit if Index Select Bit of the processor status register is 0

	uint8_t p = 0x00;
	enum Flags
	{
		C			= 0x01,			// Carry
		N			= 0x01 << 1,	// Negative
		V			= 0x01 << 2,	// Overflow
		Z			= 0x01 << 3,	// Zero
		D			= 0x01 << 4,	// Decimal
		I			= 0x01 << 5,	// IRQ Disable
		M			= 0x01 << 6,	// Memory/Accumulator, sets accumulator size
		X			= 0x01 << 7		// Index, sets index register size

	};
	bool native = false;
	bool emulation = false;

	void SetFlag(Flags f, bool v);
	bool GetFlag(Flags f);

	// ADDRESSING MODES

	uint32_t addr_abs   = 0;
	uint16_t addr_ind   = 0;
	uint8_t  addr_lo    = 0x00;
	uint8_t  addr_hi    = 0x00;
	uint8_t  addr_b     = 0x00;
	uint8_t  indOffset  = 0x00;
	int8_t   relOffset8 = 0x00;
	int16_t relOffset16 = 0;

	uint8_t block_hiX = 0x00;
	uint8_t block_hiY = 0x00;
	uint32_t block_sourceX = 0;
	uint32_t block_destY = 0;

	void ABSa();	void ABSIndInx();	void ABSXIndX();	void ABSXIndY();	void ABSInd();	void ABSLIndX();
	void ABSL();	void ACC();		    void BlockXYC();	void DirInxInd();	void DirIndX(); void DirIndY();
	void DirIndInx(); void DirIndLInx(); void DirIndL();	void DirInd();		void Direct();	void Imm();
	void Imp();		void PCRL();		void PCR();			void Stack();		void StackR();	void SRIndInx();

	// USEFUL INSTRUCTION VARIABLES
	uint8_t data = 0x00;
};