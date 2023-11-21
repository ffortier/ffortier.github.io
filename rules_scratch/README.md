# Scratch 💚 Bazel

[Bazel](https://bazel.build/) rules for [Scratch](https://scratch.mit.edu/)

Scratch is a beginer friendly game engine and Bazel is an enterprise grade build system. What do they have in common? Now they have this project. 😻

The goal is to introduce build systems, text programming language, source control and code sharing to scratch users all in one project.

## Quick Start

`WORKSPACE`

```py
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Install aspect_rules_js https://github.com/aspect-build/rules_js/releases

http_archive(
    name = "rules_scratch",
    sha256 = "<SHA256>",
    strip_prefix = "rules_scratch-<VERSION>",
    url = "https://github.com/ffortier/rules_scratch/releases/download/v<VERSION>/rules_js-v<VERSION>.tar.gz",
)

load("//scratch:repositories.bzl", "scratch_repositories")

scratch_repositories()

load("//scratch:toolchains.bzl", "scratch_register_toolchain")

scratch_register_toolchain()
```

## Rules

### Basic rules

`scratch_library`

Basic building block containing a partial `project.json` with it's assets. This target will produce a `.sb3a` file.

`scratch_binary` (TODO: Run the server)

Link all the `scratch_library` targets into a single `.sb3` and performs validation. It can be run in a browser .

### Coding rules

`scratch_sprite` (TODO)

Define a sprite using ss3 (scratch s-expression 3) files with it's assets

`scratch_stage` (TODO)

Define a stage using ss3 (scratch s-expression 3) files with it's assets

### Advanced Rules

`scratch_extension` (TODO: Define extensions)

Define an extension (ex: pen) to be used by the Language Server and the linking phase.


## TODO:

- [x] Define basic rules to build library
- [ ] Define specialized rules for sprite and stages
- [ ] Toolchain configuration
- [ ] Run scratch application with local server
- [ ] Edit scratch with local server
- [ ] Publish to scratch.mit.edu
- [ ] Code with a text language (ex: s-expressions)
- [ ] Macros
- [ ] Import sb3 as library
- [ ] Unit test
- [ ] Language server