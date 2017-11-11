#include "cpu.hpp"
#include "ram.hpp"
#include "disk.hpp"
#include "loader.hpp"
#include "pcb.hpp"
#include "debug.hpp"

#include <iostream>

extern Ram MEM;
extern Disk DISK;
extern std::vector<PCB> process_list;

using std::size_t;
using std::cout;
using std::endl;
int main(int argc, char* argv[]) {
	printf("%d, %s\n", argc, argv[0]);
	cpu CPU;
	loader ldr;
	ldr.readFromFile("test_job2");

	PCB* pcb = &(process_list.front());

	cout << "MEMSIZE: " << DISK.get_used() << endl;
	for (size_t i = 0; i < DISK.get_used(); i+=4) {
		instruct_t inst = DISK.read_instruction(i);
		CPU.fetch(pcb);
}

