#pragma once

#include <vector>

class Cache {
public:
	// first should be the frame number
	std::vector<std::pair<std::size_t, instruct_t>> data;
	unsigned int current_pid;
	std::size_t size() { return data.size();}

	// \param size Takes the number of instructions in the cache.
	Cache(std::size_t size) : data(size), current_pid(-1) {
		for (auto it = data.begin(); it != data.end(); it++) {
			(*it).first = -1;
			(*it).second = -1;
		}
	}

	void set_cache(std::size_t address, instruct_t inst, std::size_t index) {
			data[index].first = address;
			data[index].second = inst;
	}

	instruct_t get_instruction(std::size_t instructionNum) {
		for (auto it = data.begin(); it != data.end(); it++) {
			if (instructionNum == (*it).first){
				return (*it).second;
			}
		}
		return -1;
	}

	bool in_cache(unsigned int pid, std::size_t instructionNum) {
		if (pid != current_pid) return false;
		else {
			for (auto it = data.begin(); it != data.end(); it++) {
				if ((*it).first == instructionNum)
				{
					return true;
				}
			}
			return false;
		}
	}
};

