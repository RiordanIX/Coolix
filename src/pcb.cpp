#include "stdafx.h"
#include "pcb.hpp"


PCB::PCB()
{
	P = 0;
}
PCB::~PCB()
{
}
void PCB::SetPriority(int p)
{
	P = p;
}
