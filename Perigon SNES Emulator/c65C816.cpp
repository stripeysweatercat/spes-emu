#include "c65C816.h"

c65C816::c65C816()
{
}

void c65C816::ConnectMem(Memory* m)
{
	this->mem = m;
}

int c65C816::clock()
{
	opcode = mem->read(pc);
	pc++;
	TCU++;
	switch (opcode)
	{
	case 0x61:	// ADC (dir,X)
	{
		if (GetFlag(M))
		{

		}
		else
		{

		}
	}
	}
}

void c65C816::SetFlag(Flags f, bool v)
{
	if (v)
	{
		p |= f;
	}
	else
	{
		p &= ~f;
	}
}

bool c65C816::GetFlag(Flags f)
{
	return p & f;
}

void c65C816::ABSa()		// a
{
	addr_lo = mem->read(pc);
	pc++;
	TCU++;
	addr_hi = mem->read(pc);
	pc++;
	TCU++;
	addr_abs = (DBR << 16) | (addr_hi << 8) | addr_lo;
}

void c65C816::ABSIndInx()	// (a,x)
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_ind = (addr_hi << 8) | addr_lo;
	addr_ind += X;
	pc = mem->read(addr_ind);
}

void c65C816::ABSXIndX()	// a,x
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_ind = (addr_hi << 8) | addr_lo;
	addr_ind += X;
	addr_abs = (DBR << 16) | addr_ind;
}

void c65C816::ABSXIndY()	// a,y
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_ind = (addr_hi << 8) | addr_lo;
	addr_ind += Y;
	addr_abs = (DBR << 16) | addr_ind;
}

void c65C816::ABSInd()		// (a)
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_abs = (addr_hi << 8) | addr_lo;
	addr_lo = mem->read(addr_abs);
	addr_hi = mem->read(addr_abs + 1);
	pc = (addr_hi << 8) | addr_lo;
	/* ONLY FOR WHEN JML INSTRUCTION IMPLEMENTED. SWAP CURRENT OPCODE FOR CORRECT ONE
	if (opcode == 0x00)	
	{
		PBR = mem->read(addr_abs + 2);
	}
	*/
}

void c65C816::ABSLIndX()	// al,x
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_b = mem->read(pc);
	pc++;
	addr_abs = (addr_b << 16) | (addr_hi << 8) | addr_lo;
	addr_abs += X;
}

void c65C816::ABSL()		// al
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_b = mem->read(pc);
	pc++;
	addr_abs = (addr_b << 16) | (addr_hi << 8) | addr_lo;
}

void c65C816::ACC()			// A
{
	data = a;
}

void c65C816::BlockXYC()	// xyc
{
	block_hiY = mem->read(pc);
	pc++;
	block_destY = ((uint32_t)block_hiY << 16) | Y;
	DBR = block_hiY;
	block_hiX = mem->read(pc);
	pc++;
	block_sourceX = ((uint32_t)block_hiX << 16) | X;
	uint16_t bytesToMove = a + 1;
	for (int i = 0; i < bytesToMove; i++)
		mem->ram[block_destY + i] = mem->ram[block_sourceX];
	// Increment or decrement X and Y registers depending on instruction
}

void c65C816::DirInxInd()	// (d,x)
{
	if (emulation && ((direct & 0xFF) == 0))
	{
		addr_lo = mem->read(pc) + X;
		pc++;
		TCU++;
		addr_lo = mem->read((direct & 0xFF00) | addr_lo);
		addr_hi = mem->read((direct & 0xFF00) | (addr_lo + 1));
		pc += 2;
		TCU += 2;
	}
	// INCOMPLETE
}

void c65C816::DirIndX()		// d,x
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_ind += X;
	addr_abs = addr_ind;
}

void c65C816::DirIndY()		// d,y
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_ind += Y;
	addr_abs = addr_ind;
}

void c65C816::DirIndInx()	// (d),y
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_lo = mem->read(addr_ind);
	addr_hi = mem->read(addr_ind + 1);
	addr_abs = ((uint32_t)DBR << 16) | ((uint16_t)addr_hi << 8) | addr_lo;
	addr_abs += Y;
}

void c65C816::DirIndLInx()	// [d],y
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_abs = addr_ind;
	addr_abs += Y;
}

void c65C816::DirIndL()		// [d]
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_lo = mem->read(addr_ind);
	addr_hi = mem->read(addr_ind + 1);
	addr_abs = ((uint16_t)addr_hi << 8) | addr_lo;
}

void c65C816::DirInd()		// (d)
{
	indOffset = mem->read(pc);
	pc++;
	addr_ind = direct + indOffset;
	addr_lo = mem->read(addr_ind);
	addr_hi = mem->read(addr_ind + 1);
	addr_abs = ((uint32_t)DBR << 16) | ((uint16_t)addr_hi << 8) | addr_lo;
}

void c65C816::Direct()		// d
{
	if ((opcode != 0xD4) && (emulation == 1) && ((direct & 0xFF) == 0)) // not PEI
	{
		addr_lo = mem->read(pc);
		pc++;
		TCU++;
		addr_abs = (direct & 0xFF00) | addr_lo;
	}
	else
	{
		addr_lo = mem->read(pc);
		pc++;
		TCU++;
		addr_abs = direct + addr_lo;
	}
}

void c65C816::Imm()			// #
{
	// different behaviour in 8 vs 16 bit mode, i dont remember what that means so ill get back to it
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	addr_abs = ((uint16_t)addr_hi << 8) | addr_lo;
}

void c65C816::Imp()			// i
{
	// address is implied in the instruction
}

void c65C816::PCRL()		// rl
{
	addr_lo = mem->read(pc);
	pc++;
	addr_hi = mem->read(pc);
	pc++;
	relOffset16 = ((int16_t)addr_hi << 8) | (int8_t)addr_lo;
	addr_abs = pc + relOffset16;
	/*
	* if (opcode == BRL)
	* {
	*	pc = addr_abs;
	* }
	* else
	* {
	*	addr_abs -> stack;
	* }
	*/
}

void c65C816::PCR()			// r
{
	relOffset8 = mem->read(pc);
	addr_abs = pc + relOffset8;
}

void c65C816::Stack()		// s
{
	// used for any stack instructions such as Push, Pull, JSR, or Interrupts
	// bank is always 0, all interrupt vectors to be pulled from bank 0
}

void c65C816::StackR()		// d,s
{
	indOffset = mem->read(pc);
	pc++;
	addr_abs = sp + indOffset;
}

void c65C816::SRIndInx()	// (d,s),y
{
	indOffset = mem->read(pc);
	pc++;
	addr_abs = sp + indOffset;
	addr_abs = ((uint32_t)DBR << 16) | addr_abs;
	addr_abs += Y;
}