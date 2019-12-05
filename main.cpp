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
            if constexpr(std::is_same<Type, int64_t>::value || std::is_same<Type, int32_t>::value || std::is_same<Type, int16_t>::value || std::is_same<Type, int>::value)
            {
                input_list.push_back(std::stoi(input_token));
            }
            else
            {
                input_list.push_back(input_token);
            }
        }

        reader.close();
    }

    return input_list;
}

/////////////////////////////////////////////////
/// \brief Struct representing instructions for
/// an Intcode program
///
/////////////////////////////////////////////////
struct Sentence
{
    /////////////////////////////////////////////////
    int64_t noun = 0;
    int64_t verb = 0;
};

/////////////////////////////////////////////////
/// \brief Struct representing a vector or a point
/// by its coordinates in a 2D dimension
///
/////////////////////////////////////////////////
template <typename Type>
struct Vector2
{
    /////////////////////////////////////////////////
    Type x;
    Type y;

    /////////////////////////////////////////////////
    Type standard()
    {
        return std::abs(x) + std::abs(y);
    }

    /////////////////////////////////////////////////
    bool operator==(const Vector2& another_vector)
    {
        return (x == another_vector.x && y == another_vector.y);
    }
};

/////////////////////////////////////////////////
/// \brief Struct representing a vector or a point
/// by its coordinates in a 3D dimension
///
/////////////////////////////////////////////////
template <typename Type>
struct Vector3
{
    /////////////////////////////////////////////////
    Type x;
    Type y;
    Type z;
};

/////////////////////////////////////////////////
// Challenges
/////////////////////////////////////////////////
namespace fuel
{
/////////////////////////////////////////////////
int64_t calculator(int64_t mass)
{
    return std::floor(mass / 3) - 2;
}

/////////////////////////////////////////////////
int64_t requirement(std::vector<int64_t> inputs)
{
    if(inputs.empty())
    {
        return 0;
    }

    int64_t fuel = 0;

    for(const auto& mass: inputs)
    {
        fuel += calculator(mass);
    }

    return fuel;
}

/////////////////////////////////////////////////
int64_t total_requirement(std::vector<int64_t> inputs)
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
            remaining_mass = calculator(remaining_mass);

            if(remaining_mass > 0)
            {
                fuel += remaining_mass;
            }
        }
    }

    return fuel;
}

} // namespace fuel

/////////////////////////////////////////////////
namespace intcode
{
/////////////////////////////////////////////////
std::vector<int64_t> program_translater(std::vector<int64_t> inputs, Sentence instructions = {})
{
    int64_t read_position = 0;
    int64_t diagnostic_code = 0;

    bool done = false;
    while(!done)
    {
        std::string instruction = std::to_string(inputs[read_position]);

        int64_t opcode = 0;

        struct Modes
        {
            int64_t first = 0;
            int64_t second = 0;
            int64_t third = 0;
        } modes;

        if(instruction.size() <= 2)
        {
            opcode = std::stoi(instruction);
        }
        else
        {
            opcode = std::stoi(instruction.substr(instruction.size() - 2, 2));

            if(instruction.size() >= 3)
            {
                modes.first = instruction[instruction.size() - 3] - '0';
            }

            if(instruction.size() >= 4)
            {
                modes.second = instruction[instruction.size() - 4] - '0';
            }

            if(instruction.size() >= 5)
            {
                modes.third = instruction[instruction.size() - 5] - '0';
            }
        }

        int64_t incrementer = 0;

        switch(opcode)
        {
        case 1:
        {
            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(modes.second == 0)
            {
                values.y = inputs[inputs[read_position + 2]];
            }
            else
            {
                values.y = inputs[read_position + 2];
            }

            inputs[inputs[read_position + 3]] = values.x + values.y;
            incrementer = 4;
            break;
        }

        case 2:
        {
            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(modes.second == 0)
            {
                values.y = inputs[inputs[read_position + 2]];
            }
            else
            {
                values.y = inputs[read_position + 2];
            }

            inputs[inputs[read_position + 3]] = values.x * values.y;
            incrementer = 4;
            break;
        }

        case 3:
        {
            inputs[inputs[read_position + 1]] = instructions.noun;

            incrementer = 2;

            break;
        }

        case 4:
        {
            int64_t read_address = 0;

            if(modes.first == 0)
            {
                read_address = inputs[read_position + 1];
            }
            else
            {
                read_address = read_position + 1;
            }

            diagnostic_code = inputs[read_address];

            incrementer = 2;
            break;
        }

        case 5:
        {
            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(values.x != 0)
            {
                if(modes.second == 0)
                {
                    values.y = inputs[inputs[read_position + 2]];
                }
                else
                {
                    values.y = inputs[read_position + 2];
                }

                read_position = values.y;
            }
            else
            {
                incrementer = 3;
            }

            break;
        }

        case 6:
        {

            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(values.x == 0)
            {
                if(modes.second == 0)
                {
                    values.y = inputs[inputs[read_position + 2]];
                }
                else
                {
                    values.y = inputs[read_position + 2];
                }

                read_position = values.y;
            }
            else
            {
                incrementer = 3;
            }

            break;
        }

        case 7:
        {
            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(modes.second == 0)
            {
                values.y = inputs[inputs[read_position + 2]];
            }
            else
            {
                values.y = inputs[read_position + 2];
            }

            if(values.x < values.y)
            {
                inputs[inputs[read_position + 3]] = 1;
            }
            else
            {
                inputs[inputs[read_position + 3]] = 0;
            }

            incrementer = 4;
            break;
        }

        case 8:
        {
            Vector2<int64_t> values;

            if(modes.first == 0)
            {
                values.x = inputs[inputs[read_position + 1]];
            }
            else
            {
                values.x = inputs[read_position + 1];
            }

            if(modes.second == 0)
            {
                values.y = inputs[inputs[read_position + 2]];
            }
            else
            {
                values.y = inputs[read_position + 2];
            }

            if(values.x == values.y)
            {
                inputs[inputs[read_position + 3]] = 1;
            }
            else
            {
                inputs[inputs[read_position + 3]] = 0;
            }

            incrementer = 4;
            break;
        }

        default:
            done = true;
            break;
        }

        read_position += incrementer;
    }

    inputs.push_back(diagnostic_code);

    return std::move(inputs);
}

/////////////////////////////////////////////////
Vector2<int64_t> program_caller(std::vector<int64_t> inputs, Sentence instructions)
{
    //Initialization
    if(inputs[0] != 3)
    {
        inputs[1] = instructions.noun;
        inputs[2] = instructions.verb;
    }

    inputs = program_translater(inputs, instructions);

    return {inputs.front(), inputs.back()};
}

/////////////////////////////////////////////////
int64_t instruction_solver(std::vector<int64_t> inputs, int64_t code)
{
    Sentence instructions;

    for(instructions.noun = 0; instructions.noun <= 99; ++instructions.noun)
    {
        for(instructions.verb = 0; instructions.verb <= 99; ++instructions.verb)
        {
            if(program_caller(inputs, instructions).x == code)
            {
                return 100 * instructions.noun + instructions.verb;
            }
        }
    }

    return 0;
}

} // namespace intcode

