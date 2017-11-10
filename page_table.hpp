#include <vector>
#include "ram.hpp"

#define FRAME_SIZE 128

struct PageDirectory
{
	std::vector<bool> frames;
	
	public PageDirectory(Ram ram) : frames(ram.get_size() / FRAME_SIZE, 0);
}

struct Page
{
	public bool valid;
}

class PageTable
{
	public:
		std::vector<unsigned int> frameLocations;
		
		PageTable(Ram ram, PageDirectory dir); 
		byte_t readPhysicalAddress(unsigned int page, insigned int offset);
		
	private:
		Ram _ram;
}