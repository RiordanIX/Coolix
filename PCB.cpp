#include "stdafx.h"
#include "PCB.h"


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
