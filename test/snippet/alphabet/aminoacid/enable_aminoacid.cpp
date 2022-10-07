#include <bio/alphabet/aminoacid/concept.hpp>

namespace your_namespace
{

// your own aminoacid definition
struct your_aa : bio::alphabet::aminoacid_empty_base
{
    //...
};

} // namespace your_namespace

/***** OR *****/

namespace your_namespace2
{

// your own aminoacid definition
struct your_aa
{
    //...
};

} // namespace your_namespace2

template <>
inline constexpr bool bio::alphabet::custom::enable_aminoacid<your_namespace2::your_aa> = true;

