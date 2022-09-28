#include <string>
#include <fmt/core.h>
#include <bio/meta/overloaded.hpp>

int main()
{
    bio::meta::overloaded fn{
      [] (std::string const & s)      { fmt::print("I am a string:{}\n", s); },
      [] (std::integral auto const i) { fmt::print("I am an int:  {}\n", i); }
    };

    fn("foobar"); // picks first "overload"
    fn(3);        // picks second "overload"
}
