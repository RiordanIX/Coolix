/*
loader.cpp

Author: Oliver Prestridge 9/18/2017

Reads from a program text file and loads the information and data of each process into memory.
*/

#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include "loader.h"
#include "instruct.hpp"

using namespace std;

void loader::readFromFile(string filename)
{
    string x;
    
    file.open(filename.c_str());

	//Properties of a loaded process
    int pid, instructs, priority, inpBuffer, outBuffer, tmpBuffer;
    
    if(!file)
        cout << "Unable to read file." << endl;
     
	//Read each line (for every space in the file, a newline is made)
    while(file >> x)
    {
		//The JOB tag indicates the start of a new process, the three integers following are the process id, number of instructions, and priority respectively.
        if(x.compare(0, 3, "JOB") == 0)
        {
            cout << "Creating process..." << endl;

			//Read process id
            file >> x;
            pid = stoi(x, NULL, 16);

			//Read number of instructions
			file >> x;
			instructs = stoi(x, NULL, 16);
			
			//Read process' priority
			file >> x;
			priority = stoi(x, NULL, 16);

			cout << "Process " << pid << " created." << endl
				 << "Loading instructions..." << endl;

			//"Load" the process' instructions to memory
			for(int i = 0; i < instructs; i++)
			{
				file >> x;
				instruct_t instruction = stoll(x, NULL, 16);

				//Load to disk here
				cout << instruction << endl;
			}

			cout << "Instructions loaded for process " << pid << endl;
        }

		//The data tag indicates the start of the process' given data. The following three numbers are the input, output, and temporary buffer sizes respectively.
		else if(x.compare(0, 4, "Data") == 0)
		{
			cout << "Loading data..." << endl;
			
			//Input buffer size
			file >> x;
			int inpBuffer = stoi(x, NULL, 16);

			//Output buffer size
			file >> x;
			int outBuffer = stoi(x, NULL, 16);

			//Temporary buffer size
			file >> x;
			int tmpBuffer = stoi(x, NULL, 16);

			//Load all of the given data to memory
			for(int i = 0; i < inpBuffer + outBuffer + tmpBuffer; i++)
			{
				file >> x;
				instruct_t data = stoll(x, NULL, 16);

				//Load to disk here
				cout << data << endl;
			}

			cout << "Data loaded for process " << pid << endl;

			//process p(pid, instructs, priority, ...
		}
    }
    
    file.close();
}	
