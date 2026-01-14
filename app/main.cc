#include <iostream>
#include <magic_enum/magic_enum.hpp>

enum class Color {
    Red,
    Green,
    Blue
};

int main(int argc, char const *argv[])
{
    std::cout << magic_enum::enum_name(Color::Blue) << "\n";
    
    return 0;
}
