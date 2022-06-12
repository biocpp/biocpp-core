# BioC++ ↔ SeqAn3 {#biocpp_seqan3}

BioC++ was created by Hannes Hauswedell, one of the main developers behind SeqAn3 and author of [the book on the creation of SeqAn3](https://link.springer.com/book/10.1007/978-3-030-90990-1). BioC++ is a fork of SeqAn-3.0.2 and shares many design goals of the original SeqAn3.

Some of the strategic differences between BioC++ and SeqAn3 are:
* focus on delivering a core library with stable interfaces as soon as possible
* stronger modularisation
  * allows users to choose BioC++ libraries for some tasks and more specialised third party libraries for others
  * allows developers to apply different rules to some libraries (more stringent code reviews and high stability on important libraries; faster development for libraries in "research" status)
* simpler project administration with code ownership on Module basis
* priority for features that are requested by application developers
* improved integration with the standard library and other commonly used libraries like {fmt}
