#pragma once

#include <vector>

class Cache {
public:
	Cache(std::size_t size) : data(size), current_pid(-1), nextFrame(0) {
		for (auto it = data.begin(); it != data.end(); it++) {
			(*it).first = -1;
			(*it).second.resize(4);
		}
	}

	// first should be the frame number
	std::vector<std::pair<std::size_t, std::vector<instruct_t>>> data;
	unsigned int current_pid;
	int nextFrame;
	void next_frame() {
		if (nextFrame == 3) nextFrame = 0;
		else nextFrame++;
	}

	void set_cache(std::size_t frame, std::vector<instruct_t> insts) {
		data[nextFrame].first = frame;
		data[nextFrame].second = insts;
		next_frame();
	}

	instruct_t get_instruction(std::size_t frame, std::size_t offset) {
		for (auto it = data.begin(); it != data.end(); it++) {
			if (frame == (*it).first){
				return (*it).second[(offset)/(4)];
			}
		}
		return -1;
	}

	bool in_cache(unsigned int pid, std::size_t frame) {
		if (pid != current_pid) return false;
		else {
			for (auto it = data.begin(); it != data.end(); it++) {
				if ((*it).first == frame)
				{
					if ((*it).second[0] == 0)
					{
						return false;
					}
					else
					{
						return true;
					}
				}
			}
			return false;
		}
	}
};

