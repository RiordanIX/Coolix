#include <cstdlib>
#include <fstream>
#include <string>

class loader
{
private:
    std::ifstream file;
    
public:
    void readFromFile(std::string filename);
};