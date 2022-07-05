#include <bio/alphabet/aminoacid/concept.hpp>

namespace your_namespace
{

// your own aminoacid definition
struct your_aa : bio::alphabet::aminoacid_empty_base
{
    //...
};

}

static_assert(bio::alphabet::enable_aminoacid<your_namespace::your_aa> == true);

/***** OR *****/

namespace your_namespace2
{

// your own aminoacid definition
struct your_aa
{
    //...
};

constexpr bool enable_aminoacid(your_aa) noexcept
{
    return true;
}

}

static_assert(bio::alphabet::enable_aminoacid<your_namespace2::your_aa> == true);
