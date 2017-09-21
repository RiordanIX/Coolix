#pragma once
#include <vector>
#include "instruct.hpp"

using std::vector;
// Default Register size is 16.  May change if requirements change.
#define DEF_REG_SIZE 16


/** cpu
 * A simulated CPU class. Simulates some aspects of a normal CPU.  The user
 * can define the number of registers by passing it in, or just leaving it as
 * the default of 16.
 */
class cpu {
public:
	cpu(size_t size=DEF_REG_SIZE) : num_registers(size), registers(size, 0) {}
	int execute();

	// Public because the dispatcher needs to access it.
	vector<instruct_t> registers;
	size_t num_registers;
};


