#include "pcb.hpp"


void PCB::SetPriority(int p)
{
	priority = p;
}

instruct_t PCB::get_inp_address()
{
	return ramAddress + buffSizes[buffType::INSTRUCTION];	
}

instruct_t PCB::get_out_address()
{
	return ramAddress + buffSizes[buffType::INSTRUCTION] + buffSizes[buffType::INPUT];
}

instruct_t PCB::get_temp_address()
{
	return ramAddress + buffSizes[buffType::INSTRUCTION] + buffSizes[buffType::INPUT] + buffSizes[buffType::OUTPUT];
}