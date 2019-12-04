/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <iostream>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <cmath>

/////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////
template <typename Type>
std::vector<Type> get_input_list(std::filesystem::path path)
{
    if(path.exists())
    {
        return {};
    }

    std::ifstream reader(path);
}


/////////////////////////////////////////////////
// Challenges
/////////////////////////////////////////////////
uint32_t mass_calculator(std::vector<uint32_t> inputs)
{
    uint32_t result = 0;

    for(auto mass: inputs)
    {
        result += std::floor(mass / 3) - 2;
    }
}



/////////////////////////////////////////////////
// Main stream
/////////////////////////////////////////////////
int main()
{
    std::cout << mass_calculator(get_input_list<uint32_t>("inputs.txt")) << std::endl;
    return 0;
}
