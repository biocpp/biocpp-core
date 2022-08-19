#include <bio/meta/tag/ttag.hpp>

//![ttag]
void bax(bio::meta::type_list<int>)    { /* do one thing     */ }

void bax(bio::meta::type_list<float>)  { /* do another thing */ }

void bat()
{
    bax(bio::meta::ttag<int>);   // calls first overload
    bax(bio::meta::ttag<float>); // calls second overload

    /* the same as calling */
    bax(bio::meta::type_list<int>{});   // calls first overload
    bax(bio::meta::type_list<float>{}); // calls second overload
}
//![ttag]
