#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "instruct.hpp"
#include "cpu_operations.hpp"

using std::vector;
using std::size_t;
// Default Register size is 16.  May change if requirements change.
#define DEFAULT_REG_SIZE 16


/** cpu
 * A simulated CPU class. Simulates some aspects of a normal CPU.  The user
 * can define the number of registers by passing it in, or just leaving it as
 * the default of 16.
 */
class cpu {
public:
	// Public because the dispatcher needs to access it.
	size_t num_registers;
	vector<instruct_t> registers;

	cpu(size_t size=DEFAULT_REG_SIZE) : num_registers(size), registers(size, 0) {}
	void decode_and_execute(instruct_t inst);
	std::string get_info();

};