/////////////////////////////////////////////////
namespace wire_line
{
/////////////////////////////////////////////////
std::vector<Vector2<int64_t>> definer(std::vector<std::string> inputs)
{
    std::vector<Vector2<int64_t>> points;

    Vector2<int64_t> reading_position = {0, 0};

    for(auto next: inputs)
    {
        char direction = next[0];
        int64_t max_step = std::stoi(next.erase(0, 1));

        for(int64_t step = 0; step < max_step; ++step)
        {
            switch(direction)
            {
            case 'L':
                reading_position.x -= 1;
                break;
            case 'U':
                reading_position.y -= 1;
                break;
            case 'R':
                reading_position.x += 1;
                break;
            case 'D':
                reading_position.y += 1;
                break;
            default:
                break;
            }

            points.push_back(reading_position);
        }
    }

    return points;
}

/////////////////////////////////////////////////
std::vector<Vector2<int64_t>> intersections(std::vector<Vector2<int64_t>>& first_wire, std::vector<Vector2<int64_t>>& second_wire)
{
    std::vector<Vector2<int64_t>> intersection_points;

    for(auto first_wire_point: first_wire)
    {
        for(auto second_wire_point: second_wire)
        {
            if(first_wire_point == second_wire_point)
            {
                intersection_points.push_back(first_wire_point);
            }
        }
    }

    return intersection_points;
}

/////////////////////////////////////////////////
Vector2<int64_t> closest_intersection(std::array<std::vector<std::string>, 2> wires)
{
    Vector2<int64_t> closest_wire = {0, 0};

    auto&& first_wire = definer(wires[0]);
    auto&& second_wire = definer(wires[1]);

    auto&& intersection_points = intersections(first_wire, second_wire);

    for(auto intersection: intersection_points)
    {
        if((closest_wire.x == 0 && closest_wire.y == 0) || (std::abs(intersection.x) + std::abs(intersection.y) < closest_wire.standard()))
        {
            closest_wire = {std::abs(intersection.x), std::abs(intersection.y)};
        }
    }

    return closest_wire;
}

/////////////////////////////////////////////////
Vector2<int64_t> fewest_combined_steps(std::array<std::vector<std::string>, 2> wires)
{
    auto&& first_wire = definer(wires[0]);
    auto&& second_wire = definer(wires[1]);

    auto&& intersection_points = intersections(first_wire, second_wire);

    Vector2<int64_t> fewest = {int64_t(first_wire.size()), int64_t(second_wire.size())};

    for(auto intersection: intersection_points)
    {
        Vector2<int64_t> step = {std::distance(first_wire.begin(), std::find(first_wire.begin(), first_wire.end(), intersection)) + 1, std::distance(second_wire.begin(), std::find(second_wire.begin(), second_wire.end(), intersection)) + 1};

        if(step.standard() < fewest.standard())
        {
            fewest = step;
        }
    }

    return fewest;
}

/////////////////////////////////////////////////
std::array<std::vector<std::string>, 2> input_list(std::filesystem::path path)
{
    std::vector<std::string> wires = get_input_list<std::string>(path);

    std::array<std::vector<std::string>, 2> wire_points;

    for(size_t wire = 0; wire < wires.size(); wire++)
    {
        std::stringstream wire_map(wires[wire]);

        std::string token;
        while(std::getline(wire_map, token, ','))
        {
            wire_points[wire].push_back(token);
        }
    }

    return wire_points;
}

} // namespace wire_line

