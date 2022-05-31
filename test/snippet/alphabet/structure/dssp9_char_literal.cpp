#include <bio/alphabet/structure/dssp9.hpp>

int main()
{
    using bio::operator""_dssp9;

    // Using the char literal to assign a single DSSP annotation:
    bio::dssp9 my_letter{'I'_dssp9};
    
    my_letter.assign_char('G');      // <- assigns the char explicitly
}
