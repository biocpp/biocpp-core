# BioC++ ↔ SeqAn3 {#biocpp_seqan3}

BioC++ was created by Hannes Hauswedell, one of the main developers behind SeqAn3 and author of [the book on the creation of SeqAn3](https://link.springer.com/book/10.1007/978-3-030-90990-1). The BioC++ core library is a fork of SeqAn-3.0.2 and shares many design goals of the original SeqAn3. Other BioC++ libraries are independent works.

One key structural difference between BioC++ and SeqAn3 is that BioC++ has much stronger **modularisation**.
BioC++ is a set of (more or less) independent libraries that each contain one or more modules.
The libraries share design patterns but are released independently, and the modules have their own namespaces.
This allows enforcing more stringent code reviews and high stability on important components like the core library while allowing experimental libraries and research projects to evolve more quickly.
It also allows the user to mix and match with other third party libraries.

How the design goals affect BioC++ compared to SeqAn3:
* stability: shipping an API stable core library is of high priority
* compactness: core library without any dependencies; much smaller, cleaned up code-base
* integration: easier to integrate into your project (fewer dependencies, include only what you need, separate namespaces); improved integration with the standard library and popular libraries like {fmt}
* adaptability: customisation via the simple `tag_invoke()` mechanism

BioC++ also aims for much simpler project administration with code ownership on Module basis.
We also hope to ship new features to our users more quickly.
