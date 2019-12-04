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
int64_t fuel_calculator(int64_t mass)
{
    return std::floor(mass / 3) - 2;
}

/////////////////////////////////////////////////
int64_t fuel_requirement(std::vector<int64_t> inputs)
{
    if(inputs.empty())
    {
        return 0;
    }

    int64_t fuel = 0;

    for(const auto& mass: inputs)
    {
        fuel += fuel_calculator(mass);
    }

    return fuel;
}

/////////////////////////////////////////////////
int64_t total_fuel_requirement(std::vector<int64_t> inputs)
{
    if(inputs.empty())
    {
        return 0;
    }

    int64_t fuel = 0;

    for(const auto& mass: inputs)
    {
        auto remaining_mass = mass;

        while(remaining_mass > 0)
        {
            remaining_mass = fuel_calculator(remaining_mass);

            if(remaining_mass > 0)
            {
                fuel += remaining_mass;
            }
        }
    }

    return fuel;
}

/////////////////////////////////////////////////
// Main stream
/////////////////////////////////////////////////
int main()
{
    std::vector<std::variant<std::string, int64_t>> answers;

    answers.push_back(fuel_requirement(get_input_list<int64_t>("inputs/01-mass_input.txt")));
    answers.push_back(total_fuel_requirement(get_input_list<int64_t>("inputs/01-mass_input.txt")));

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
                writer << output.str() << '\n';
            }
        }, answers[day]);
    }

    if(writer)
    {
        writer.close();
    }

    return 0;
}
