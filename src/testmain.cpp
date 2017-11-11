#include "cpu.hpp"
#include "ram.hpp"
#include "disk.hpp"
#include "loader.hpp"
#include "pcb.hpp"
#include "debug.hpp"
#include "PriorityQueue.hpp"

#include <iostream>


extern Ram MEM;
extern Disk DISK;
extern std::vector<PCB> process_list;
extern PriorityQueue readyQueue;
extern PriorityQueue newQueue;

using std::size_t;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	printf("%d, %s\n", argc, argv[0]);
	cpu CPU;
	loader ldr;
	ldr.readFromFile("test_job2");


	cout << "MEMSIZE: " << DISK.get_used() << endl;
	auto chunk = DISK.read_instruction_chunk(0, DISK.get_used());


	for (auto it = chunk.begin(); it != chunk.end(); ++it) {
		std::cout << "Instruction: " << *it << '\n';
	}

	MEM.allocate_chunk(0, DISK.read_instruction_chunk(0, DISK.get_used()));

	// Queue Management and status readying
	newQueue.getProcess()->set_status(status::READY);
	readyQueue.addProcess(newQueue.getProcess());
	newQueue.removeProcess();
	readyQueue.getProcess()->set_ram_address(0);

	// Run Process
	while (readyQueue.getProcess()->get_status() != status::TERMINATED) {
		instruct_t inst = CPU.fetch(readyQueue.getProcess());
		printf("%u at %lu\n", inst, readyQueue.getProcess()->get_program_counter());
		CPU.decode_and_execute(inst, readyQueue.getProcess());
		readyQueue.getProcess()->increment_PC();
	}
	MEM.dump_data();
}

