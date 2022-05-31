#include <bio/alphabet/structure/dot_bracket3.hpp>

int main()
{
    using bio::operator""_db3;

    // Using the char literal to assign a single dot bracket:
    bio::dot_bracket3 my_letter{'('_db3};
    
    my_letter.assign_char(')');             // <- assigns the char explicitly
}
