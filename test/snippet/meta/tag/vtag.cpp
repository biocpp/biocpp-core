#include <bio/meta/tag/vtag.hpp>

//![dispatch]
void dispatchee(bio::meta::vtag_t<1>) { /* do one thing     */ }

void dispatchee(bio::meta::vtag_t<2>) { /* do another thing */ }

void dispatcher()
{
    dispatchee(bio::meta::vtag<1>); // calls first overload
    dispatchee(bio::meta::vtag<2>); // calls second overload
}
//![dispatch]


//![string_literal]
void foo(auto tag)
{
    using namespace bio::meta::literals;

    // selection of path AT COMPILE TIME:
    if constexpr (tag == "onething"_vtag)
    {
        // something
    }
    else
    {
        // something else
    }
}

void bar()
{
    using namespace bio::meta::literals;

    foo("onething"_vtag);   // triggers first path
    foo("other"_vtag);      // triggers second path
}
//![string_literal]

//![int_literal]
void dispatchee2(bio::meta::vtag_t<int64_t{1}>) { /* do one thing     */ }

void dispatchee2(bio::meta::vtag_t<int64_t{2}>) { /* do another thing */ }

void dispatcher2()
{
    using namespace bio::meta::literals;

    dispatchee2(1_vtag); // calls first overload
    dispatchee2(2_vtag); // calls second overload
}
//![int_literal]

void ignoreme()
{
//![negative_int_literal]
using namespace bio::meta::literals;

static_assert(1_vtag == bio::meta::vtag<1>);
static_assert(-1_vtag == bio::meta::vtag<-1>);
//![negative_int_literal]
}
