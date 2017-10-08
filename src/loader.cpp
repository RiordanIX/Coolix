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

using std::string;
using std::stoi;
// instruct_t is a uint32_t, which is 32 bits.  stoul goes to unsigned long,
// which should also be 32 bits.
using std::stoul;
using std::cout;
using std::endl;


void loader::readFromFile(string filename, Disk& disk) {
    string x;

    file.open(filename.c_str());

	//Properties of a loaded process
    int pid = -1, numInstructs, address priority, inpBuffer, outBuffer, tmpBuffer;

    if(!file) {
        cout << "Unable to read file." << endl;
	}

	//Read each line (for every space in the file, a newline is made)
    while(file >> x) {
		//The JOB tag indicates the start of a new process, the three integers
		//following are the process id, number of instructions, and priority
		//respectively.
        if(x.compare(0, 3, "JOB") == 0) {
            cout << "Creating process..." << endl;

			address = disk.get_used();
				
			//Read process id
            file >> x;
            pid = stoi(x, NULL, 16);

			//Read number of instructions
			file >> x;
			numInstructs = stoi(x, NULL, 16);

			//Read process' priority
			file >> x;
			priority = stoi(x, NULL, 16);

			cout << "Process " << pid << " created." << endl
				 << "Priority: " << priority << endl
				 << "Loading instructions..." << endl;

			//"Load" the process' instructions to memory
			for(int i = 0; i < instructs; i++) {
				file >> x;
				instruct_t instruction = stoul(x, NULL, 16);

				//Load to disk here
				//cout << instruction << endl;
				disk.allocate(instruction);
			}

			cout << "Instructions loaded for process " << pid << endl;
        }

		//The data tag indicates the start of the process' given data. The
		//following three numbers are the input, output, and temporary buffer
		//sizes respectively.
		else if(x.compare(0, 4, "Data") == 0) {
			cout << "Loading data..." << endl;

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
			for(int i = 0; i < inpBuffer + outBuffer + tmpBuffer; i++) {
				file >> x;
				instruct_t data = stoul(x, NULL, 16);

				//Load to disk here
				//cout << data << endl;
				disk.allocate(data);
			}


			//process p(pid, instructs, priority, ...
			if(pid != -1)
			{
				process p(pid, address, numInstructs, inpBuffer, outBuffer, tempBuffer, priority);
				//put into ready queue
			}
		}
		// pid may not be initialized at this point
		// TODO ensure pid is initialized

		//cout << "Data loaded for process " << pid << endl;
    }

    file.close();
}

