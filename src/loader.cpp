/*
loader.cpp

Author: Oliver Prestridge 9/18/2017

Reads from a program text file and loads the information and data of each
process into memory.
*/

#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include "loader.hpp"
#include "instruct.hpp"
#include "PriorityQueue.h"

using std::size_t;
using std::string;
using std::stoi;
// instruct_t is a uint32_t, which is 32 bits.  stoul goes to unsigned long,
// which should also be 32 bits.
using std::stoul;

extern Disk DISK;
extern std::vector<PCB> process_list;

void loader::readFromFile(string filename) {
    string x;

    file.open(filename.c_str());

	//Properties of a loaded process
    int pid = -1;
	size_t numInstructs=0, address=0xDEADBEEF, priority=0, inpBuffer, outBuffer, tmpBuffer;

    if(!file) {
        printf("Unable to read file.\n");
	}

	//Read each line (for every space in the file, a newline is made)
    while(file >> x) {
		//The JOB tag indicates the start of a new process, the three integers
		//following are the process id, number of instructions, and priority
		//respectively.
        if(x.compare(0, 3, "JOB") == 0) {
            printf("Creating process...\n");

			address = DISK.get_used();

			//Read process id
            file >> x;
            pid = stoul(x, NULL, 16);

			//Read number of instructions
			file >> x;
			numInstructs = stoul(x, NULL, 16);

			//Read process' priority
			file >> x;
			priority = stoul(x, NULL, 16);

			debug_printf("Process %d created.\n",pid);
			debug_printf("Priority: %lu\n", priority);
			debug_printf("Loading instructions...%s","\n");

			//"Load" the process' instructions to disk
			for(unsigned int i = 0; i < numInstructs; i++) {
				file >> x;
				instruct_t instruction = stoul(x, NULL, 16);

				//Load to disk here
				//cout << instruction << endl;
				DISK.allocate(instruction);
			}

			debug_printf("Instructions loaded for process %d\n", pid);
        }

		//The data tag indicates the start of the process' given data. The
		//following three numbers are the input, output, and temporary buffer
		//sizes respectively.
		else if(x.compare(0, 4, "Data") == 0) {
			debug_printf("Loading data...%s","\n");

			//Input buffer size
			file >> x;
			inpBuffer = stoi(x, NULL, 16);

			//Output buffer size
			file >> x;
			outBuffer = stoi(x, NULL, 16);

			//Temporary buffer size
			file >> x;
			tmpBuffer = stoi(x, NULL, 16);

			//Load all of the given data to memory
			for(unsigned int i = 0; i < inpBuffer + outBuffer + tmpBuffer; i++) {
				file >> x;
				instruct_t data = stoul(x, NULL, 16);

				//Load to disk here
				//cout << data << endl;
				DISK.allocate(data);
			}


			//process p(pid, instructs, priority, ...
			if(pid != -1)
			{
				// Multiply the instructs by 4 to give bytes instead of words
				PCB p(pid, address, numInstructs*4, inpBuffer*4, outBuffer*4, tmpBuffer*4, priority);
				process_list.push_back(p);
			}
		}
		// pid may not be initialized at this point
		// TODO ensure pid is initialized

		//cout << "Data loaded for process " << pid << endl;
    }

    file.close();
}

