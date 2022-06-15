---
name: Release Checklist
about: A checklist used internally for the release process.
title: "#.#.# Release"
labels: ''
assignees: ''

---

- [ ] Check for critical performance regressions.
- [ ] Check the [Changelog.md](https://github.com/seqan/seqan3/blob/master/CHANGELOG.md) for completeness.
- [ ] Update the index from cppreference.com so that up-to-date documentation links are generated.
  <details><summary>Click for detailed steps</summary><br>

  Check for [new releases](https://github.com/PeterFeicht/cppreference-doc/releases) and update the link and hash in the biocpp-doxygen.cmake.

  For the hash do

  ```
  wget -O- <link to html book> | sha256sum
  ```

  </details>
- [ ] Add versioned documentation build on docs.seqan.de.
  <details><summary>Click for detailed steps</summary>

  1. Build the documentation locally

  2. Create a #.#.# directory for the release in `/web/docs.seqan.de/htdocs/seqan/`

  3. Copy everything from the local build (`doc_usr/html/*`) into the directory.

  4. Alter the file `/web/docs.seqan.de/htdocs/index.html` with a link to the new documentation build.

  </details>

- [ ] Prepare a release note with notable features, API changes, bugs, and external dependency updates.

---

- [ ] **Tag release on Github.**

---

- [ ] Bump succeeding version number in `version.hpp`.
- [ ] Announce release on twitter.
- [ ] Notify upstream package maintainers
  - brew
  - macports
  - conda
  - debian
- [ ] Celebrate :tada: :beer:
