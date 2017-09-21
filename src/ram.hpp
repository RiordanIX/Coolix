#pragma once
#include <vector>
#include "instruct.h"

class Ram {
public:
	Ram();
	bool allocate(instruct_t data);

private:
	vector<instruct_t> _space_;
};

