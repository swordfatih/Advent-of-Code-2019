/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <iostream>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <variant>

/////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////
template <typename Type>
std::vector<Type> get_input_list(std::filesystem::path path)
{
    std::vector<Type> input_list;

    if(!std::filesystem::exists(path))
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

    return input_list;
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

    for(const auto& mass: inputs)
    {
        result += std::floor(mass / 3) - 2;
    }

    return result;
}

/////////////////////////////////////////////////
// Main stream
/////////////////////////////////////////////////
int main()
{
    std::vector<std::variant<std::string, int64_t>> answers;

    answers.push_back(mass_calculator(get_input_list<uint32_t>("inputs/01-mass_input.txt")));

    std::ofstream writer("output.txt");
    for(uint16_t day = 0; day < answers.size(); ++day)
    {
        std::visit([&day, &writer](auto&& answer)
        {
            std::stringstream output;
            output << day + 1 << "/12: " << answer;

            std::cout << output.str() << std::endl;

            if(writer)
            {
                writer << output.str();
            }
        }, answers[day]);
    }

    if(writer)
    {
        writer.close();
    }

    return 0;
}
