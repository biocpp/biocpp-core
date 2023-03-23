#include <bio/ranges/container/dictionary.hpp>

int main()
{
    bio::ranges::dictionary<std::string, std::string> employee_status;

    /* add some elements */
    employee_status.emplace_back("Bob", "hired");
    employee_status.emplace_back("Jane", "hired");
    employee_status.emplace_back("Marc", "fired");
    employee_status.emplace_back("Maria", "promoted");

    /* Access by position in dictionary; same as insert-order */
    auto && first = employee_status[0];         // a tuple of key and mapped_value
    fmt::print("{}\n", first);                  // prints '("Bob", "hired")'
    //get<0>(first) = "Rob";                    // key cannot be changed
    get<1>(first) = "tired";                    // mapped_value can be changed
    fmt::print("{}\n", employee_status[0]);     // prints '("Bob", "tired")'

    /* Access by key */
    auto && jane = employee_status["Jane"];     // the mapped_value belonging to Jane
    fmt::print("{}\n", jane);                   // prints 'hired'
    jane = "wired";                             // can be changed
    fmt::print("{}\n", employee_status["Jane"]);// prints 'wired'

    /* No implicit inserts like std::unordered_map */
    //employee_status["Shane"] = "hired";       // DOES NOT WORK.
}