/////////////////////////////////////////////////
namespace password
{
/////////////////////////////////////////////////
int64_t criteria_count(Vector2<int64_t> range)
{
    int64_t minimal_number = range.x;
    int64_t current_number = minimal_number;

    int64_t count = 0;

    while(current_number < range.y)
    {
        bool same_digits = false;

        auto digits = std::to_string(current_number);
        for(size_t character = 0; character < digits.size() - 1; ++character)
        {
            if(digits[character] - '0' > digits[character + 1] - '0')
            {
                digits[character + 1] = digits[character];
            }

            if(digits[character] == digits[character + 1])
            {
                same_digits = true;
            }
        }

        current_number = std::stoi(digits);

        if(current_number <= range.y && same_digits)
        {
            count++;
        }

        current_number++;
    }

    return count;
}

/////////////////////////////////////////////////
int64_t group_criteria_count(Vector2<int64_t> range)
{
    int64_t minimal_number = range.x;
    int64_t current_number = minimal_number;

    int64_t count = 0;

    while(current_number < range.y)
    {
        bool same_digits = false;

        auto digits = std::to_string(current_number);
        for(size_t character = 0; character < digits.size() - 1; ++character)
        {
            if(digits[character] - '0' > digits[character + 1] - '0')
            {
                digits[character + 1] = digits[character];
            }
        }

        for(size_t character = 0; character < digits.size() - 1; ++character)
        {
            if(digits[character] == digits[character + 1])
            {
                same_digits = true;

                if(character > 0 && digits[character - 1] == digits[character])
                {
                    same_digits = false;
                }

                if(character < digits.size() - 1 && digits[character + 2] == digits[character])
                {
                    same_digits = false;
                }

                if(same_digits)
                {
                    break;
                }
            }
        }

        current_number = std::stoi(digits);

        if(current_number <= range.y && same_digits)
        {
            count++;
        }

        current_number++;
    }

    return count;
}

} // namespace password

/////////////////////////////////////////////////
// Main stream
/////////////////////////////////////////////////
int main()
{
    std::vector<std::variant<std::string, int64_t>> answers;

    answers.push_back(fuel::requirement(get_input_list<int64_t>("inputs/01-mass_input.txt")));
    answers.push_back(fuel::total_requirement(get_input_list<int64_t>("inputs/01-mass_input.txt")));
    answers.push_back(intcode::program_caller(get_input_list<int64_t>("inputs/02-program_integers.txt"), {12, 2}).x);
    answers.push_back(intcode::instruction_solver(get_input_list<int64_t>("inputs/02-program_integers.txt"), 19690720));
    answers.push_back("too long"/*wire_line::closest_intersection(wire_line::input_list("inputs/03-wire-maps.txt")).standard()*/);
    answers.push_back("too long"/*wire_line::fewest_combined_steps(wire_line::input_list("inputs/03-wire-maps.txt")).standard()*/);
    answers.push_back(password::criteria_count({372304, 847060}));
    answers.push_back(password::group_criteria_count({372304, 847060}));
    answers.push_back(intcode::program_caller(get_input_list<int64_t>("inputs/05-program_integers.txt"), {1, 0}).y);
    answers.push_back(intcode::program_caller(get_input_list<int64_t>("inputs/05-program_integers.txt"), {5, 0}).y);

    std::ofstream writer("output.txt");
    for(uint16_t part = 0; part < answers.size(); ++part)
    {
        std::visit([&part, &writer](auto&& answer)
        {
            std::stringstream output;
            output << "Part " << std::floor(part / 2) + 1 << '.' << part % 2 << ": " << answer;

            std::cout << output.str() << std::endl;

            if(writer)
            {
                writer << output.str() << '\n';
            }
        }, answers[part]);
    }

    if(writer)
    {
        writer.close();
    }

    return 0;
}
