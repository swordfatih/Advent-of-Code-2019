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
    std::vector<Type> input_list;

    if(std::filesystem::exists(path))
    {
        return input_list;
    }

    std::ifstream reader(path);

    if(reader)
    {
        std::string input_token;
        while(reader >> input_token)
        {
            input_list.push_back(std::stoi(input_token));
        }

        reader.close();
    }
}


/////////////////////////////////////////////////
// Challenges
/////////////////////////////////////////////////
uint32_t mass_calculator(std::vector<uint32_t> inputs)
{
    if(inputs.empty())
    {
        return 0;
    }

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
